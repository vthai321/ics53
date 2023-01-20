#include "hw1.h"

// You may define any helper functions you want. Place them in helpers.c/.h
int _53wgrep(int argc, char** arg_str) 
{
    //directly grab arguments needed from argv
    //use argc for # of arguments
      
    // required argument flags
    // n = 0, l = 1, h = 2
    int argReq = -1;


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
    else if(strcmp(arg_str[1], "-l") == 0)
    {
        argReq = 1;
        if(argumentCounter(argReq, argc) == 1)
        {
            fprintf(stderr, USAGE);
            return 1;
        }
        int lhParseResult = lhParser(argc, arg_str, &argI, &argS, &argC, &fg, &bg);
        if(lhParseResult == 1)
        {
            
            fprintf(stderr, USAGE);
            return 1; // we had an error code!! 
        }
    }
    else if(strcmp(arg_str[1], "-h") == 0)
    {
        argReq = 2;
        if(argumentCounter(argReq, argc) == 1)
        {
            fprintf(stderr, USAGE);
            return 1;
        }
        int lhParseResult = lhParser(argc, arg_str, &argI, &argS, &argC, &fg, &bg);
        if(lhParseResult == 1)
        {
            fprintf(stderr, USAGE);
            return 1; // we had an error code!! 
        }
    }
    else
    {
        fprintf(stderr, USAGE);
        return 1; // we had an error code!!
    }

    int exitStatus = 0;
    if(argReq == 0)
    {
        // -n, call the -n function
        exitStatus = nCommand();
        return exitStatus;
    }

    char* word = arg_str[2];
    
    if(argReq == 1)
    {
        exitStatus = lCommand(argI, argS, word);
        return exitStatus;
    }
    else if(argReq == 2)
    {
        // -h

        //write a -h function taking in relevant flags and values
        exitStatus = hCommand(argI, argS, argC, fg, bg, word);
        return exitStatus;

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
