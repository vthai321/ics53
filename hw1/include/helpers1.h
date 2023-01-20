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
int lhParser(int argc, char** arg_str, int* argI, int* argS, int* argC, int* fg, int* bg); 

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

Using the modified algorithm for l, parse char by char through the text and print each char. HOWEVER, if you encounter the same start
char as the target word, start building a string and stop printing char by char. If we follow to the end of the word (whitespace or \n) and it matches, 
then set the special escape codes and print that word to output. If it ends up not matching, we immediately print what we have (no special colors), 
and go back to printing char by char

-I is case insensitive
-S is to count total number of char and lines and occurrances (only if numOcc > 1)


*/
int hCommand(int argI, int argS, int argC, int fg, int bg, char* word);

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

/**
Argument counter to see if amount of arguments is valid 

*/
int argumentCounter(int flag, int argc);



# endif