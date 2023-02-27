#include "helpers.h"
#include "debug.h"

/* Helper function definitions go here */
int pagesNeeded(size_t size, int* numOfPages, void* currentPage, ics_free_header* freelist_head)
{
    // if we have no pages, start by initializing a page
    if(size > 6)
    {
        return -1; //cannot request more than 6 pages at once
    }
    size_t availablePayload = 0; // goal is to get this to equal size i.e. allocate enough space, change later to account for prev pages
    if(*numOfPages == 0)
    {
        currentPage = ics_inc_brk(1);
        if(currentPage == (void*)-1)
        {
            return -1;
        }
        // set the prologue
        ((ics_header*)currentPage)->block_size = 0;
        ((ics_header*)currentPage)->hid = PROLOGUE_MAGIC;
        ((ics_header*)currentPage)->padding_amount = 0;
        // set the epilogue
        char* endOfPage = (char*)ics_get_brk() - 8; // the address of brk stores the epilogue
        ((ics_footer*)endOfPage)->block_size = 0;
        ((ics_footer*)endOfPage)->fid = EPILOGUE_MAGIC;
        // set the header (8 bytes after prologue)
        char* afterPrologue = (char*)currentPage + 8;
        ((ics_free_header*)(afterPrologue))->header.block_size = 4080;
        ((ics_free_header*)(afterPrologue))->header.hid = HEADER_MAGIC;
        ((ics_free_header*)(afterPrologue))->header.padding_amount = 0;
        ((ics_free_header*)(afterPrologue))->next = NULL;
        ((ics_free_header*)(afterPrologue))->prev = NULL;
        // set the footer (8 bytes before epilogue)
        ((ics_footer*)(endOfPage - 8))->block_size = 4080;
        ((ics_footer*)(endOfPage - 8))->fid = FOOTER_MAGIC;

        availablePayload += 4064; // 4096 - 32 (header, footer, prologue, epilogue)
        freelist_head = ((ics_free_header*)(afterPrologue));
        *numOfPages = *numOfPages + 1;
    }

    // allocate more pages if needed
    while(availablePayload < size && *numOfPages < 6)
    {
        currentPage = ics_inc_brk(1);
        if(currentPage == (void*)-1)
        {
            return -1;
        }
        *numOfPages = *numOfPages + 1;
        availablePayload = availablePayload + 4080;
        // turn epilogue into this page's header (8 bytes before currentPage)
        // set the block at head's pointers to your new address?
        char* beforeCurrentPage = (char*)currentPage - 8;
        //printf("beforeCurrentPage->prev: %p\n", ((ics_free_header*)beforeCurrentPage)->prev);
        ((ics_free_header*)(beforeCurrentPage))->header.block_size = 4088;
        ((ics_free_header*)(beforeCurrentPage))->header.hid = HEADER_MAGIC;
        ((ics_free_header*)(beforeCurrentPage))->header.padding_amount = 0;
        
        if(freelist_head != NULL)
        {
            ((ics_free_header*)(beforeCurrentPage))->next = freelist_head;
            ((ics_free_header*)(beforeCurrentPage))->prev = NULL;
            freelist_head->prev = ((ics_free_header*)(currentPage));
        }
        // move epilogue to end of this page
        char* endOfPage = (char*)ics_get_brk() - 8;
        ((ics_footer*)endOfPage)->block_size = BLOCK_SIZE_BITS;
        ((ics_footer*)endOfPage)->fid = EPILOGUE_MAGIC;
        // add a footer (8 bytes before endOfPage)
        ((ics_footer*)(endOfPage-8))->block_size = 4088;
        ((ics_footer*)(endOfPage-8))->fid = FOOTER_MAGIC;

        freelist_head = (ics_free_header*)currentPage; // points to the header
    }
    if(*numberOfPages == 6)
    {
        return -1; // an error if u try to request more pages than 6
    }
    return 0;   
}

int removeFromList(ics_free_header* currentListBlock, ics_free_header* freelist_head)
{
    if(currentListBlock->next == NULL && currentListBlock->next == NULL)
    {
        // case 0: currentListBlock is only list in block
        freelist_head = NULL;
        currentListBlock->next = NULL;
        currentListBlock->prev = NULL;
    }
    else if(currentListBlock->prev == NULL)
    {
        // case 1: remove from head when list > 1 element
        freelist_head = currentListBlock->next;
        currentListBlock->next->prev = NULL;
        currentListBlock->next = NULL;
        currentListBlock->prev = NULL;

    }
    else if(currentListBlock->next == NULL)
    {
        // case 2: remove from tail when list > 1 element
        currentListBlock->prev->next = NULL;
        currentListBlock->next = NULL;
        currentListBlock->prev = NULL;
    }
    else
    {
        // case 3: remove from middle of list
        currentListBlock->prev->next = currentListBlock->next;
        currentListBlock->next->prev = currentListBlock->prev;
        currentListBlock->next = NULL;
        currentListBlock->prev = NULL;        
    }
    return 0;
}