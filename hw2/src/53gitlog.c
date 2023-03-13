#include "constants2.h"
#include "helpers2.h"
#include "hw2.h"

#define _XOPEN_SOURCE
#include <time.h>

int main(int argc, char* argv[]) {
	int D_flag = 0;
	int A_flag = 0;
    int NUM_arg = 0;
    int LEVEL_arg = 0;
    char ORDER_arg = 'n';   // default no order
    char* DATE_arg = NULL;
    char* OUTFILE = NULL;
    char* INFILE = NULL;

    // additional flags
    int I_flag = 0;
    int O_flag = 0;
    int a_flag = 0; // note capitalization matter
    int n_flag = 0;


    // Use basic getopt to parse flags and respective arguments
    int option;
    while ((option = getopt(argc, argv, "HD:A:O:I:n:ad" )) >= 0) {
        switch (option) {
            case 'H':
				fprintf(stdout, USAGE_MSG);
				return EXIT_SUCCESS;
            case 'D':
        	    D_flag = 1;
                DATE_arg = optarg;
                break;
            case 'A':
				A_flag = 1;
                LEVEL_arg = atoi(optarg);
                break;
            case 'n':
				n_flag = 1;
                NUM_arg = atoi(optarg);
                break;
            case 'a':
                a_flag = 1;
            // case 'd': // change?
			// 	ORDER_arg = option;
            //     break;
            case 'O':
				O_flag = 1;
                OUTFILE = optarg;
                break;
            case 'I':
				I_flag = 1;
                INFILE = optarg;
                break;
            default:
                fprintf(stderr, USAGE_MSG);
                return EXIT_FAILURE;
        }
    }

    // validate a required option was specified - Does not check for more than 1
    if ( ! (A_flag | D_flag) )
    {
        fprintf(stderr, "ERROR: Search mode was not specified.\n\n" USAGE_MSG);
        return EXIT_FAILURE;
    }
    
    // INSERT YOUR IMPLEMENTATION HERE
    // getopts only stored the arguments and performed basic checks. More error checking is still needed!!!!
    // tar -C ~/ics53 -cvf hw2_vqthai.tar hw2

    if(A_flag == 1 && D_flag == 1)
    {
        // cannot have more than 1 required option;
        fprintf(stderr, USAGE_MSG);
        return EXIT_FAILURE;
    }

    if(NUM_arg < 0)
    {
        return EXIT_FAILURE;
    }

    // dd/mm/yyyy
    // check the date
    // only if D_flag specified
    

    // use it for the part below where we have to insert into a list
    // the code below converts the date given in dd/mm/yyyy into unix time (which is present in logs)

    time_t gitDate; // on standby 
    if(D_flag)
    {
        char* dateCharPointer = DATE_arg;
        
        char* dayString = myStrCpy(dateCharPointer, "/");
        int day = atoi(dayString);
        free(dayString);
        dayString = NULL;
        dateCharPointer += 3;

        char* monthString = myStrCpy(dateCharPointer, "/");
        int month = atoi(monthString);
        free(monthString);
        monthString = NULL;
        dateCharPointer += 3;

        char* yearString = myStrCpy(dateCharPointer, "/");
        int year = atoi(yearString);
        free(yearString);
        monthString = NULL;

        if(day < 1 || day > 31)
        {
            fprintf(stderr, USAGE_MSG);
            return EXIT_FAILURE;
        }
        else if(month < 1 || month > 12)
        {
            fprintf(stderr, USAGE_MSG);
            return EXIT_FAILURE;
        }
        else if(year < 1900 || year > 9999)
        {
            fprintf(stderr, USAGE_MSG);
            return EXIT_FAILURE;
        }

        struct tm myTime;
        time_t timeOfDay;

        myTime.tm_year = year - 1900;
        myTime.tm_mon = month - 1; // month index starts at 0
        myTime.tm_mday = day;
        myTime.tm_hour = 23;
        myTime.tm_min = 59;
        myTime.tm_sec = 59;
        myTime.tm_isdst = -1; //-1 means we don't know if DST is on
        gitDate = mktime(&myTime);

    }

    // check file, if we indicated we're passing one in
    if(I_flag)
    {
        if(INFILE == NULL || myStrcmp(INFILE, "") == 0)
        {
            //fprintf(stderr, USAGE_MSG);
            return 2;
        }
        else if(fopen(INFILE, "r") == NULL)
        {
            return 2;
        }

    }

    if(O_flag)
    {
        // print into outfile instead of stdout
        FILE* fd = fopen(OUTFILE, "w");
        if(fd == NULL) 
        {
            return 2;
        }
        else
        {
            stdout = fd;
        }
    }

    // build the linkedLists
    
    list_t* modFileList = CreateList(ModFileABC_Comparator, ModFile_Printer, ModFile_Deleter);  
    list_t* authorList =  CreateList(ModFileABC_Comparator, ModFile_Printer, ModFile_Deleter);  

    // we will read both author and modFile
    FILE* fp = NULL;
    if(I_flag)
    {
        fp = fopen(INFILE, "r");
        stdin = fp;
    }
    else
    {
        fp = stdin;
    }
    
    /*
    if(A_flag) // changed from: if(A_flag || D_flag)
    {
        /*
        char* buffer = malloc(205);
        long int* timeStampBuffer = malloc(sizeof(long int)); // will most likely replace with time-getting function
        while(fgets(buffer, 205, fp) != NULL) // putting it here will get all the author lines
        {  
            Author* newAuthor = CreateAuthor(buffer, timeStampBuffer);
            if(*timeStampBuffer > gitDate)
            {
                ProcessModFile(fp, modFileList, 'f'); // loop until we read the blank line
            }
            
            if(FindInList(authorList, newAuthor) == NULL)
            {
                InsertInOrder(authorList, newAuthor);
            }
        }
    }
    */

    if(A_flag)
    {
        //todo: fix "Syscall param openat(filename) points to unaddressable byte(s)", which is an error outisde the D or A Flags
    }
    else if(D_flag)
    {
        // involves modFileList
        // plan: go through buffer and call processModFile for every line grabbed. compare with date (will have to use converter function)
        // how to get to the line's date? Use a while loop to get to first comma, then increment ONCE after loop
        // listed in order of appearance by default, otherwise alphabetical order with -a 
        char* buffer = malloc(205);
        if(a_flag)
        {
            while(fgets(buffer, 200, fp) != NULL)
            {
                char* bufferCurrent = buffer;
                while(*bufferCurrent != ',')
                {
                    ++bufferCurrent;
                }
                ++bufferCurrent;

                char* modFileDateStr = myStrCpy(bufferCurrent, ",");
                time_t modFileDate = (time_t) atoll(modFileDateStr);
                if(modFileDate > gitDate)
                {
                    ProcessModFile(fp, modFileList, 'a');
                }
                free(modFileDateStr);
            }
        }
        else 
        {
            while(fgets(buffer, 200, fp) != NULL) 
            {
                char* bufferCurrent = buffer;
                while(*bufferCurrent != ',')
                {
                    ++bufferCurrent;
                }
                ++bufferCurrent;

                char* modFileDateStr = myStrCpy(bufferCurrent, ",");
                time_t modFileDate = (time_t) atoll(modFileDateStr);
                if(modFileDate > gitDate)
                {
                    ProcessModFile(fp, modFileList, 'f');
                }
                free(modFileDateStr);
            }
        }
        free(buffer);
    }

    // print here
    if(D_flag)
    {
        if(n_flag)
        {
            PrintNLinkedList(modFileList, stdout, NUM_arg);
        }
        else
        {
            PrintLinkedList(modFileList, stdout);
        }
    }
    else if(A_flag)
    {
        // don't forget to implement levels
        if(n_flag)
        {
            PrintNLinkedList(authorList, stdout, NUM_arg);
        }
        else
        {
            PrintLinkedList(authorList, stdout);   
        }
    }

    //delete the lists (also frees it)
    DestroyList(&modFileList);
    DestroyList(&authorList);

    return 0;
}
