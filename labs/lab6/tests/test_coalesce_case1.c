#include "debug.h"
#include "icslabmm.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

int main(int argc, char *argv[]) {
  // Initialize the custom allocator
  icslab_mem_init(false);

  // Print out title for first test
  printf("=== Test: No Coalescing ===\n\n"); 
  // Allocate an integer
  icslab_malloc(4);             // 1st block
  icslab_malloc(4);             // 2nd block
  void *p1 = icslab_malloc(4);  // 3rd block
  icslab_malloc(4);             // 4th block
  icslab_malloc(4);             // 5th block

  printf("\n\n=== Before calling free(p1)  ===\n\n");

  icslab_list_print();

  press_to_cont();

  icslab_free(p1);

  printf("\n\n=== After calling free(p1) ===\n\n");

  icslab_list_print();

  icslab_mem_fini();

  return EXIT_SUCCESS;
}
