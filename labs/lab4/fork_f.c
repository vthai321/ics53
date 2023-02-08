#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  if (fork() == 0) 
  {
    /*this is the child*/
    while (1) 
    {
        printf("Running child : ID=%d\n", getpid());
        sleep(10);
    }

  }

  /* parent code */
    printf("Terminating child : PID=%d\n", getpid());
    exit(0);

}
