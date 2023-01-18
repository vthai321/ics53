// Useage statement
#define USAGE "usage:\n\
53wgrep -n\n\
53wgrep -l WORD [-I] [-S]\n\
53wgrep -h WORD [-I] [-S] [-C FG BG]\n"

#include <stdio.h>
#include <stdlib.h>
#include "helpers1.h"
#include <stdio.h>
#include <string.h>

// might not need these statements

#ifndef hw1
#define hw1

/** 
    synopsis: scan input (should do 1 byte at a time) of a file and find occurrances of numbers / words

    Required Options: One at a time, beginning of command

    -n: 
    >print all numberical values from STDIN to STDOUT, 1 value per line (sequence of numerals)

    -l WORD: 
    >WORD represents the word to search. WORD is case sensitive, if word is found, line number (starts with 1) and word position (starts with 1),
    i.e. like a coordinate system, printed to STDOUT as linenum:wordnum\n

    >only full words (tokens separated by spaces), no substrings

    >if # words > 1, print amount of words to STDERR

    -h WORD: 
    
    >like l word but this time we highlight the words found. This is done by echoing the input from STDIN to STDOUT, 
    with the highlighting being the main change.

    >Default color: White(37) FG, Red(41) BF

    >Full Words only

    Optional Options: refer to document to see which required options to pair with

    -S

    >Use only with l and h. prints total number of lines (# newline chars) and number of chars to STDERR 
    plus number of occurances (akin to l)

    -I

    >Use with l and h. Word matches become case insensitive (check everything in lowercase?)

    -C FG BG

    >Use with C to set FG and BG color 

    >Valid FG (30-37), Valid BG (40-47)

    Exit Status:

    >0 = success, 
    >1 = invalid program args. Useage statement prints to STDERR. 
    >2 = No occurance found within text

    >Standard escape codes are prefixed with an escape char.
    */ 

    /**
    Parameters: 
    >int argc: amount of arguments (beware: includes name of function!)
    >char* arg_str: the string of arguments that determine function operation
    
    Working with commands:
    >Parse the arg str and find the appropriate commands (from hw0, I think C accounts for spaces between commands
    >If you read -C, you'll also need to read the next 2 commands (if present; otherwise, it's an error)
    >args start at index 1

    Return Type: Int (exit status)
    */

int _53wgrep(int argc, char** arg_str);

#endif
