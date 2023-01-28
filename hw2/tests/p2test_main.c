#include <stdio.h>
#include <stdlib.h>
#include <string.h> //string.h functions are not allowed in your assignment

#include "helpers2.h"
#include "hw2.h"
#include "linkedlist.h"

int main()
{
    list_t* myList = CreateList(ModFileABC_Comparator, ModFile_Printer, ModFile_Deleter);
    FILE *fp = fopen("/root/ics53/hw2/rsrc/tell-a-vision-log.txt", "r");
    ProcessModFile(fp, myList, 'a');
    fclose(fp);
    return 0;
}
