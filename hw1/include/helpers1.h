// Declare all helper functions for hw1 in this file
# ifndef HW1_HELPERS
# define HW1_HELPERS

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "hw1.h"

/**
If -l or -h is supplied, uses a for-loop to parse through and process any optional arguments present

*/
int lhParser(int flag, int argc, char** arg_str, int* argI, int* argS, int* argC, int* fg, int* bg, char** word); 

/**
Implements the -l command that counts words and has varying actions depending on flag.
Implementation can be similar to nCommand. Read one char at a time and use a flag to determine existance of a word
A word happens if charFlag = 0 and isWhitespace = 0, up to charFlag = 1 and isWhitespace = 1

Special cases:
Empty file: handle it using if statement
Word at begnining of file: determine initial value of charFlag using the very first char
Word at end of file: if statement: If charFlag = 1 and we reach eof, go ahead and increment wordCount by 1 before exiting
Double space: just proceed with the loop

Optional arguments:
-I: case insenstive
-S: print total number of chars, total number of lines to stderr, as well as number of occurances of word (if >=1 )

*/
int lCommand(int argI, int argS, char* word);

/**
Implements the -h command that highlights words and has varying actions depending on flag
fg and bg are used depending on value of argC

*/
int hCommand(int argI, int argS, int argC, int fg, int bg);

/**
Implements the -n command that counts the amount of numbers in a given text
A number is defined as a token consisting completely of digits

Read char by char (or token by token?); read line as str using char *fgets(char *str, int n, FILE *stream)
Check if a sequence of chars is all digits or perhaps if the token does not contain special chars or letters 

*/
int nCommand();

/**
Compares word according to our special rules
Also prints to stdout

*/

void compareWord(char* wordConstruct, char* word, int argI, int* matchOcc, int wordPos, int lineNumber);


# endif