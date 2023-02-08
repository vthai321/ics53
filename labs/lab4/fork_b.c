#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t pid;
  int x = 1000, i;

  pid = fork();
  if (pid == 0) {
    /*this is the child*/
    //sleep(3);
    for (i = 0; i <= x; i++) {
      printf("child : %d\n", i);
    }
    exit(0);
  }

  /* parent code */
  for (i = x; i >= 0; i--) {
    //sleep(2);
    printf("parent : %d\n", i);
  }
  exit(0);

  // expected output for parent: count down from 1000
  // expected output for child: count up to 1000

  // actual: the output is not neatly separated into 2 blocks, but is instead intertwining statements and smaller blocks
  // from parent and child 
  // sometimes it alternates, sometimes it's a block 

}
