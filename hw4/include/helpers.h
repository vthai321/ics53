#ifndef HELPERS_H
#define HELPERS_H
#include "icsmm.h"
#include "debug.h"
#include <stdlib.h>
#include <stdint.h>

/* Helper function declarations go here */

// global variable used to track number of pages used
extern int numOfPages;

int pagesNeeded(size_t size, int* numOfPages, void* currentPage, ics_free_header* freelist_head);

#endif
