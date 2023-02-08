#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t pid;
  int x = 10;

  pid = fork();
  if (pid == 0) {
    /*this is the child*/
    printf("child : x=%d\n", ++x);
    exit(0);
  }

  /* parent code */
  printf("parent : x=%d\n", --x);

  exit(0);

  // expected x for child: 11

  // expected x for parent: 9

  // same as actual

}
