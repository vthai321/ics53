#include "helpers.h"
#include "debug.h"

/* Helper function definitions go here */
int pagesNeeded(size_t size, int* numOfPages, void* currentPage, ics_free_header* freelist_head)
{
    // if we have no pages, start by initializing a page
    size_t availablePayload = 0; // goal is to get this to equal size i.e. allocate enough space
    if(*numOfPages == 0)
    {
        currentPage = ics_inc_brk(1);
        if(*(int*)currentPage == -1)
        {
            return -1;
        }
        // set the prologue
        ((ics_header*)currentPage)->block_size = 0;
        ((ics_header*)currentPage)->hid = PROLOGUE_MAGIC;
        ((ics_header*)currentPage)->padding_amount = 0;
        // set the epilogue
        void* endOfPage = currentPage += 4096;
        ((ics_footer*)endOfPage)->block_size = BLOCK_SIZE_BITS;
        ((ics_footer*)endOfPage)->fid = EPILOGUE_MAGIC;
        // set the header
        ((ics_header*)currentPage)->block_size = 4064;
        ((ics_header*)currentPage)->hid = HEADER_MAGIC;
        ((ics_header*)currentPage)->padding_amount = 0;
        ((ics_free_header*)(currentPage))->next = NULL;
        ((ics_free_header*)(currentPage))->prev = NULL;
        // set the footer
        ((ics_footer*)(endOfPage-8))->block_size = 4064;
        ((ics_footer*)(endOfPage-8))->fid = FOOTER_MAGIC;

        availablePayload += 4064; // 4096 - 32 (header, footer, prologue, epilogue)
        freelist_head = (ics_free_header*)(currentPage + 8);
        *numOfPages = *numOfPages + 1;
    }

    // allocate more pages if needed
    while(*numOfPages < 6 && availablePayload < size)
    {
        currentPage = ics_inc_brk(1);
        if(*(int*)currentPage == -1)
        {
            return -1;
        }
        *numOfPages = *numOfPages + 1;
        availablePayload = availablePayload + 4096 + 8 - 8 - 8;
        // turn epilogue into this page's header (8 bytes before currentPage)
        // set the block at head's pointers to your new address?
        ((ics_header*)(currentPage-8))->block_size = 4088;
        ((ics_header*)(currentPage-8))->hid = HEADER_MAGIC;
        ((ics_header*)(currentPage-8))->padding_amount = 0;
        ((ics_free_header*)(currentPage))->next = NULL;
        ((ics_free_header*)(currentPage))->prev = NULL;
        freelist_head->prev = ((ics_free_header*)(currentPage))->prev; // should point to the prev pointer... right?
        // move epilogue to end of this page
        void* endOfPage = currentPage += 4096;
        ((ics_footer*)endOfPage)->block_size = BLOCK_SIZE_BITS;
        ((ics_footer*)endOfPage)->fid = EPILOGUE_MAGIC;
        // add a footer (8 bytes before endOfPage)
        ((ics_footer*)(endOfPage-8))->block_size = 4088;
        ((ics_footer*)(endOfPage-8))->fid = FOOTER_MAGIC;

        freelist_head = (ics_free_header*)currentPage - 8; // points to the header, which is 8 bytes before currentPage
    }

    return 0;
    
}