#include "debug.h"
#include "helpers.h"
#include "icsmm.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>

#define VALUE1_VALUE 320
#define VALUE2_VALUE 0xDEADBEEFF00D

void press_to_cont() {
    printf("Press Enter to Continue");
    while (getchar() != '\n')
      ;
    printf("\n");
}

void null_check(void* ptr, long size) {
    if (ptr == NULL) {
      error(
          "Failed to allocate %lu byte(s) for an integer using ics_malloc.\n",
          size);
      error("%s\n", "Aborting...");
      assert(false);
    } else {
      success("ics_malloc returned a non-null address: %p\n", (void *)(ptr));
    }
}

void payload_check(void* ptr) {
    if ((unsigned long)(ptr) % 16 != 0) {
      warn("Returned payload address is not divisble by a quadword. %p %% 16 "
           "= %lu\n",
           (void *)(ptr), (unsigned long)(ptr) % 16);
    }
}

int main(int argc, char *argv[]) {
  // Initialize the custom allocator
  ics_mem_init();

  /*
  // Tell the user about the fields
  info("Initialized heap\n");
  press_to_cont();

  // Print out title for first test
  printf("=== Test1: Allocation test ===\n");
  // Test #1: Allocate an integer
  // Test #1.1: Allocate 773 bytes
  //int *value1 = ics_malloc(sizeof(7773));
  int *value1 = ics_malloc(sizeof(int));
  null_check(value1, sizeof(int));
  //null_check(value1, 7737);
  payload_check(value1);
  ics_payload_print((void*)value1);
  ics_freelist_print();
  press_to_cont();

  // Now assign a value
  printf("=== Test2: Assignment test ===\n");
  info("Attempting to assign value1 = %d\n", VALUE1_VALUE);
  // Assign the value
  *value1 = VALUE1_VALUE;
  // Now check its value
  CHECK_PRIM_CONTENTS(value1, VALUE1_VALUE, "%d", "value1");
  ics_payload_print((void*)value1);
  // print the freelist (so far)
  ics_freelist_print();
  press_to_cont();

  printf("=== Test3: Allocate a second variable ===\n");
  info("Attempting to assign value2 = %ld\n", VALUE2_VALUE);
  long *value2 = ics_malloc(sizeof(long));
  null_check(value2, sizeof(long));
  payload_check(value2);
  // Assign a value
  *value2 = VALUE2_VALUE;
  // Check value
  CHECK_PRIM_CONTENTS(value2, VALUE2_VALUE, "%ld", "value2");
  ics_payload_print((void*)value2);
  press_to_cont();

  printf("=== Test4: does value1 still equal %d ===\n", VALUE1_VALUE);
  CHECK_PRIM_CONTENTS(value1, VALUE1_VALUE, "%d", "value1");
  ics_payload_print((void*)value1);
  press_to_cont();

  // Free a variable
  printf("=== Test5: Free a block and snapshot ===\n");
  info("%s\n", "Freeing value1...");
  ics_free(value1);
  ics_freelist_print();
  press_to_cont();

  // Allocate a large chunk of memory and then free it
  printf("=== Test6: 8192 byte allocation ===\n");
  void *memory = ics_malloc(8192);
  ics_freelist_print();
  press_to_cont();
  
  //ics_free(memory);
  
  ics_freelist_print();
  press_to_cont();

*/

void *ptr0 = ics_malloc(1);
void *ptr1 = ics_malloc(1);
void *ptr2 = ics_malloc(1);
void *ptr3 = ics_malloc(1);
void *ptr4 = ics_malloc(1);
void *ptr5 = ics_malloc(1);
void *ptr6 = ics_malloc(1);
void *ptr7 = ics_malloc(1);
void *ptr8 = ics_malloc(1);
void *ptr9 = ics_malloc(1);
void *ptr10 = ics_malloc(1);
void *ptr11 = ics_malloc(1);
void *ptr12 = ics_malloc(1);
void *ptr13 = ics_malloc(1);
void *ptr14 = ics_malloc(1);
void *ptr15 = ics_malloc(1);
void *ptr16 = ics_malloc(1);
void *ptr17 = ics_malloc(1);
void *ptr18 = ics_malloc(1);
void *ptr19 = ics_malloc(1);
void *ptr20 = ics_malloc(1);
void *ptr21 = ics_malloc(1);
void *ptr22 = ics_malloc(1);
void *ptr23 = ics_malloc(1);
void *ptr24 = ics_malloc(1);
void *ptr25 = ics_malloc(1);
void *ptr26 = ics_malloc(1);
void *ptr27 = ics_malloc(1);
void *ptr28 = ics_malloc(1);
void *ptr29 = ics_malloc(1);
void *ptr30 = ics_malloc(1);
void *ptr31 = ics_malloc(1);
void *ptr32 = ics_malloc(1);
void *ptr33 = ics_malloc(1);
void *ptr34 = ics_malloc(1);
void *ptr35 = ics_malloc(1);
void *ptr36 = ics_malloc(1);
void *ptr37 = ics_malloc(1);
void *ptr38 = ics_malloc(1);
void *ptr39 = ics_malloc(1);
void *ptr40 = ics_malloc(1);
void *ptr41 = ics_malloc(1);
void *ptr42 = ics_malloc(1);
void *ptr43 = ics_malloc(1);
void *ptr44 = ics_malloc(1);
void *ptr45 = ics_malloc(1);
void *ptr46 = ics_malloc(1);
void *ptr47 = ics_malloc(1);
void *ptr48 = ics_malloc(1);
void *ptr49 = ics_malloc(1);
void *ptr50 = ics_malloc(1);
void *ptr51 = ics_malloc(1);
void *ptr52 = ics_malloc(1);
void *ptr53 = ics_malloc(1);
void *ptr54 = ics_malloc(1);
void *ptr55 = ics_malloc(1);
void *ptr56 = ics_malloc(1);
void *ptr57 = ics_malloc(1);
void *ptr58 = ics_malloc(1);
void *ptr59 = ics_malloc(1);
void *ptr60 = ics_malloc(1);
void *ptr61 = ics_malloc(1);
void *ptr62 = ics_malloc(1);
void *ptr63 = ics_malloc(1);
void *ptr64 = ics_malloc(1);
void *ptr65 = ics_malloc(1);
void *ptr66 = ics_malloc(1);
void *ptr67 = ics_malloc(1);
void *ptr68 = ics_malloc(1);
void *ptr69 = ics_malloc(1);
void *ptr70 = ics_malloc(1);
void *ptr71 = ics_malloc(1);
void *ptr72 = ics_malloc(1);
void *ptr73 = ics_malloc(1);
void *ptr74 = ics_malloc(1);
void *ptr75 = ics_malloc(1);
void *ptr76 = ics_malloc(1);
void *ptr77 = ics_malloc(1);
void *ptr78 = ics_malloc(1);
void *ptr79 = ics_malloc(1);
void *ptr80 = ics_malloc(1);
void *ptr81 = ics_malloc(1);
void *ptr82 = ics_malloc(1);
void *ptr83 = ics_malloc(1);
void *ptr84 = ics_malloc(1);
void *ptr85 = ics_malloc(1);
void *ptr86 = ics_malloc(1);
void *ptr87 = ics_malloc(1);
void *ptr88 = ics_malloc(1);
void *ptr89 = ics_malloc(1);
void *ptr90 = ics_malloc(1);
void *ptr91 = ics_malloc(1);
void *ptr92 = ics_malloc(1);
void *ptr93 = ics_malloc(1);
void *ptr94 = ics_malloc(1);
void *ptr95 = ics_malloc(1);
void *ptr96 = ics_malloc(1);
void *ptr97 = ics_malloc(1);
void *ptr98 = ics_malloc(1);
void *ptr99 = ics_malloc(1);
void *ptr100 = ics_malloc(1);
void *ptr101 = ics_malloc(1);
void *ptr102 = ics_malloc(1);
void *ptr103 = ics_malloc(1);
void *ptr104 = ics_malloc(1);
void *ptr105 = ics_malloc(1);
void *ptr106 = ics_malloc(1);
void *ptr107 = ics_malloc(1);
void *ptr108 = ics_malloc(1);
void *ptr109 = ics_malloc(1);
void *ptr110 = ics_malloc(1);
void *ptr111 = ics_malloc(1);
void *ptr112 = ics_malloc(1);
void *ptr113 = ics_malloc(1);
void *ptr114 = ics_malloc(1);
void *ptr115 = ics_malloc(1);
void *ptr116 = ics_malloc(1);
void *ptr117 = ics_malloc(1);
void *ptr118 = ics_malloc(1);
void *ptr119 = ics_malloc(1);
void *ptr120 = ics_malloc(1);
void *ptr121 = ics_malloc(1);
void *ptr122 = ics_malloc(1);
void *ptr123 = ics_malloc(1);
void *ptr124 = ics_malloc(1);
void *ptr125 = ics_malloc(1);
void *ptr126 = ics_malloc(1);
ics_free(ptr126);
ics_free(ptr125);
ics_free(ptr124);
ics_free(ptr123);
ics_free(ptr122);
ics_free(ptr121);
ics_free(ptr120);
ics_free(ptr119);
ics_free(ptr118);
ics_free(ptr117);
ics_free(ptr116);
ics_free(ptr115);
ics_free(ptr114);
ics_free(ptr113);
ics_free(ptr112);
ics_free(ptr111);
ics_free(ptr110);
ics_free(ptr109);
ics_free(ptr108);
ics_free(ptr107);
ics_free(ptr106);
ics_free(ptr105);
ics_free(ptr104);
ics_free(ptr103);
ics_free(ptr102);
ics_free(ptr101);
ics_free(ptr100);
ics_free(ptr99);
ics_free(ptr98);
ics_free(ptr97);
ics_free(ptr96);
ics_free(ptr95);
ics_free(ptr94);
ics_free(ptr93);
ics_free(ptr92);
ics_free(ptr91);
ics_free(ptr90);
ics_free(ptr89);
ics_free(ptr88);
ics_free(ptr87);
ics_free(ptr86);
ics_free(ptr85);
ics_free(ptr84);
ics_free(ptr83);
ics_free(ptr82);
ics_free(ptr81);
ics_free(ptr80);
ics_free(ptr79);
ics_free(ptr78);
ics_free(ptr77);
ics_free(ptr76);
ics_free(ptr75);
ics_free(ptr74);
ics_free(ptr73);
ics_free(ptr72);
ics_free(ptr71);
ics_free(ptr70);
ics_free(ptr69);
ics_free(ptr68);
ics_free(ptr67);
ics_free(ptr66);
ics_free(ptr65);
ics_free(ptr64);
ics_free(ptr63);
ics_free(ptr62);
ics_free(ptr61);
ics_free(ptr60);
ics_free(ptr59);
ics_free(ptr58);
ics_free(ptr57);
ics_free(ptr56);
ics_free(ptr55);
ics_free(ptr54);
ics_free(ptr53);
ics_free(ptr52);
ics_free(ptr51);
ics_free(ptr50);
ics_free(ptr49);
ics_free(ptr48);
ics_free(ptr47);
ics_free(ptr46);
ics_free(ptr45);
ics_free(ptr44);
ics_free(ptr43);
ics_free(ptr42);
ics_free(ptr41);
ics_free(ptr40);
ics_free(ptr39);
ics_free(ptr38);
ics_free(ptr37);
ics_free(ptr36);
ics_free(ptr35);
ics_free(ptr34);
ics_free(ptr33);
ics_free(ptr32);
ics_free(ptr31);
ics_free(ptr30);
ics_free(ptr29);
ics_free(ptr28);
ics_free(ptr27);
ics_free(ptr26);
ics_free(ptr25);
ics_free(ptr24);
ics_free(ptr23);
ics_free(ptr22);
ics_free(ptr21);
ics_free(ptr20);
ics_free(ptr19);
ics_free(ptr18);
ics_free(ptr17);
ics_free(ptr16);
ics_free(ptr15);
ics_free(ptr14);
ics_free(ptr13);
ics_free(ptr12);
ics_free(ptr11);
ics_free(ptr10);
ics_free(ptr9);
ics_free(ptr8);
ics_free(ptr7);
ics_free(ptr6);
ics_free(ptr5);
ics_free(ptr4);
ics_free(ptr3);
ics_free(ptr2);
ics_free(ptr1);
ics_free(ptr0);

  
  ics_mem_fini();

  return EXIT_SUCCESS;
}
