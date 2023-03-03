#include "icsmm.h"
#include "debug.h"
#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>


 /*
 * The allocator MUST store the head of its free list in this variable. 
 * Doing so will make it accessible via the extern keyword.
 * This will allow ics_freelist_print to access the value from a different file.
 */
ics_free_header *freelist_head = NULL;

void* bottomOfHeap = NULL;
int numOfPages = 0;

/*
 * This is your implementation of malloc. It acquires uninitialized memory from  
 * ics_inc_brk() that is 16-byte aligned, as needed.
 *
 * @param size The number of bytes requested to be allocated.
 *
 * @return If successful, the pointer to a valid region of memory of at least the
 * requested size is returned. Otherwise, NULL is returned and errno is set to 
 * ENOMEM - representing failure to allocate space for the request.
 * 
 * If size is 0, then NULL is returned and errno is set to EINVAL - representing
 * an invalid request.
 */
void *ics_malloc(size_t size) 
{
    if(size < 0 || size > 24576)
    {
        errno = EINVAL;
        return NULL;
    }
    // void* pointer to return allocated space
    void* mallocBlock = NULL;
    void* currentPage = NULL;
    size_t mallocBlockSize = 0;
    size_t calcPadding = 0; 
    
    if(size > 16)
    {
        int remainderSize = size % 16;
        if(remainderSize != 0)
        {
            calcPadding = 16 - remainderSize;
            mallocBlockSize = calcPadding + size + 16;
        }
        else
        {
            mallocBlockSize = size + 16;  
            calcPadding = 0;
        }
    }
    else
    {
        mallocBlockSize = 32;
        calcPadding = 16 - size; // should be 16 - size, right?
    }
    
    //printf("MALLOCBLOCKSIZE: %d\n"mallocBlockSize);
    if(numOfPages == 0)
    {
        pagesNeeded(size, &numOfPages, &currentPage, &freelist_head, &bottomOfHeap, mallocBlockSize);
    }
    ics_free_header* currentListBlock = freelist_head;
    //debug
    //printf("DEBUG STATEMENT1 BELOW\n");
    //ics_freelist_print();
    
    mallocBlock = loopThroughList(currentListBlock, calcPadding, mallocBlockSize, size);
    int pagesNeededResult = 0;
    
    while(mallocBlock == NULL)
    {
        //printf("DEBUG STATEMENT2 BELOW\n");
        //ics_freelist_print();
        pagesNeededResult = pagesNeeded(size, &numOfPages, &currentPage, &freelist_head, &bottomOfHeap, mallocBlockSize);
        if(pagesNeededResult == -1)
        {
            return NULL;
            // errno has been set
        }
        currentListBlock = freelist_head;   
        mallocBlock = loopThroughList(currentListBlock, calcPadding, mallocBlockSize, size);
    }
    return mallocBlock;
}

/*
 * Marks a dynamically allocated block as no longer in use and coalesces with 
 * adjacent free blocks (as specified by Homework Document). 
 * Adds the block to the appropriate bucket according to the block placement policy.
 *
 * @param ptr Address of dynamically allocated memory returned by the function
 * ics_malloc.
 * 
 * @return 0 upon success, -1 if error and set errno accordingly.
 */
int ics_free(void *ptr) 
{ 
    void* headerOfBlockAddress = ptr - 8; // be careful here
    ics_header* headerOfBlock = (ics_header*)headerOfBlockAddress;
    size_t headerSize = headerOfBlock->block_size;
    
    void* footerOfBlockAddress = headerOfBlockAddress + (headerSize - 1) - 8;
    ics_footer* footerOfBlock = (ics_footer*)footerOfBlockAddress;
    size_t footerSize = footerOfBlock->block_size;
    
    void* brkLocation = ics_get_brk();
    // alloc bit is set of block size is ODD; if even is a mistake
    // below is a set of error checking if statements
    // don't forget to set errno
    
    if(headerSize % 2 == 0 || footerSize % 2 ==0)
    {
        //set errno
        errno = EINVAL;
        return -1;
    }
    else if(ptr >= brkLocation || ptr < bottomOfHeap) // bottomOfHeap was intitialized in first call to ics_malloc()
    {
        errno = EINVAL;
        return -1;
    }
    else if(headerSize != footerSize)
    {
        errno = EINVAL;
        return -1;
    }
    else if(headerOfBlock->hid != HEADER_MAGIC || footerOfBlock->fid != FOOTER_MAGIC)
    {
        errno = EINVAL;
        return -1;
    }

    // free the block
    headerOfBlock->block_size -= 1;
    footerOfBlock->block_size -=1;

    //TODO: check adjacent blocks in memory (via header or footer) to coalesce
    // case 2: higher address (look at header of next block)
    // if the next block is free, remove that block from the freelist

    void* nextBlockHeaderAddress = footerOfBlockAddress + 8;
    ics_free_header* nextBlockHeader = (ics_free_header*)nextBlockHeaderAddress;
    int nextBlockSize = nextBlockHeader->header.block_size;

    void* nextBlockFooterAddress = nextBlockHeaderAddress + (nextBlockSize - 8);
    ics_footer* nextBlockFooter = (ics_footer*)nextBlockFooterAddress;

    if(nextBlockSize % 2 == 0 && nextBlockSize != 0) // cannot coalesce with epilogue, so check that block size != 0...
    {
        //coalesce
        removeFromList(nextBlockHeader, &freelist_head);
        size_t newBlockSize = nextBlockSize + headerOfBlock->block_size;
        // update the header of the block freed by free() and the existing footer of next block
        headerOfBlock->block_size = newBlockSize;
        nextBlockFooter->block_size = newBlockSize;
        // can ignore the middle parts
    }

    //insert it back into the free list
    ics_free_header* freeHeader = (ics_free_header*)headerOfBlockAddress;
    if(freelist_head == NULL)
    {
        freeHeader->next = NULL;
        freeHeader->prev = NULL;
        freelist_head = freeHeader;
    }
    else
    {
        freeHeader->next = freelist_head;
        freeHeader->prev = NULL;
        freelist_head->prev = freeHeader;
        freelist_head = freeHeader;
    }

    return 0;
}

/********************** EXTRA CREDIT ***************************************/

/*
 * Resizes the dynamically allocated memory, pointed to by ptr, to at least size 
 * bytes. See Homework Document for specific description.
 *
 * @param ptr Address of the previously allocated memory region.
 * @param size The minimum size to resize the allocated memory to.
 * @return If successful, the pointer to the block of allocated memory is
 * returned. Else, NULL is returned and errno is set appropriately.
 *
 * If there is no memory available ics_malloc will set errno to ENOMEM. 
 *
 * If ics_realloc is called with an invalid pointer, set errno to EINVAL. See ics_free
 * for more details.
 *
 * If ics_realloc is called with a valid pointer and a size of 0, the allocated     
 * block is free'd and return NULL.
 */
void *ics_realloc(void *ptr, size_t size) {
    return NULL;
}
