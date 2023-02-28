#include "helpers.h"
#include "debug.h"

/* Helper function definitions go here */
int pagesNeeded(size_t size, int* numOfPages, void** currentPage, ics_free_header** freelist_head)
{
    // if we have no pages, start by initializing a page
    if(*numOfPages >= 6)
    {
        return -1; //cannot request more than 6 pages at once
    }
    size_t availablePayload = 0; // goal is to get this to equal size i.e. allocate enough space, change later to account for prev pages
    if(*numOfPages == 0)
    {
        *currentPage = ics_inc_brk(1);
        if(*currentPage == (void*)-1)
        {
            return -1;
        }
        // set the prologue
        ics_header* currentPageCasted = (ics_header*)*currentPage;
        
        (currentPageCasted)->block_size = 0;
        (currentPageCasted)->hid = PROLOGUE_MAGIC;
        (currentPageCasted)->padding_amount = 0;
        // set the epilogue
        char* endOfPageAddress = (char*)ics_get_brk() - 8; // the address of brk stores the epilogue
        ics_footer* endOfPage = (ics_footer*)endOfPageAddress;
        (endOfPage)->block_size = 0;
        (endOfPage)->fid = EPILOGUE_MAGIC;
        // set the header (8 bytes after prologue)
        char* afterPrologueAddress = (char*)*currentPage + 8;
        ics_free_header* afterPrologue = (ics_free_header*)afterPrologueAddress;
        (afterPrologue)->header.block_size = 4080;
        (afterPrologue)->header.hid = HEADER_MAGIC;
        (afterPrologue)->header.padding_amount = 0;
        (afterPrologue)->next = NULL;
        (afterPrologue)->prev = NULL;
        // set the footer (8 bytes before epilogue)
        ics_footer* blockFooter = (ics_footer*)(endOfPageAddress-8); // check pointer
        (blockFooter)->block_size = 4080;
        (blockFooter)->fid = FOOTER_MAGIC;

        availablePayload += 4064; // 4096 - 32 (header, footer, prologue, epilogue)
        *freelist_head = afterPrologue;
        (*numOfPages)++;
    }
    // allocate more pages if needed
    // curently only has the case where the malloc call spans less than or equal to a new page's payload (4080 bytes)
    // please account for the case where malloc call spans more than one page (merge the pages as 1 mega block)
    while(availablePayload < size && *numOfPages < 6)
    {
        *currentPage = ics_inc_brk(1);
        if(*currentPage == (void*)-1)
        {
            return -1;
        }
        (*numOfPages)++;
        availablePayload = availablePayload + 4080;
        // turn epilogue into this page's header (8 bytes before currentPage)
        // set the block at head's pointers to your new address?
        char* beforeCurrentPageAddress = (char*)*currentPage - 8;
        ics_free_header* beforeCurrentPage = (ics_free_header*)beforeCurrentPageAddress;
        
        char* endOfPageAddress = (char*)ics_get_brk() - 8;
        ics_footer* endOfPage = (ics_footer*)endOfPageAddress;
        
        char* endOfPageFooterAddress = endOfPageAddress - 8;
        ics_footer* endOfPageFooter = (ics_footer*)endOfPageFooterAddress;
        if(size < 4080)
        {
            (beforeCurrentPage)->header.block_size = 4088;
            (beforeCurrentPage)->header.hid = HEADER_MAGIC;
            (beforeCurrentPage)->header.padding_amount = 0;
            
            // move epilogue to end of this page

            (endOfPage)->block_size = BLOCK_SIZE_BITS;
            (endOfPage)->fid = EPILOGUE_MAGIC;
            // add a footer (8 bytes before endOfPage)
            (endOfPageFooter)->block_size = 4088;
            (endOfPageFooter)->fid = FOOTER_MAGIC;
            
            // modify the freelist, using header of page 
            if(*freelist_head != NULL)
            {
                beforeCurrentPage->next = *freelist_head;
                beforeCurrentPage->prev = NULL;
                //*freelist_head->prev = ((ics_free_header*)(*currentPage));
                (*freelist_head)->prev = beforeCurrentPage;
            }
            *freelist_head = beforeCurrentPage; // points to the header
        }
        else
        {
            // megablock
            // move epilogue and footer to end of megablock
            char* prevPageFooterAddress = beforeCurrentPageAddress - 8;
            ics_footer* prevPageFooter = (ics_footer*)prevPageFooterAddress;
            ics_footer* beforeCurrentPageAsFooter = (ics_footer*)beforeCurrentPage;
            *endOfPageFooter = (*prevPageFooter);
            *endOfPage = (*beforeCurrentPageAsFooter);
            
            // clear what was at old location of epilogue and footer
            prevPageFooter->fid = prevPageFooter->fid & 0;
            beforeCurrentPage->header.hid = beforeCurrentPage->header.hid & 0;
        }
    }
    /*
    if(*numOfPages == 6)
    {
        return -1; // an error if u try to request more pages than 6
    }
    */
    return 0;   
    
}

int removeFromList(ics_free_header** currentListBlock, ics_free_header** freelist_head)
{
    if((*currentListBlock)->next == NULL && (*currentListBlock)->prev == NULL)
    {
        // case 0: currentListBlock is only list in block
        *freelist_head = NULL;
        (*currentListBlock)->next = NULL;
        (*currentListBlock)->prev = NULL;
    }
    else if((*currentListBlock)->prev == NULL)
    {
        // case 1: remove from head when list > 1 element
        *freelist_head = (*currentListBlock)->next;
        (*currentListBlock)->next->prev = NULL;
        (*currentListBlock)->next = NULL;
        (*currentListBlock)->prev = NULL;

    }
    else if((*currentListBlock)->next == NULL)
    {
        // case 2: remove from tail when list > 1 element
        (*currentListBlock)->prev->next = NULL;
        (*currentListBlock)->next = NULL;
        (*currentListBlock)->prev = NULL;
    }
    else
    {
        // case 3: remove from middle of list
        (*currentListBlock)->prev->next = (*currentListBlock)->next;
        (*currentListBlock)->next->prev = (*currentListBlock)->prev;
        (*currentListBlock)->next = NULL;
        (*currentListBlock)->prev = NULL;        
    }
    return 0;
}