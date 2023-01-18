// Declare all helper functions for hw1 in this file
#include "helpers1.h"

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
            int fgColor = atoi(arg_str[i+1]);
            int bgColor = atoi(arg_str[i+2]);
           
            if(fgColor >= 30 && fgColor <= 37)
            {
                // valid fg
                *fg = fgColor;
            }
            else
            {
                fprintf(stderr, USAGE);
                return 1; // invalid fg argument :(
            }

            if(bgColor >= 40 && bgColor <= 47)
            {
                //valid bg
                *bg = bgColor;
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

Only full words count (not substrings); thus, either use tokens or finding words via spaces
Use fscanf?


*/
int lCommand(int argI, int argS)
{

    return 0;
}

/**
Implements the -n command that counts the amount of numbers in a given text
A number is defined as a sequence of numbers

Read char by char (or token by token?); read line as str 
using char *fgets(char *str, int n, FILE *stream) (loop ends when null pointer returns)
Check if a sequence of chars is all digits or perhaps if the token does not contain special chars or letters 

*/
int nCommand()
{
    int numberFlag = 0; // 1 if we encounter a number, 0 if we encounter any other, etc.
    char num[1024] = ""; // constructs the number to print. review buffer with TA!
    int exitStatus = 2; // 0 for success, 2 for no occurance of numbers

    while(1)
    {
        if(feof(stdin))
        {
            break; // end of file reached
        }
       
        int currentChar = getchar(); //ascii value

        if(isdigit(currentChar))
        {
            exitStatus = 0; // not efficient but oh well
            numberFlag = 1;
            char currentCharCasted = (char)currentChar;
            strncat(num, &currentCharCasted, 1); 
        }
        else if(numberFlag) //encountered a number but encountered something not a digit
        {
            numberFlag = 0;
            fprintf(stdout, "%s\n", num);
             num[0] = '\0'; // clear the string
        }
    }
    return exitStatus;
}
