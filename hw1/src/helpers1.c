// Declare all helper functions for hw1 in this file
#include "helpers1.h"

int lhParser(int argc, char** arg_str, int* argI, int* argS, int* argC, int* fg, int* bg)
{         
    for(int i = 3; i < argc; ++i)
    {
        if(strcmp(arg_str[i], "-I") == 0 && *argI == 0) 
        {
            *argI = 1;
        }
        else if(strcmp(arg_str[i], "-S") == 0 && *argS == 0) 
        {
            *argS = 1;
        }
        else if(strcmp(arg_str[i], "-C") == 0 && *argC == 0) 
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
            i += 2; //to account for also processing the fg and bg 
            
        }
        else
        {
            return 1; // invalid optional argument (duplicates, invalid values)
        }
    }
    return 0; // success
}



int lCommand(int argI, int argS, char* word)
{
    int lineNumber = 1;
    int wordPos = 1; // note: increments by word, NOT by char
    int interestWordPos = 1;
    int matchOcc = 0;
    int exitStatus = 2;
    int iterateWord = 0; // iterates  word, up to length of word
    int correctSoFar = 0;
    int matchesWord = 1; // 1 up until first differing char

    //flags
    int notWhitespace = 0;
    int charFlag = 0;

    // for -S
    int numChars = 0;

    if(argI)
    {
        //make the word lowercase
        size_t wordLen = strlen(word);
        for(size_t i = 0; i < wordLen; ++i)
        {
            if(isalpha(word[i]))
            {
                word[i] = (char)tolower(word[i]);
            }
        }
    }

    // reuse algorithms from -n
    while(1)
    {
        int currentChar = getchar(); //ascii value
        ++numChars; 
        
        if(feof(stdin))
        {
            //add some mechanism to check if the last char belongs to a word we look for
            --numChars; // i think EOF is not a char
            if(notWhitespace)
            {
                // now compare the word
                 if(matchesWord) //equal
                {
                    ++matchOcc;
                    // keep track of line num and pos, print it out
                    fprintf(stdout, "%d:%d\n", lineNumber, interestWordPos);
                    iterateWord = 0;
                }
                
            }
            break; // end of file reached
        }
        
        if(isspace(currentChar) == 0) // begin or middle of word
        {
            if(notWhitespace == 0)
            {
                //mark begining of word by setting interestWordPos value
                interestWordPos = wordPos;
            }

            if(argI)
            {
                if(isalpha(currentChar))
                {
                    currentChar = (char)tolower(currentChar);
                }
            }

            if(currentChar != word[iterateWord])
            {
                matchesWord = 0;
            }
            else
            {
                correctSoFar++;
            }
            
            notWhitespace = 1;
            ++iterateWord;

        }
        else if(isspace(currentChar) != 0 && notWhitespace || currentChar == '\n') //encountered whitespace at end of a word or a newline char
        {
            notWhitespace = 0;

            // now compare the word
            if(matchesWord == 1 && correctSoFar == strlen(word)) //equal
            {
                ++matchOcc;
                // keep track of line num and pos, print it out
                fprintf(stdout, "%d:%d\n", lineNumber, interestWordPos);
            }
            
            interestWordPos = wordPos;
            matchesWord = 1;
            iterateWord = 0;
            correctSoFar = 0;
            
            if(currentChar == '\n')
            {
                lineNumber++;
                wordPos = 1;
            }
            else
            {
                ++wordPos;
            }
            
        }
    }

    if(matchOcc > 0 && argS != 1)
    {
        exitStatus = 0;
        fprintf(stderr, "%d\n", matchOcc);
    }
    else if(argS)
    {
        if(matchOcc > 0)
        {
            exitStatus = 0;
            fprintf(stderr, "%d %d %d\n", matchOcc, numChars, lineNumber);
        }
    }

    //printf("DEBUG: number of occurances: %d\n", matchOcc);
    return exitStatus;
}

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
int hCommand(int argI, int argS, int argC, int fg, int bg, char* word)
{
    int matchOcc = 0;
    int exitStatus = 2;
    int iterateWord = 0; // iterates  word, up to length of word
    int correctSoFar = 0; 
    //int matchesWord = 1; // 1 up until first differing char; possibly not needed
    int lineNumber = 1;
    char wordBuffer[sizeof(word)] = ""; // if strlen + 1 equals sizeof(word), immediately proceed to print. Null terminator alr included?
    char wordCheck[sizeof(word)] = ""; // differs only if -I flag is used.
    strncpy(wordCheck, word, sizeof(word));

    //flags
    int notWhitespace = 0;
    int charFlag = 0;

    // for -S
    int numChars = 0;

    //fg and bg as str
    char fgStr[3]; 
    char bgStr[4];
    if(argC)
    {
        sprintf(fgStr, "%d", fg);
        sprintf(bgStr, "%dm", bg);
    }
    else
    {
        sprintf(fgStr, "37");
        sprintf(bgStr, "41m");
    }
    //default escape code
    char defaultEscape[] = "\x1B[39;49m";

    if(argI)
    {
        //make the wordCheck lowercase
        size_t wordLen = strlen(word);
        for(size_t i = 0; i < wordLen; ++i)
        {
                wordCheck[i] = (char)tolower(word[i]);
        }
    }

    // reuse algorithms from -l
    while(1)
    {      
        int currentChar = getchar(); //ascii value
        int currentCharCheck = currentChar; //will differ only if -I flag is used
        ++numChars; 
        if(currentChar == '\n')
        {
            ++lineNumber;
        }
        if(argI)
        {
            if(isalpha(currentChar))
            {
                currentCharCheck = tolower(currentChar);
            }
        }

        if(feof(stdin))
        {
            //add some mechanism to check if the last char belongs to a word we look for
            --numChars; // i think EOF is not a char
            if(notWhitespace)
            {
                // now compare the word with strcmp
                int debug = strcmp(wordCheck, wordBuffer);
                if(correctSoFar == strlen(wordCheck)) //equal
                {
                    ++matchOcc;
                    //proceed to print with highlight
                    char escapeCode[11] = "\x1B[";
                    strncat(escapeCode, fgStr, 3);
                    strncat(escapeCode, ";", 2);
                    strncat(escapeCode, bgStr, 4);

                    fprintf(stdout, "%s", escapeCode);
                    fprintf(stdout, "%s", wordBuffer);
                    //reset escape code
                    fprintf(stdout, "%s", defaultEscape);
                }
                else
                {
                    //print with no highlight
                    fprintf(stdout, "%s", wordBuffer);
                    memset(wordBuffer, 0, strlen(wordBuffer));
                    iterateWord = 0;
                }
            }
            break; // end of file reached
        }
        
        if(currentCharCheck == wordCheck[iterateWord]) // if we encounter a matching first char (and subsequent matches)
        {
            notWhitespace = 1;
            //check the bounds and add to buffer
            if(sizeof(wordBuffer) + 1 == sizeof(wordCheck))
            {
                // kill switch, proceed to print without highlight
                fprintf(stdout, "%s", wordBuffer);
                memset(wordBuffer, 0, strlen(wordBuffer));
                iterateWord = 0;
                correctSoFar = 0;
            }
            else
            {
                wordBuffer[iterateWord] = currentChar;
            }
            ++iterateWord;
            ++correctSoFar;
            
        }
        else if(isspace(currentCharCheck) != 0 && notWhitespace || currentCharCheck == '\n' && notWhitespace) //encountered whitespace at end of a word or a newline char
        {
            notWhitespace = 0;

            // initiates sequence to compare the word

            if(correctSoFar == strlen(word)) //equal
            {
                ++matchOcc;
                char escapeCode[11] = "\x1B[";
                strncat(escapeCode, fgStr, 3);
                strncat(escapeCode, ";", 2);
                strncat(escapeCode, bgStr, 4);

                fprintf(stdout, "%s", escapeCode);
                fprintf(stdout, "%s", wordBuffer);
                //reset escape code
                fprintf(stdout, "%s", defaultEscape);
            }
            else
            {
                // print what you have
                fprintf(stdout, "%s", wordBuffer);
            }
            fprintf(stdout, "%c", currentChar);
            
            iterateWord = 0;
            correctSoFar = 0;
            memset(wordBuffer, 0, strlen(wordBuffer));

        }
        else if(currentCharCheck != wordCheck[iterateWord] && correctSoFar > 0) 
        {
            //you lost, print what you have (no highlight)
            //printf("What we have at lost: %s ", wordBuffer);
            fprintf(stdout, "%s", wordBuffer);
            memset(wordBuffer, 0, strlen(wordBuffer));
            iterateWord = 0;
            correctSoFar = 0;

            fprintf(stdout, "%c", (char)currentChar);
        }
        else
        {
            // proceed as normal. print the char to stdout
            fprintf(stdout, "%c", (char)currentChar);
        }
    }

    if(matchOcc > 0)
    {
        exitStatus = 0;
        if(argS)
        {
            fprintf(stderr, "%d %d %d\n", matchOcc, numChars, lineNumber);
        }
    }

    //printf("DEBUG: number of occurances: %d\n", matchOcc);
    return exitStatus;    
}


/**
Implements the -n command that counts the amount of numbers in a x text
A number is defined as a sequence of numbers

Read char by char (or token by token?); read line as str 
using char *fgets(char *str, int n, FILE *stream) (loop ends when null pointer returns)
Check if a sequence of chars is all digits or perhaps if the token does not contain special chars or letters 

*/
int nCommand()
{
    int numberFlag = 0; // 1 if we encounter a number, 0 if we encounter any other, etc.
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
            fprintf(stdout, "%c", currentCharCasted);
        }
        else if(numberFlag) //encountered a number but encountered something not a digit
        {
            numberFlag = 0;
            fprintf(stdout, "\n");
        }
    }
    return exitStatus;
}

int argumentCounter(int flag, int argc)
{
    int maxArgs = 0;

    if(flag == 1) // -l
    {
        maxArgs = 5;

    }
    else if(flag == 2) // -h
    {
        maxArgs = 8;
    }
    else
    {
        fprintf(stderr, USAGE);
        return 1; // not -l or -h
    }

    if(argc > maxArgs)
    {
        fprintf(stderr, USAGE);
        return 1; // too many arguments
    }
    return 0; // everything works out.
}
