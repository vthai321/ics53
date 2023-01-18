// Vincent Quan Thai
// vqthai

#include <stdio.h>
#include <string.h>

int main()
{
    char netid[10] = "vqthai";
    int courseNum = 53;
    printf("Hello, %s! Welcome to ICS%d!\n", netid, courseNum);
    int netidLen = strlen(netid);
    
    for(int i = netidLen; i >= 0; --i)
    {
        printf("%d\n", i);
    }

    return 0;
}