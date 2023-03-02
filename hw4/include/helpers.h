#ifndef HELPERS_H
#define HELPERS_H
#include "icsmm.h"
#include "debug.h"
#include <stdlib.h>
#include <stdint.h>

/* Helper function declarations go here */

// global variable used to track number of pages used
extern int numOfPages;

/*
    Calculates the number of pages to allocate if there are no available free blocks to satisfy malloc
    Parameters: size_t size, the size of block to allocate in bytes, int* numOfPages, to keep track of numOfPages currently allocated;
    void** currentPage, a pointer to the pointer to start of most recently allocated page, 
    ics_free_header** freelist_head, a pointer to the pointer of head of list of free blocks
    Return value: 0 on success, -1 on failure
*/
int pagesNeeded(size_t size, int* numOfPages, void** currentPage, ics_free_header** freelist_head, void** bottomOfHeap, size_t mallocBlockSize);

/*
    Removes a free block from the freelist, using a process akin to removal of a node from a doubly linked list
    Parameters: ics_free_header** currentListBlock, pointer to the list block we want to remove;
    ics_free_header* freelist_head, pointer to head of the free list
    Return value: 0 on success
*/
int removeFromList(ics_free_header* currentListBlock, ics_free_header** freelist_head);

void* loopThroughList(ics_free_header* currentListBlock, size_t calcPadding, size_t mallocBlockSize);

#endif
