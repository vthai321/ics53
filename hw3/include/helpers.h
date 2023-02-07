#include "linkedList.h"
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

