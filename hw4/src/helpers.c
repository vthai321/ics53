#include "helpers.h"
#include "debug.h"

/* Helper function definitions go here */

int pagesNeeded(size_t size, int* numOfPages, void** currentPage, ics_free_header** freelist_head, void** bottomOfHeap, size_t mallocBlockSize)
{
    size_t availableSize = 0;
    // if we have no pages, start by initializing a page
    /*
    if(*numOfPages >= 6)
    {
        return -1; //cannot request more than 6 pages at once
    }
    */
    
    if(*numOfPages == 0)
    {
        *bottomOfHeap = ics_get_brk(); // will be used for free() error checking
        if(mallocBlockSize <= 4080)
        {
            *currentPage = ics_inc_brk(1);
            if(*currentPage == (void*)-1)
            {
                return -1;
            }
            
            // set the prologue
            ics_header* currentPageCasted = (ics_header*)*currentPage;
            currentPageCasted->block_size = 0;
            currentPageCasted->hid = PROLOGUE_MAGIC;
            currentPageCasted->padding_amount = 0;

            // set the epilogue
            void* endOfPageAddress = ics_get_brk() - 8; // the address of brk stores the epilogue
            ics_footer* endOfPage = (ics_footer*)endOfPageAddress;
            endOfPage->block_size = 0;
            endOfPage->fid = EPILOGUE_MAGIC;
        
            // set the header (8 bytes after prologue)
            void* afterPrologueAddress = (*currentPage) + 8;
            ics_free_header* afterPrologue = (ics_free_header*)afterPrologueAddress;
            afterPrologue->header.block_size = 4080;
            afterPrologue->header.hid = HEADER_MAGIC;
            afterPrologue->header.padding_amount = 0;
            afterPrologue->next = NULL;
            afterPrologue->prev = NULL;
        
            // set the footer (8 bytes before epilogue)
            ics_footer* blockFooter = (ics_footer*)(endOfPageAddress-8); // check pointer
            blockFooter->block_size = afterPrologue->header.block_size;
            blockFooter->fid = FOOTER_MAGIC;

            //availableSize += 4080; // 4096 - 16 (header, footer, prologue, epilogue)
            *freelist_head = afterPrologue;
            (*numOfPages)++;
            return 0;
        }
        else if(mallocBlockSize <= 8168)
        {
            *currentPage = ics_inc_brk(2);
            if(*currentPage == (void*)-1)
            {
                return -1;
            }  
            // set the prologue
            ics_header* currentPageCasted = (ics_header*)*currentPage;
            currentPageCasted->block_size = 0;
            currentPageCasted->hid = PROLOGUE_MAGIC;
            currentPageCasted->padding_amount = 0;

            // set the epilogue
            void* endOfPageAddress = ics_get_brk() - 8; // the address of brk stores the epilogue
            ics_footer* endOfPage = (ics_footer*)endOfPageAddress;
            endOfPage->block_size = 0;
            endOfPage->fid = EPILOGUE_MAGIC;
        
            // set the header (8 bytes after prologue)
            void* afterPrologueAddress = (*currentPage) + 8;
            ics_free_header* afterPrologue = (ics_free_header*)afterPrologueAddress;
            afterPrologue->header.block_size = 8176;
            afterPrologue->header.hid = HEADER_MAGIC;
            afterPrologue->header.padding_amount = 0;
            afterPrologue->next = NULL;
            afterPrologue->prev = NULL;
        
            // set the footer (8 bytes before epilogue)
            ics_footer* blockFooter = (ics_footer*)(endOfPageAddress-8); // check pointer
            blockFooter->block_size = afterPrologue->header.block_size;
            blockFooter->fid = FOOTER_MAGIC;

            //availableSize += 8176; // 4096 - 16 (header, footer, prologue, epilogue)
            *freelist_head = afterPrologue;
            (*numOfPages) += 2;
            return 0;
        }
        else
        {
            // first page will store 4088, middle pages store 4096, last pages store 4088
            int blockSizeDifference = mallocBlockSize - (8168); // case of 2 pages
            int sizeAllocated = 8176;
            int numOfMidPages = 0;
            while(blockSizeDifference > 0 && numOfMidPages < 4)
            {
                numOfMidPages++;
                // everytime we add a new mid page, we get 4096 more
                blockSizeDifference -= 4096;
            }
            
            // add this megablock to the freelist
            *currentPage = ics_inc_brk(numOfMidPages + 2);
            sizeAllocated += (numOfMidPages * 4096);
            
            if(*currentPage == (void*)-1)
            {
                return -1;
            }  
            // set the prologue
            ics_header* currentPageCasted = (ics_header*)*currentPage;
            currentPageCasted->block_size = 0;
            currentPageCasted->hid = PROLOGUE_MAGIC;
            currentPageCasted->padding_amount = 0;

            // set the epilogue
            void* endOfPageAddress = ics_get_brk() - 8; // the address of brk stores the epilogue
            ics_footer* endOfPage = (ics_footer*)endOfPageAddress;
            endOfPage->block_size = 0;
            endOfPage->fid = EPILOGUE_MAGIC;
        
            // set the header (8 bytes after prologue)
            void* afterPrologueAddress = (*currentPage) + 8;
            ics_free_header* afterPrologue = (ics_free_header*)afterPrologueAddress;
            afterPrologue->header.block_size = sizeAllocated;
            afterPrologue->header.hid = HEADER_MAGIC;
            afterPrologue->header.padding_amount = 0;
            afterPrologue->next = NULL;
            afterPrologue->prev = NULL;
        
            // set the footer (8 bytes before epilogue)
            ics_footer* blockFooter = (ics_footer*)(endOfPageAddress-8); // check pointer
            blockFooter->block_size = afterPrologue->header.block_size;
            blockFooter->fid = FOOTER_MAGIC;

            *freelist_head = afterPrologue;
            (*numOfPages) += numOfMidPages + 2;
            return 0;
        }
    }
    else
    {
        // This case is for non-megablock page allocation
        // a new page added to the end stores 4096
        if(mallocBlockSize <= 4096)
        {
            *currentPage = ics_inc_brk(1);
            if(*currentPage == (void*)-1)
            {
                return -1;
            }
            (*numOfPages)++;
            //availableSize += 4088;
            
            // retrieve relevant addresses
            void* beforeCurrentPageAddress = (*currentPage) - 8; // for header
            ics_free_header* beforeCurrentPage = (ics_free_header*)beforeCurrentPageAddress;
            
            void* endOfPageAddress = ics_get_brk() - 8; // for epilogue
            ics_footer* endOfPage = (ics_footer*)endOfPageAddress;
            
            void* endOfPageFooterAddress = endOfPageAddress - 8; // for footer
            ics_footer* endOfPageFooter = (ics_footer*)endOfPageFooterAddress;

            // create a new free block
            // epilogue becomes header
            beforeCurrentPage->header.block_size = 4096; // check later
            beforeCurrentPage->header.hid = HEADER_MAGIC;
            beforeCurrentPage->header.padding_amount = 0;
            
            // move epilogue to end of this page
            endOfPage->block_size = 0;
            endOfPage->fid = EPILOGUE_MAGIC;
            
            // add a footer (8 bytes before endOfPage)
            endOfPageFooter->block_size = beforeCurrentPage->header.block_size;
            endOfPageFooter->fid = FOOTER_MAGIC;
            
            // modify the freelist, using header of page 
            if(*freelist_head == NULL)
            {
                beforeCurrentPage->next = NULL;
                beforeCurrentPage->prev = NULL;
                *freelist_head = beforeCurrentPage; // points to the header
            }
            else
            {
                beforeCurrentPage->next = *freelist_head;
                beforeCurrentPage->prev = NULL;
                (*freelist_head)->prev = beforeCurrentPage;
                *freelist_head = beforeCurrentPage; // points to the header
            }
            return 0;
        }
        else
        {
            //megablock time
            // add 1 page as required, then determine the rest
            // the new page added is a beginning page and stores 4088
            // subsequent "mid" pages add 4096, but the last page only adds 4088
            void* newHeaderAddress = ics_get_brk() - 8; //location of the epilogue
            int blockSizeDifference = mallocBlockSize - 4088; 
            int sizeAllocated = 4096;
            int numOfMidPages = 1;
            blockSizeDifference -= 4096;
            while(blockSizeDifference > 0 && numOfMidPages < 4)
            {
                numOfMidPages++;
                // everytime we add a new mid page, we get 4096 more
                blockSizeDifference -= 4096;
            }
            
            // add this megablock to the freelist
            *currentPage = ics_inc_brk(1 + numOfMidPages);
            sizeAllocated += (numOfMidPages * 4096);
            
            if(*currentPage == (void*)-1)
            {
                return -1;
            }  

            // set the epilogue
            void* endOfPageAddress = ics_get_brk() - 8; // the address of brk stores the epilogue
            ics_footer* endOfPage = (ics_footer*)endOfPageAddress;
            endOfPage->block_size = 0;
            endOfPage->fid = EPILOGUE_MAGIC;
        
            // update the header (epilogue of prev page)
            ics_free_header* newHeader = (ics_free_header*)newHeaderAddress;
            newHeader->header.block_size = sizeAllocated;
            newHeader->header.hid = HEADER_MAGIC;
            newHeader->header.padding_amount = 0;
            newHeader->next = NULL;
            newHeader->prev = NULL;
        
            // set the footer (8 bytes before epilogue)
            ics_footer* blockFooter = (ics_footer*)(endOfPageAddress-8); // check pointer
            blockFooter->block_size = newHeader->header.block_size;
            blockFooter->fid = FOOTER_MAGIC;

            *freelist_head = newHeader;
            (*numOfPages) += numOfMidPages + 2;
            return 0;
        }
        return 0;
    }
}

int removeFromList(ics_free_header* currentListBlock, ics_free_header** freelist_head)
{
    if(currentListBlock->next == NULL && currentListBlock->prev == NULL)
    {
        // case 0: currentListBlock is only list in block
        *freelist_head = NULL;
        currentListBlock->next = NULL;
        currentListBlock->prev = NULL;
        return 0; 
    }
    else if(currentListBlock->prev == NULL)
    {
        // case 1: remove from head when list > 1 element
        *freelist_head = currentListBlock->next;
        currentListBlock->next->prev = NULL;
        currentListBlock->next = NULL;
        currentListBlock->prev = NULL;
        return 0; 
    }
    else if(currentListBlock->next == NULL)
    {
        // case 2: remove from tail when list > 1 element
        currentListBlock->prev->next = NULL;
        currentListBlock->next = NULL;
        currentListBlock->prev = NULL;
        return 0; 
    }
    else if(currentListBlock -> next != NULL && currentListBlock->prev != NULL)
    {
        // case 3: remove from middle of list
        currentListBlock->prev->next = currentListBlock->next;
        currentListBlock->next->prev = currentListBlock->prev;
        currentListBlock->next = NULL;
        currentListBlock->prev = NULL;
        return 0;        
    }
    return -1;
}

void* loopThroughList(ics_free_header* currentListBlock, size_t calcPadding, size_t mallocBlockSize, size_t size)
{
    void* mallocBlock = NULL;
    while(currentListBlock != NULL)
    {
        size_t currentBlockSize = currentListBlock->header.block_size;
        size_t mallocBlockDifference = currentBlockSize - mallocBlockSize;
        // don't forget to account for case of splinters (payload < 16 or block size < 32)
        if(currentBlockSize >= mallocBlockSize) // be careful w/ size_t arithmetic
        {
            //split block if our old block remains >32 bytes in size; otherwise, pad it
            if(mallocBlockDifference >= 32)
            {
                // case 1: we can split the old block
                mallocBlock = (char*)currentListBlock + 8;
                void* currentListBlockFooterAddress = (char*)currentListBlock + (currentBlockSize - 8);
                ics_footer* currentListBlockFooter = (ics_footer*)currentListBlockFooterAddress;
                //printf("ACTUAL FOOTER ADDRESS CALCULATED: %p\n", currentListBlockFooterAddress);

                (currentListBlock)->header.block_size = mallocBlockSize;
                (currentListBlockFooter)->block_size -= mallocBlockSize;

                // create the new header and footer
                
                void* newHeaderAddress = (char*)currentListBlock + mallocBlockSize;
                void* newFooterAddress = (char*)currentListBlock + mallocBlockSize - 8;
                ics_free_header* newHeader = (ics_free_header*)newHeaderAddress;
                ics_footer* newFooter = (ics_footer*)newFooterAddress;

                (newHeader)->header.block_size = (currentListBlockFooter)->block_size;
                (newHeader)->header.hid = HEADER_MAGIC;
                (newHeader)->header.padding_amount = 0;

                (newFooter)->block_size = mallocBlockSize;
                (newFooter)->fid = FOOTER_MAGIC;

                // add this new split block to head of freelist
                freelist_head->prev = newHeader;
                newHeader->next = freelist_head;
                newHeader->prev = NULL;
                freelist_head = newHeader;

                //use bitwise OR on the old header and new footer
                    currentListBlock->header.block_size  = currentListBlock->header.block_size | 1;
                    newFooter->block_size  = newFooter->block_size | 1;
                
                //calculate padding as needed
                //currentListBlock->header.padding_amount = currentListBlock->header.block_size - 16 - size - 1;
                currentListBlock->header.padding_amount = calcPadding;
                removeFromList(currentListBlock, &freelist_head);
                return mallocBlock;
            }
            else if(mallocBlockDifference < 32)
            {
                //case 2: use up the entire block and pad as needed. Padding value in header = mallocBlockDifference
                mallocBlock = (char*)currentListBlock + 8; // point to first byte of payload?
                void* currentListBlockFooterAddress = (char*)currentListBlock + (currentBlockSize - 8);
                ics_footer* currentListBlockFooter = (ics_footer*)currentListBlockFooterAddress;
                removeFromList(currentListBlock, &freelist_head);
                
                //the padding is just the rest of the block
                size_t currentListBlockPayload = currentListBlock->header.block_size - 16;
                currentListBlock->header.padding_amount = currentListBlockPayload - size; 

                //perform bitwise OR 
                currentListBlock->header.block_size = currentListBlock->header.block_size | 1;
                currentListBlockFooter->block_size = currentListBlockFooter->block_size | 1;
                return mallocBlock;
            }
        }
        // should be reached if currentBlockSize payload is too small to accomodate request 
        currentListBlock = currentListBlock->next;
    }
    return NULL;
}