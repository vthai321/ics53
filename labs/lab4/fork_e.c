#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t pid;
  char c;

  FILE *fd = fopen("nums.txt", "r");
  if (fd == NULL) {
    printf("Error: Could not open file");
    exit(0);
  }

  pid = fork();
  if (pid == 0) {
    /*this is the child*/
    while ((c = getc(fd)) != 0xff) {
      printf("child: %c\n", c);
    }
    exit(0);
  }

  /* parent code */
  while ((c = getc(fd)) != 0xff) {
    printf("parent: %c\n", c);
  }

  exit(0);

  // i expect the output to be the same as what's in the file

  // it got stuck, I think because sharing the same stream led to both processes unable to reach the end

}
