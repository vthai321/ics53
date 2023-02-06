#include "helpers.h"
#include "icssh.h"
#include "linkedList.h"

// Your helper functions need to be here.
int bGentryTime_Comparator(void* bGentryOne, void* bGentryTwo)
{
    bgentry_t* firstItem = (bgentry_t*) bGentryOne;
    bgentry_t* secItem = (bgentry_t*) bGentryTwo;

    if(firstItem->seconds == secItem->seconds)
    {
        return 0;
    }
    else if(firstItem->seconds > secItem->seconds)
    {
        return 1;
    }
    else if(firstItem->seconds < secItem->seconds)
    {
        return -1;
    }

}

void sigchldHandler(int sig)
{
    // set a conditional flag denoting terminated child
    terminatedChild = 1;
    
}

void terminateDeleter(List_t* list)
{
    int index = 0;
    node_t* current = list->head;
    while(current != NULL) // should handle the empty case, but do watch out
    {
        // f it lets just delete everything (to see if it works )
        removeByIndex(list, index);
        current = current->next;
        ++index;
    }
}