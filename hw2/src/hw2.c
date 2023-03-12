// Vincent Quan Thai
// vqthai

#include "hw2.h"
#include "helpers2.h"
#include "linkedlist.h"

// Part 0 Function to implement
char* myStrCpy(char* str, char* delimiters) 
{

    if(str == NULL)
    {
        return NULL; // parameter check
    }
    
    char* newStr = (char *) malloc(myStrSize(str));    
    char currentChar = ' ';

    if(newStr == NULL)
    {
        return NULL; // malloc check
    }

    int parseString = 1; // becomes 0 if we reach null terminator or deliminator
    int offset = 0; // offset for pointer arithmetic
    int inDelimit = 0; // becomes 1 if our char is part of our delimit string
    
    int delimitBound = 0;
    if(delimiters != NULL)
    {
        delimitBound = myStrSize(delimiters) - 1; // - 1 accounts for null terminator
    }
    else
    {
        delimitBound = -1;
    }
    
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
        
        //copy string into struct?
        newModFile ->filename = myStrCpy(filename, '\0');
        //newModFile->filename = *filename;
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
            mf->inserts += ins;
            mf->deletes += dels;
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
    int file1Sum = ((ModFile*)file1)->inserts + ((ModFile*)file1)->deletes;
    int file2Sum = ((ModFile*)file2)->inserts + ((ModFile*)file2)->deletes;

    if(file1Sum == file2Sum)
    {
        return 0;
    }
    else if(file1Sum > file2Sum)
    {
        return 1;
    }
    else if(file1Sum < file2Sum)
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
    // free the ModFile too?
    if(data != NULL)
    {
        if(((ModFile*)data)->filename != NULL)
        {
            free( ((ModFile*)data)->filename );
            //free(data);
        }
    }
}

node_t* FindInList(list_t* list, void* token)  
{
    // search list using specifications provided by token's type and search value to provide to comparator
    // return pointer to matching node
    // note use of _t for template (will use for your code too) <-- just cast to appropriate data type (ModFile) if needed
    // find which comparator to use using the list struct datatype
    // invoke the function using the function pointer

    if(token == NULL)
    {
        return NULL; //empty token
    }


    node_t* current = list->head;
    while(current != NULL)
    {
        if(list->comparator(current->data, token) == 0) 
        {
            return current;
        }
        current = current->next;
    }

    
    return NULL; // not found 
}

void DestroyList(list_t** list) // pointer to a pointer, so that we can free the pointer the pointer points to  
{
    // please look at doc carefully
    if(list != NULL) // exit if we have no list
    {
        if((*list)->head == NULL)
        {
            // just need to delete and free the list pointer
            free(*list);
            // do we need free(list)? (without the *)
        }
        else
        {
            node_t* current = (*list)->head;
            node_t* nextNode = NULL;
            while(current != NULL)
            {
                nextNode = current->next;
                (*list)->deleter(current->data); // deleter deletes the data within the ModFIle within the node.
                free(current->data); // deletes the ModFule
                free(current); // free the node 
                current = nextNode;
            }
            free(*list);
            nextNode = NULL;        
        }
    }
}

void ProcessModFile(FILE* fp, list_t* list, char ordering) 
{
    // read lines from fp; end when the line from fp is empty (null terminator only?)
    // use FindInList, putModFile, and linked list functions to update linked list

    // use the ModFileABC_Comparator to sort the ModFiles
    // ordering argument: filename alphabetically (insertInOrder), or first order of appearance in file (InsertAtTale)
    // ^ use ordering parameter for this
    // if list is null, do nothing

    // a modFile needs inserts, deletes, fileName
    // the testcases skip the actual commit line, I think. Just worry about the insert, delete, filename part

    if(list == NULL)
    {
        return;
    }

    //list->comparator = ModFileABC_Comparator; // probably unnecessary, but whatever

    char* theInserts  = NULL;
    char* theDels = NULL;
    char* theFilename = NULL;
    char* buffer = (char*)malloc(205);
    char* current = buffer;

    char* fgetsResult = fgets(buffer, 200, fp);
    while(myStrcmp(buffer, "\n") != 0 && fgetsResult != NULL) 
    {
        // parsing algo
        current = buffer;

        while(*current < 48 || *current > 57) 
        {
            ++current; // avoid the letters
        }

        theInserts = myStrCpy(current, "\t\0");
        current += myStrSize(theInserts);

        while(*current < 48 || *current > 57) 
        {
            ++current; // avoid the letters
        }

        theDels = myStrCpy(current, "\t\0");
        current += myStrSize(theDels);
        theFilename = myStrCpy(current, "\n\0"); // do we include the \n or not; do we include EOF or not

        int insertNum = atoi(theInserts);
        int deleteNum = atoi(theDels);
        free(theInserts);
        free(theDels);

        ModFile* newModFile; // no need to dynamically allocate here, it's already covered
        newModFile = PutModFile(insertNum, deleteNum, theFilename, NULL); //generates a new struct every time
        node_t* posInList = FindInList(list, newModFile);
        // use findInList to prevent duplicates?
        // update if found
        if(ordering == 'a'&& posInList == NULL)
        {
            InsertInOrder(list, newModFile); 
            free(theFilename);
        }
        else if(ordering == 'f'&& posInList == NULL)
        {
            InsertAtTail(list, newModFile);
            free(theFilename);
        }
        else if(posInList != NULL)
        {
            // update ins and dels using putModFile and delete the newModFile
            PutModFile(insertNum, deleteNum, theFilename, posInList->data);
            ModFile_Deleter(newModFile);
            free(newModFile);
            free(theFilename);
        }

        //scanResult = fscanf(fp, "Total Inserts:%d\tTotal Deletes:%d\t%s\n", theInserts, theDels, theFilename);
        fgetsResult = fgets(buffer, 200, fp);
    }
    free(buffer);
    buffer = NULL;
    current = NULL;
}


// Part 3 Functions to implement
void AuthorPrinter(void* data, void *fp, int flag) 
{
    if(data != NULL)
    {
        char* authorName = ((Author*)data)->fullname;
        char* email = ((Author*)data)->email;
        int commitCount = ((Author*)data)->commitCount;

        if(authorName == NULL || email == NULL)
        {
            return; // oops, our authorName or email strings are null
        }

        if(flag == 0)
        {   
            if( ((Author*)data)->modFileList == NULL)
            {
                return; // gotta check if node has ModFile
            }
            
            int modFileListLen = ((Author*)data)->modFileList->length;
            fprintf((FILE*)fp, "%s <%s>,%d,%d\n", authorName, email, commitCount, modFileListLen);
        }
        else
        {
            fprintf((FILE*)fp, "%s <%s>,%d\n", authorName, email, commitCount);
            // use modFilePrinter
            node_t* current = NULL;
            if(((Author*)data)->modFileList == NULL)
            {
                return; // oops, this node does not have a ModFile
            }
            else
            {
                current = ((Author*)data)->modFileList->head;
            }
            while(current != NULL)
            {
                if(current->data == NULL)
                {
                    return; // oops, this node does not have a ModFile
                }
                
                fprintf((FILE*)fp, "\t");
                ModFile_Printer(current->data, fp, 0);
                current = current->next;
            }
        }
    }
}

int AuthorEmailComparator(const void* lhs, const void* rhs)  
{
    // cast the void pointers
    // use myStrcmp
    if(lhs == NULL || rhs == NULL)
    {
        return 0;
    }

    Author* theLhs = (Author*)lhs;   
    Author* theRhs = (Author*)rhs;

    if(theLhs->email == NULL || theRhs->email == NULL)
    {
        return 0;
    }

    return myStrcmp(theLhs->email, theRhs->email);  
}

int AuthorCommitComparator(const void* lhs, const void* rhs) 
{
    if(lhs == NULL || rhs == NULL)
    {
        return 0;
    }
    
    Author* theLhs = (Author*)lhs;   
    Author* theRhs = (Author*)rhs;

    // if count is equal, sort alphabetically via email
    // if email and commitCounts are identical or either is NULL, return 0
    
    int lhsCommitCount = theLhs->commitCount;
    int rhsCommitCount = theRhs->commitCount;

    if(lhsCommitCount == rhsCommitCount && AuthorEmailComparator(lhs, rhs) == 0) // short circuiting 
    {
        return 0;
    }

    //use author email comparator if we are TIED
    if(lhsCommitCount > rhsCommitCount)
    {
        return -1;
    }
    else if(lhsCommitCount < rhsCommitCount)
    {
        return 1;
    }
    else
    {
        return AuthorEmailComparator(lhs, rhs);

    }


}

void AuthorDeleter(void* data)  
{
    // Author contains: char* fullname, char* email, list_t* modFileList to free 
    // call destroyList on modFileList, manually free fullname, email
    // ther eis possible seg fault here

    Author* authorPointer = (Author*)data;
    list_t** listPointer = &(authorPointer->modFileList);

    // time to free
    free(authorPointer->fullname);
    free(authorPointer->email);
    authorPointer->fullname = NULL;
    authorPointer->email = NULL;
    DestroyList(listPointer); // frees the nodes as well as the list itself.
    listPointer = NULL; 
}

Author* CreateAuthor(char* line, long int *timestamp)  
{
    // line is provided, we will separate elements into relevant information
    // use strCpy with the delimiter feature (comma separated) or fgets()
    // use pointers to track which parts of line we've already gone over? (with myStrSize?)
    // commitHash, timestamp, fullname, email, message
    if(line == NULL)
    {
        return NULL; // line was null
    }
    
    char* linePos = line;

    // we'll store commitHash anyways just to see if its empty or not
    char* commitHash = myStrCpy(linePos, ",\0");
    if(myStrcmp(commitHash, "\0") == 0)
    {
        return NULL; //empty commit field
    }
    linePos += (myStrSize(commitHash));

    char* theTimestamp = myStrCpy(linePos,",\0");
    long int timestampNum = strtol(theTimestamp, NULL, 10); // passing null means I will increment it manually
        if(timestampNum < 0)
    {
        return NULL; // negative timestamp value is invalid
    }
    
    linePos += (myStrSize(theTimestamp)); 
    *timestamp = timestampNum;

    char* fullname = myStrCpy(linePos, ",\0"); // copy starting at where we point in the string
    linePos += (myStrSize(fullname));// increment pointer by size of string 

    char* email = myStrCpy(linePos, ",\0");
    linePos += (myStrSize(email));

    if(myStrcmp(fullname, "\0") == 0 || myStrcmp(email, "\0") == 0)
    {
        return NULL; // empty name or email field
    }

    // time to build the author (dynamically allocated)
    Author* newAuthor = malloc(sizeof(Author));
    newAuthor->fullname = myStrCpy(fullname, "\0"); // we need to make a completely separate copy, NOT the one I stored in this instance
    newAuthor->email = myStrCpy(email, "\0");
    newAuthor->commitCount = 1; // is it always 1?
    newAuthor->modFileList = CreateList(ModFileABC_Comparator, ModFile_Printer, ModFile_Deleter);

    free(fullname);
    free(email);
    free(commitHash);
    free(theTimestamp);

    theTimestamp = NULL;
    fullname = NULL;
    email = NULL;
    commitHash = NULL;

    return newAuthor;

}


// Part 4 Functions to implement
void PrintNLinkedList(list_t* list, FILE* fp, int NUM) 
{
    if(list == NULL || fp == NULL || NUM < 0 || list->head == NULL)
    {
        return;
    }
    
    int printed = 0;
    node_t* current = list->head; // points to node, not modfile

    while(current != NULL && printed < NUM) // printed max specified or reached end of list 
    {
        // use ModFile_Printer
        ModFile_Printer(current->data, fp, 0);
        current = current->next;
        ++printed;
    }
    current = NULL;
}
