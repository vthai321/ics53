// Declare all helper functions for hw1 in this file
# ifndef HW1_HELPERS
# define HW1_HELPERS

/**
If -l or -h is supplied, uses a for-loop to parse through and process any optional arguments present

*/
int lhParser(int flag, int argc, char** arg_str, int* argI, int* argS, int* argC, int* fg, int* bg, char** word); 

/**
Implements the -l command that counts words and has varying actions depending on flag.

*/
int lCommand(int argI, int argS);

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


# endif