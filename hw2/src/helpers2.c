// Define all helper functions for hw2 in this file
#include "helpers2.h"

int myStrcmp(char* string1, char* string2)
{
    // use a while loop
    int string1Offset = 0;
    int string2Offset = 0;
    int comparisonStatus = 0;
    char currentString1 = ' ';
    char currentString2 = ' ';

    do
    {
        currentString1 = *(string1 + string1Offset);
        currentString2 = *(string2 + string2Offset);
        
        if(currentString1 < currentString2)
        {
            comparisonStatus = -1;
            return comparisonStatus;
        }
        else if(currentString1 > currentString2)
        {
            comparisonStatus = 1;
            return comparisonStatus;
        }

        ++string1Offset;
        ++string2Offset;

    }
    while(currentString1 != '\0' || currentString2 != '\0');
    return comparisonStatus;
}
