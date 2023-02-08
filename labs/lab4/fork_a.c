#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() 
{
  pid_t pid;

  printf("About to call fork() one time\n");

  pid = fork();

  if (pid == 0) {
    printf("child: After the call to fork()\n");
  } else {
    sleep(1);
    printf("parent: After the call to fork()\n");
  }

  // i expect 1 call
  // actual: 2 calls (one for the parent, one for the child)

  // sleep(): causes calling thead to sleep for specified number of seconds (notice the delay!)

  exit(0);
}
