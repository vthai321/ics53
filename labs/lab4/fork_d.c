#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

void strLower(char *str) {
  int i;
  for (i = 0; str[i]; i++) {
    str[i] = tolower(str[i]);
  }
}

int main() {
  pid_t pid;
  int *status = NULL;
  char str[] = "ABCDEFG";

  pid = fork();
  if (pid == 0) {
    /*this is the child*/
    printf("child : str= %s\n", str);
    strLower(str);
    printf("child : str= %s\n", str);
    exit(0);
  }

  /* parent code */
  // note it's possible for the code to get here first (parent gets operated on first)
  // then with the wait call, it would then be the child's turn to get worked on 
  printf("parent : str= %s\n", str);

  while (wait(status) > 0) {
    printf("After child terminated\n");
    printf("parent : str= %s\n", str);
  }

  // child before: ABCDEFG 
  // child after: abcdefg

  // parent before: ABCDEFG
  // parent after ABCDEFG

  // parent and child do not share the same memory

  exit(0);
}
