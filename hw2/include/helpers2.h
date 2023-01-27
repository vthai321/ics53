#ifndef HELPERS_2_H
#define HELPERS_2_H

// Declare all helper functions for hw2 in this file

/*
    My own implementation of strcmp using pointers ONLY 
    Does NOT check if string is properly null terminated!
    Return 0 if equal, -1 if string1 < string2, 1 if string1 > string2
*/
int myStrcmp(char* string1, char* string2);

/*
    Counts size (including null terminator)
    
*/
int myStrSize(const char* str);


#endif