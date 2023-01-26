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
        if(list->comparator(current->data, token) == 0) // it works, but please ask how it works, I DON'T GET ITTTTT
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

    if(AuthorEmailComparator(lhs, rhs) == 0 && lhsCommitCount == rhsCommitCount)
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
        printf("banana\n");
    }


}

void AuthorDeleter(void* data)  
{
    //if(data != NULL)
    //{
    //    ModFile_Deleter(data->modFileList);
    //}
}

Author* CreateAuthor(char* line, long int *timestamp)  
{
    
}


// Part 4 Functions to implement
void PrintNLinkedList(list_t* list, FILE* fp, int NUM) {

}
