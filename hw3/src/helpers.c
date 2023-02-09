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

void terminateDeleter(List_t* list, int terminatePID)
{
    int index = 0;
    node_t* current = list->head;
    bgentry_t* currentBGentry = NULL;
    if(current != NULL)
    {
        currentBGentry = (bgentry_t*)(current->value);
    }

    while(current != NULL) 
    {
        // find the node to delete based on PID
        // then free everything in the node before deleting the node itself
        char cmd[300];
        strncpy(cmd, currentBGentry->job->line, 300);
        cmd[300] = '\0'; // just in case  
        int currentPID = currentBGentry->pid;
        if(currentPID == terminatePID)
        {            
            // now free the job in bgentry
            free_job(currentBGentry->job);
            // now delete the node
            free(currentBGentry);
            currentBGentry = NULL;
            
            removeByIndex(list, index);
            printf(BG_TERM, terminatePID, cmd);
            return;
        }
        else
        {
            current = current->next;
            if(current != NULL)
            {
                currentBGentry = (bgentry_t*)(current->value);
            }
            ++index;
        }
    }
}

void sigusr2Handler(int sig)
{
    // should implement some signal blockers here using sigprogmask
    // block the whole thing, feel free to use anything!!!!!!!!!!!!
    // also, write is the only async safe thing to use
    // pid of the process that receives the signal (use getpid)
    
    sigset_t mask, prev; // the set to store signals (to mask), and the set to store prev signals so they don't get lost
    sigfillset(&mask); // initialize set to contain ALL signals that you can block ;)
    sigaddset(&mask, SIGUSR2);
    
    sigprocmask(SIG_BLOCK, &mask, &prev);
    //below is not the most efficient way, but is practice using these functions
    int callerPid = (int)getpid();
    char myMsg[100];

    snprintf(myMsg, 50, "Hi User! I am process %d\n", callerPid);
    write(1, myMsg, 100);

    sigprocmask(SIG_SETMASK, &prev, NULL);
}

void terminateAll(List_t* list)
{
    node_t* current = list->head;
    bgentry_t* currentBGentry = NULL;
    if(current != NULL)
    {
        currentBGentry = (bgentry_t*)(current->value);
    }
    while(current != NULL) 
    {
        current = current->next;
        // for every bentry, print BG_TERM, then delete
        char cmd[300];
        strncpy(cmd, currentBGentry->job->line, 300);
        cmd[300] = '\0'; // just in case  
        int currentPID = currentBGentry->pid;
            
        // now free the job in bgentry as well as bgentry itself, and kill the process
        free_job(currentBGentry->job);
        free(currentBGentry);
        printf(BG_TERM, currentPID, cmd);
        kill(currentPID, 0);

        if(current != NULL)
        {
            currentBGentry = (bgentry_t*)(current->value);
        }
    }
    // deleteList(list); not applicable to a static list
}

int shellRedirection(job_info* job)
{
    
    // part 4
    int fd1 = 0; // input
    int fd2 = 0; // output
    int fd3 = 0; // error
    int pid;
    
    if(job->in_file != NULL && job->out_file!=NULL)
    {
        // > and < both provided
        // name is always passed, even if the file initially does not exist?

        //error checking
        if(strcmp(job->in_file, job->out_file) == 0)
        {
            //cannot use 2 operators on same file
            fprintf(stderr, RD_ERR);
            return -1;
        }
        if(job->procs->err_file != NULL)
        {
            if(strcmp(job->in_file, job->procs->err_file) == 0 || strcmp(job->in_file, job->procs->err_file) == 0)
            {
                fprintf(stderr, RD_ERR);
                return -1;
            }
        }

        // time to fork
        if ((pid = fork()) < 0) 
        {
			perror("fork error");
			exit(EXIT_FAILURE);
		}

        if(pid == 0)
        {
            // open the necessary files
            // use bitwise | for multiple flags
            fd1 = open(job->in_file, O_RDONLY);
            fd2 = open(job->out_file, O_WRONLY | O_CREAT); // creates file if needed
            if(job->procs->err_file != NULL)
            {
                fd3 = open(job->procs->err_file, O_WRONLY | O_CREAT);
            }
            if(fd1 < 0 || fd2 < 0 || fd3 < 0)
            {
                perror("file error");
                exit(0);
            }

            // use dup
            dup2(fd2, 1); // stdout now refers to output file
            dup2(fd1, 0); // stdin now refers to input file

            // call execvp
            execvp(job->procs->cmd, job->procs->argv);
        }
        
            
    }
    else if(job->in_file != NULL)
    {
        // < provided only
    }
    else if(job->out_file != NULL)
    {
        // > provided only
        // name is always passed, even if the file initially does not exist?
    }
    return 0;
}
