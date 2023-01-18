#include "hw1.h"
#include <stdio.h>
#include <string.h>
// You may define any helper functions you want. Place them in helpers.c/.h
int _53wgrep(int argc, char** arg_str) 
{
    //directly grab arguments needed from argv
    //use argc for # of arguments

    // first determine which essential argument we have (use flags?); otherwise, return error 
    // ask if we need to worry about accidental junk characters

    //WARNING: CANNOT USE == OPERATOR TO COMPARE STRINGS IN C
    
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
        return 1; // too little arguments
    }
    else if(strcmp(arg_str[1], "-n") == 0)
    {
        if(argc > 2)
        {
            return 1; // too many arguments (n has no option)
        }
        argReq = 0; // flag for n set true
    }
    else
    {
        int lhParseResult = lhParser(argReq, argc, arg_str, &argI, &argS, &argC, &fg, &bg, &word);
        if(lhParseResult = 1)
        {
            return 1; // we had an error code!!!
        }
    }

    // plan is to implement each method of counting separately in its own function. 
    // Use if statements to decide which one to call

    if(argReq == 0)
    {
        // -n, call the -n function
    

    }
    else if(argReq == 1)
    {
        // -l

        //write a -l function that takes in relevant flags to determine course of action

    }
    else if(argReq = 2)
    {
        // -h

        //write a -h function taking in relevant flags and values
    }

    // the functions will return the appropriate exit status here.

    return 0;


}

// Main program
int main(int argc, char *argv[]) 
{
    // Insert your hw1 code here
    int exitCode = _53wgrep(argc, argv);  

    int a = 5;  

    return 0;
}
