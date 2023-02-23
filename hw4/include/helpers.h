#ifndef HELPERS_H
#define HELPERS_H
#include <stdlib.h>
#include <stdint.h>

/* Helper function declarations go here */

// global variable used to track number of pages used
extern int numOfPages;

int pagesNeeded(size_t size);

#endif
