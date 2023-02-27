#include "icsmm.h"
#include "debug.h"
#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * The number of pages currently allocated is tracked using the variable below.
*/
 
/*
 * The allocator MUST store the head of its free list in this variable. 
 * Doing so will make it accessible via the extern keyword.
 * This will allow ics_freelist_print to access the value from a different file.
 */
ics_free_header *freelist_head = NULL;

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
    int numOfPages = 0;
    // void* pointer for tracking current page
    void* currentPage = ics_get_brk();
    // void* pointer to return allocated space
    void* mallocBlock = NULL;
    size_t mallocBlockSize = 0;
    if(size > 16)
    {
        mallocBlockSize = size + 16;
    }
    else
    {
        mallocBlockSize = 32;
    }

    while(pagesNeeded(size, &numOfPages, &currentPage, &freelist_head) != -1)
    {
        ics_free_header* currentListBlock = freelist_head;
        while(currentListBlock != NULL)
        {
            size_t currentBlockSize = ((ics_free_header*)currentListBlock)->header.block_size;
            printf("current block size: %ld\n", currentBlockSize);
            // don't forget to account for case of splinters (payload < 16 or block size < 32)
            if(currentBlockSize >= mallocBlockSize)
            {
                //split block if our old block remains >32 bytes in size; otherwise, pad it
                size_t mallocBlockDifference = currentBlockSize - mallocBlockSize;
                if(mallocBlockDifference >= 32)
                {
                    // case 1: we can split the old block
                    mallocBlock = (char*)currentListBlock + 8;
                    void* currentListBlockFooter = (char*)currentListBlock + (currentBlockSize - 8);
                    ((ics_free_header*)currentListBlock)->header.block_size = mallocBlockSize;
                    ((ics_footer*)currentListBlockFooter)->block_size -= mallocBlockSize;

                    // create the new header and footer
                    void* newHeader = currentListBlock + mallocBlockSize;
                    void* newFooter = currentListBlock + mallocBlockSize - 8;

                    ((ics_free_header*)newHeader)->header.block_size = ((ics_footer*)currentListBlockFooter)->block_size;
                    ((ics_free_header*)newHeader)->header.hid = HEADER_MAGIC;
                    ((ics_free_header*)newHeader)->header.padding_amount = 0;

                    ((ics_footer*)newFooter)->block_size = mallocBlockSize;
                    ((ics_footer*)newFooter)->fid = FOOTER_MAGIC;
                    return mallocBlock;

                }
                else
                {
                    //case 2: use up the entire block and pad as needed. Padding value in header = mallocBlockDifference
                    mallocBlock = (char*)currentListBlock + 8; // point to first byte of payload?
                    ics_free_header* blockToRemove = (ics_free_header*)currentListBlock;
                    removeFromList(&blockToRemove, &freelist_head);
                    ((ics_free_header*)currentListBlock)->header.padding_amount = mallocBlockSize - size;
                    return mallocBlock;
                }
            }
            currentListBlock = currentListBlock->next;
        }
    }
    return NULL; 
    // to do: set errno to ENOMEM?
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
    return -99999;
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
