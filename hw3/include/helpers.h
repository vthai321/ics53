#include "linkedList.h"
#include "icssh.h"
#include <signal.h>
#include <unistd.h>

// flag for terminated child (bg) (global) (should change to volatile sig_atomic t)
//extern int terminatedChild;
extern volatile sig_atomic_t terminatedChild;

// A header file for helpers.c
// Declare any additional functions in this file

/*
    Helper function to sort items based on time
    Parameters: bGentryOne and bGentryTwo, 2 const void* pointers that will be casted to gentry_t type and compared
    Return Values: 0 if equal, -1 if one < two, 1 if two > one
*/
int bGentryTime_Comparator(void* bGentryOne, void* bGentryTwo);

/*
    Handles SIGCHLD Signal
    Used in conjunction with signal() via function pointer
    Parameters: int sig, the signal to process
    Return Values: none
*/
void sigchldHandler(int sig);

/*
Iterates thru the bgentry list and deletes terminated processes (using a counter and removebyindex)
A process (node) is terminated if 
Also prints BG_TERM for the terminated nodes
Parameters: List_t* list, the list to modify
Return Values: none
*/
void terminateDeleter(List_t* list, int terminatePID);

/*
Handles the SIGUSR2 signal (custom signal); if sent to shell process, print to stdout with specs in hw doc
Parameters: int sig, the signal to process 
Return Values: None
*/
void sigusr2Handler(int sig);

/*
A modified version of terminateDeleter.
Iterates through the bgentry list, prints the necessary info w/ BG_TERM, and frees the bgentry struct as well as the job struct inside
Then at the end calls deleteList to delete the data structure
Parameters: list_t* list, the list to destroy
Return Values: none
*/
void terminateAll(List_t* list);

/*
Handles the redirection portion of the shell. 
Parameters: job_info* job, the job struct where we get relevent data, char* line, the buffer for the command
Return values: 0 on success, -1 on failure
*/
int shellRedirection(job_info* job, char* line, pid_t *wait_result, int *exit_status, List_t *bgentry_List); 

/*
Handles calling a command without piping or redirection.
Parameters: char* line, pid_t pid, pid_t* wait_result, int* exec_result, int* exit_status job_info* job
Return Values: none
*/
void execute(char* line, pid_t pid, pid_t* wait_result, int* exec_result, int* exit_status, job_info* job);

/*
Contains all the code for piping, so that the shell looks neater
Parameters: job_info* job
Return Values: An int for return status. 0 for Success
*/
int doPipe(job_info* job, int* pid);

