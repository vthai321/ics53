#include "hw1.h"

// You may define any helper functions you want. Place them in helpers.c/.h
int _53wgrep(int argc, char** arg_str) 
{
    //directly grab arguments needed from argv
    //use argc for # of arguments
    
    // required argument flags
    // n = 0, l = 1, h = 2
    int argReq = -1;

    // Word to check for 
    char* word = "";

    // -I and -S flags
    int argI = 0;
    int argS = 0;

    // -C flag
    int argC = 0;
    int fg = 0;
    int bg = 0;
    
    if(argc < 2)
    {
        fprintf(stderr, USAGE);
        return 1; // too little arguments
    }
    else if(strcmp(arg_str[1], "-n") == 0)
    {
        if(argc > 2)
        {
            fprintf(stderr, USAGE);
            return 1; // too many arguments (n has no option)
        }
        argReq = 0; // flag for n set true
    }
    else
    {
        int lhParseResult = lhParser(argReq, argc, arg_str, &argI, &argS, &argC, &fg, &bg, &word);
        if(lhParseResult = 1)
        {
            
            fprintf(stderr, USAGE);
            return 1; // we had an error code!! 
        }
    }

    int exitStatus = 0;
    if(argReq == 0)
    {
        // -n, call the -n function
        exitStatus = nCommand();
        return exitStatus;
    }
    else if(argReq == 1)
    {
        // -l

        //write a -l function that takes in relevant flags to determine course of action

        // read by token

    }
    else if(argReq = 2)
    {
        // -h

        //write a -h function taking in relevant flags and values

        // read by token
    }

    // the functions will return the appropriate exit status here.

    return exitStatus;
}

// Main program
int main(int argc, char *argv[]) 
{
    // Insert your hw1 code here
    int exitCode = _53wgrep(argc, argv);  
    return exitCode;
}
