// Declare all helper functions for hw1 in this file
#include "helpers1.h"
#include <stdlib.h>
#include <string.h>

int lhParser(int flag, int argc, char** arg_str, int* argI, int* argS, int* argC, int* fg, int* bg, char** word)
{
    int maxArgs = 0;

    if(flag == 1)
    {
        maxArgs = 5;

    }
    else if(flag == 2)
    {
        maxArgs = 8;
    }
    else
    {
        return 1; // not -l or -h
    }

    if(argc > maxArgs)
    {
        return 1; // too many arguments
    }
    *word = arg_str[2];
        
    for(int i = 3; i < argc; ++i)
    {

        if(strcmp(arg_str[i], "-I") == 0 && argI == 0) 
        {
            *argI = 1;
        }
        else if(strcmp(arg_str[i], "-S") == 0 && argS == 0) 
        {
            *argS = 1;
        }
        else if(strcmp(arg_str[i], "-C") == 0 && argC == 0) 
        {
            *argC = 1;
            // assume we can retrieve FG and BG
            if(*arg_str[i+1] >= 30 && *arg_str[i+1] <= 37)
            {
                // valid fg
                *fg = atoi(arg_str[i+1]);
            }
            else
            {
                return 1; // invalid fg argument :(
            }

            if(*arg_str[i+2] >= 40 && *arg_str[i+2] <= 47)
            {
                //valid bg
                *bg = atoi(arg_str[i+2]);
            }
            else
            {
                return 1; // invalid bg argument
            }
            i = argc; // end the loop 
            
        }
        else
        {
            return 1; // invalid optional argument (duplicates, invalid values)
        }
    }
    return 0; // success
}

/**
Implements the -l command that counts words and has varying actions depending on flag.

Make prominent use of the "<" operator to read from a file into STDIN. Use STDIN and process
the information there using various methods

*/
int lCommand(int argI, int argS)
{
    
    return 0;
}
