// Vincent Quan Thai
// vqthai

#include "hw2.h"
#include "helpers2.h"
#include "linkedlist.h"

// Part 0 Function to implement
char* myStrCpy(char* str, char* delimiters) 
{
    char* newStr = (char *) malloc(myStrSize(str));
    if(newStr == NULL)
    {
        return "fail"; // not necessary?
    }
    
    char currentChar = ' ';

    if(newStr == NULL || str == NULL)
    {
        return NULL;
    }

    int parseString = 1; // becomes 0 if we reach null terminator or deliminator
    int offset = 0; // offset for pointer arithmetic
    int inDelimit = 0; // becomes 1 if our char is part of our delimit string
    int delimitBound = myStrSize(delimiters) - 1; // -1 accounts for null terminator
    
    while(parseString)
    {
        
        currentChar = *(str + offset);
        for(int i = 0; i < delimitBound; ++i)
        {
            if(currentChar == *(delimiters + i))
            {
                inDelimit = 1;
                break;
            }
        }
        
        if(currentChar == '\0' || inDelimit == 1) 
        {
            *(newStr + offset) = '\0';
            parseString = 0;
        }
        else
        {
            *(newStr + offset) = currentChar;
        }
        ++offset;

    }
    return newStr;
}

// Part 1 Functions to implement

ModFile* PutModFile(int ins, int dels, char* filename, ModFile* mf) 
{
   //create a new instance of ModFile struct or update a preexisting ones
   
    if(mf == NULL)
    {
        // create a new struct
        ModFile* newModFile = (ModFile*) malloc(sizeof(ModFile));
        newModFile->inserts = ins;
        newModFile->deletes = dels;
        newModFile->filename = filename;
        return newModFile;
    }
    else
    {
        // update preexisting ModFile
        // check if filename in mf matches filename argument first
        if(myStrcmp(mf->filename, filename) != 0 || filename == NULL || ins < 0 || dels < 0)
        {
            return NULL;
        }
        else
        {
            // filename matches ModFile's filename
            mf->inserts = ins;
            mf->deletes = dels;
            ModFile* validReturn = mf;
            return validReturn;
        }
    }
    return NULL; // kept just in case the function somehow makes it here
}

int ModFileABC_Comparator(const void* file1, const void* file2) 
{
    // compare the filenames
    // use pointer arithmetic?
    return myStrcmp( ((ModFile*)file1)->filename, ((ModFile*)file2)->filename ); 
}

int ModFileTotal_Comparator(const void* file1, const void* file2) 
{
    // compare number of ins/del between the two ModFiles
    // 0 if ins + del equal, -1 if file1 < file2, 1 if file1 > file2
    int file1Sum = ((ModFile*)file1)->inserts + ((ModFile*)file2)->deletes;
    int file2Sum = ((ModFile*)file2)->inserts + ((ModFile*)file2)->deletes;

    if(file1Sum == file2Sum)
    {
        return 0;
    }
    else if(file1Sum > file2Sum)
    {
        return 1;
    }
    else
    {
        return -1;
    }
    return 69; // should never be 69
}

// Part 2 Functions to implement
void ModFile_Printer(void* data, void* fp, int flag) 
{
    // print to fp
    int theDeletes = ((ModFile*)data)->deletes;
    int theInserts = ((ModFile*)data)->inserts;
    char* theFilename = ((ModFile*)data)->filename;

    fprintf( (FILE*)fp, "Total Inserts:%d\tTotal Deletes:%d\t%s\n", theInserts, theDeletes, theFilename);

}

void ModFile_Deleter(void* data) 
{
    // free the dynamically allocated filename string 
    if(data != NULL)
    {
        free( ((ModFile*)data)->filename );
    }
}

node_t* FindInList(list_t* list, void* token)  
{
    // search list using specifications provided by token's type and search value to provide to comparator
    // return pointer to matching node
    // note use of _t for template (will use for your code too)

    node_t* current = list->head;
    while(current != NULL)
    {
        
        
        current = current->next;
    }

    
    return NULL;
}

void DestroyList(list_t** list) {

}

void ProcessModFile(FILE* fp, list_t* list, char ordering) {

}


// Part 3 Functions to implement
void AuthorPrinter(void* data, void *fp, int flag) 
{

}

int AuthorEmailComparator(const void* lhs, const void* rhs)  {
    return -999;
}

int AuthorCommitComparator(const void* lhs, const void* rhs) {
    return -999;
}

void AuthorDeleter(void* data)  {

}

Author* CreateAuthor(char* line, long int *timestamp)  {
    return NULL;
}


// Part 4 Functions to implement
void PrintNLinkedList(list_t* list, FILE* fp, int NUM) {

}
