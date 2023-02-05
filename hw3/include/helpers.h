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
