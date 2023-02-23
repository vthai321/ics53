#include "helpers.h"
#include "icssh.h"
#include "linkedList.h"
#include <stdio.h>
//#include <stdlib.h>

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
    memset(&myMsg, 0, 100);

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
    deleteList(list); // might not work
}

int shellRedirection(job_info* job, char* line, pid_t *wait_result, int *exit_status, List_t *bgentry_List)
{
    
    // part 4
    int fd1 = 0; // input
    int fd2 = 0; // output
    int fd3 = 0; // error
    int pid;
    int exec_result;
    
    if(job->in_file != NULL && job->out_file != NULL)
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
		
        if(job->bg == 1 && pid != 0) // parent only 
		{	
			bgentry_t* newBgentry = malloc(sizeof(bgentry_t));
			newBgentry->job = job;
			newBgentry->pid = pid; 
			newBgentry->seconds = time(NULL); 

			// use the comparator to insert into the list (already makes the node)
			insertInOrder(bgentry_List, newBgentry);
		}
        
        if(pid == 0)
        {
            // open the necessary files
            // use bitwise | for multiple flags
            fd1 = open(job->in_file, O_RDONLY);
            fd2 = open(job->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0777); // creates file if needed
            if(job->procs->err_file != NULL)
            {
                fd3 = open(job->procs->err_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            }
            if(fd1 < 0 || fd2 < 0 || fd3 < 0)
            {
                fprintf(stderr, RD_ERR);
                exit(EXIT_FAILURE);
            }

            // use dup
            dup2(fd2, STDOUT_FILENO); // stdout now refers to output file
            dup2(fd1, STDIN_FILENO); // stdin now refers to input file
            if(job->procs->err_file != NULL)
            {
                dup2(fd3, 2);
            }

            // call execvp
            exec_result = execvp(job->procs->cmd, job->procs->argv);
            if (exec_result < 0) 
            {  //Error checking
                printf(EXEC_ERR, job->procs->cmd);
                // Cleaning up to make Valgrind happy 
                // (not necessary because child will exit. Resources will be reaped by parent)
                free_job(job);  
                free(line);
                validate_input(NULL);  // calling validate_input with NULL will free the memory it has allocated
                exit(EXIT_FAILURE);
            }
            exit(0);
        }
        else
        {
			if(job->bg == 0)
			{
				*wait_result = waitpid(pid, exit_status, 0);
				if (wait_result < 0) 
				{
					printf(WAIT_ERR);
					exit(EXIT_FAILURE);
				}
			}
        }
        
        if(job->bg == 0)
		{
			free_job(job);  // if a foreground job, we no longer need the data
		}
		free(line);  
    }
    else if(job->in_file != NULL)
    {
        // < provided only aka input only
        if(job->procs->err_file != NULL)
        {
            if(strcmp(job->procs->err_file, job->in_file) == 0)
            {
                //cannot use 2 operators on same file
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

        if(job->bg == 1 && pid != 0) // parent only 
		{	
			bgentry_t* newBgentry = malloc(sizeof(bgentry_t));
			newBgentry->job = job;
			newBgentry->pid = pid; 
			newBgentry->seconds = time(NULL); 

			// use the comparator to insert into the list (already makes the node)
			insertInOrder(bgentry_List, newBgentry);
		}

        if(pid == 0)
        {
            fd1 = open(job->in_file, O_RDONLY);
            if(job->procs->err_file != NULL)
            {
                fd3 = open(job->procs->err_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            }
            if(fd1 < 0 || fd3 < 0)
            {
                fprintf(stderr, RD_ERR);
                exit(EXIT_FAILURE);
            }

            dup2(fd1, 0);
            if(job->procs->err_file != NULL)
            {
                dup2(fd3, 2);
            }
            exec_result = execvp(job->procs->cmd, job->procs->argv);
            if (exec_result < 0) 
            {  //Error checking
                printf(EXEC_ERR, job->procs->cmd);
                // Cleaning up to make Valgrind happy 
                // (not necessary because child will exit. Resources will be reaped by parent)
                free_job(job);  
                free(line);
                validate_input(NULL);  // calling validate_input with NULL will free the memory it has allocated
                exit(EXIT_FAILURE);
            }           
            close(fd1);
            if(job->procs->err_file != NULL)
            {
                close(fd3);
            }              
        }
        else
        {
			if(job->bg == 0)
			{
				*wait_result = waitpid(pid, exit_status, 0);
				if (wait_result < 0) 
				{
					printf(WAIT_ERR);
					exit(EXIT_FAILURE);
				}
			}
        }

        if(job->bg == 0)
		{
			free_job(job);  // if a foreground job, we no longer need the data
		}
		free(line);

    }
    else if(job->out_file != NULL)
    {
        // > provided only aka output only
        // name is always passed, even if the file initially does not exist?
        if(job->procs->err_file != NULL)
        {
            if(strcmp(job->procs->err_file, job->out_file) == 0)
            {
                //cannot use 2 operators on same file
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

        if(job->bg == 1 && pid != 0) // parent only 
		{	
			bgentry_t* newBgentry = malloc(sizeof(bgentry_t));
			newBgentry->job = job;
			newBgentry->pid = pid; 
			newBgentry->seconds = time(NULL); 

			// use the comparator to insert into the list (already makes the node)
			insertInOrder(bgentry_List, newBgentry);
		}

        if(pid == 0)
        {
            fd2 = open(job->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if(job->procs->err_file != NULL)
            {
                fd3 = open(job->procs->err_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            }
            if(fd2 < 0 || fd3 < 0)
            {
                fprintf(stderr, RD_ERR);
                perror("fileDBad");
                exit(EXIT_FAILURE);
            }

            dup2(fd2, 1);
            if(job->procs->err_file != NULL)
            {
                dup2(fd3, 2);
            }
            exec_result = execvp(job->procs->cmd, job->procs->argv);
            if (exec_result < 0) 
            {  //Error checking
                printf(EXEC_ERR,  job->procs->cmd);
                // Cleaning up to make Valgrind happy 
                // (not necessary because child will exit. Resources will be reaped by parent)
                free_job(job);  
                free(line);
                validate_input(NULL);  // calling validate_input with NULL will free the memory it has allocated
                exit(EXIT_FAILURE);
            }
            close(fd2);
            if(job->procs->err_file != NULL)
            {
                close(fd3);
            }             
        }
        else
        {
			if(job->bg == 0)
			{
				*wait_result = waitpid(pid, exit_status, 0);
				if (wait_result < 0) 
				{
					printf(WAIT_ERR);
					exit(EXIT_FAILURE);
				}
			}
        }

        if(job->bg == 0)
		{
			free_job(job);  // if a foreground job, we no longer need the data
		}
		free(line);
    }
    else if(job->procs->err_file != NULL)
    {
        // 2> only
        
        // time to fork
        if ((pid = fork()) < 0) 
        {
			perror("fork error");
			exit(EXIT_FAILURE);
		}

        if(job->bg == 1 && pid != 0) // parent only 
		{	
			bgentry_t* newBgentry = malloc(sizeof(bgentry_t));
			newBgentry->job = job;
			newBgentry->pid = pid; 
			newBgentry->seconds = time(NULL); 

			// use the comparator to insert into the list (already makes the node)
			insertInOrder(bgentry_List, newBgentry);
		}

        if(pid == 0)
        {
            fd3 = open(job->procs->err_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if(fd3 < 0)
            {
                fprintf(stderr, RD_ERR);
                exit(EXIT_FAILURE);
            }
            dup2(fd3, 2);
            
            exec_result = execvp(job->procs->cmd, job->procs->argv);
            if (exec_result < 0) 
            {  //Error checking
                printf(EXEC_ERR, job->procs->cmd);
                // Cleaning up to make Valgrind happy 
                // (not necessary because child will exit. Resources will be reaped by parent)
                free_job(job);  
                free(line);
                validate_input(NULL);  // calling validate_input with NULL will free the memory it has allocated
                exit(EXIT_FAILURE);
            }
            if(job->procs->err_file != NULL)
            {
                close(fd3);
            }  
        }
        else
        {
			if(job->bg == 0)
			{
				*wait_result = waitpid(pid, exit_status, 0);
				if (wait_result < 0) 
				{
					printf(WAIT_ERR);
					exit(EXIT_FAILURE);
				}
			}
        }

        if(job->bg == 0)
		{
			free_job(job);  // if a foreground job, we no longer need the data
		}
		free(line);
    }
    else
    {
        return 2;  // nothing done involving redirection
    }
}

void execute(char* line, pid_t pid, pid_t* wait_result, int* exec_result, int* exit_status, job_info* job)
{
    if (pid == 0) 
    {  //If zero, then it's the child process
            	//get the first command in the job list
		    proc_info* proc = job->procs;
			*exec_result = execvp(proc->cmd, proc->argv);
			if (*exec_result < 0) 
            {  //Error checking
				printf(EXEC_ERR, proc->cmd);
				
				// Cleaning up to make Valgrind happy 
				// (not necessary because child will exit. Resources will be reaped by parent)
				free_job(job);  
				free(line);
    			validate_input(NULL);  // calling validate_input with NULL will free the memory it has allocated
				exit(EXIT_FAILURE);
			}
		}
        else 
        { 
            // As the parent, wait for the foreground job to finish
			if(job->bg == 0)
			{
				*wait_result = waitpid(pid, exit_status, 0);
				if (*wait_result < 0) 
				{
					printf(WAIT_ERR);
					exit(EXIT_FAILURE);
				}
			}
		}
		if(job->bg == 0)
		{
			free_job(job);  // if a foreground job, we no longer need the data
		}
		free(line); 
}

void doPipe(job_info* job, int* pid, int* exec_result, int* exit_status, pid_t* wait_result, char* line)
{
    // pipefd[0] is read fd
    // pipefd[1] is write fd

    //fork 2 times
    if(job->nproc == 2)
    {
        int pipefd1[2]; // to store fd
        if(pipe(pipefd1) == -1)
        {
            exit(EXIT_FAILURE); // placeholder
        }
        
        //fork for manager below
        if ((*pid = fork()) < 0) 
		{
			perror("fork error");
			exit(EXIT_FAILURE);
		}

        if(*pid == 0) // manager
        {

            if ((*pid = fork()) < 0) 
            {
                perror("fork error");
                exit(EXIT_FAILURE);
            }

            if(*pid == 0)
            {
                close(pipefd1[0]);
                dup2(pipefd1[1], 1); // copy the pipe into stdout 
                close(pipefd1[1]);

                proc_info* proc = job->procs;
                *exec_result = execvp(proc->cmd, proc->argv);
                if (*exec_result < 0) 
                {  //Error checking
                    printf(EXEC_ERR, proc->cmd);
                    
                    // Cleaning up to make Valgrind happy 
                    // (not necessary because child will exit. Resources will be reaped by parent)
                    free_job(job);  
                    free(line);
                    validate_input(NULL);  // calling validate_input with NULL will free the memory it has allocated
                    exit(EXIT_FAILURE);
                }
            }
            else // other process
            {
                /*
                *wait_result = waitpid(*pid, exit_status, 0);
                if (*wait_result < 0) 
                {
                    printf(WAIT_ERR);
                    exit(EXIT_FAILURE);
                }
                
                if ((*pid = fork()) < 0) 
                {
                    perror("fork error");
                    exit(EXIT_FAILURE);
                }
                */
                
                    close(pipefd1[1]); // close write
                    dup2(pipefd1[0], 0); // change stdin
                    close(pipefd1[0]);
                    // note: for the arrow operator chaining below, we may need a helper function loop for infinite piping implementation
                    proc_info* proc = job->procs->next_proc;
                    *exec_result = execvp(proc->cmd, proc->argv);
                    if (*exec_result < 0) 
                    {  //Error checking
                        printf(EXEC_ERR, proc->cmd);
                        
                        // Cleaning up to make Valgrind happy 
                        // (not necessary because child will exit. Resources will be reaped by parent)
                        free_job(job);  
                        free(line);
                        validate_input(NULL);  // calling validate_input with NULL will free the memory it has allocated
                        exit(EXIT_FAILURE);
                    }
            }
        }    
        else
        {
            close(pipefd1[0]);
            close(pipefd1[1]);
            *wait_result = waitpid(*pid, exit_status, 0);
            if (*wait_result < 0) 
            {
                printf(WAIT_ERR);
                exit(EXIT_FAILURE);
            }
            exit(0);
        }
    }
    else if(job->nproc == 3)
    {
        // fork 3 times
        int pipefd1[2]; // pipe 1
        int pipefd2[2]; // pipe 2

        if(pipe(pipefd1) == -1)
        {
            exit(EXIT_FAILURE); // placeholder
        }
        
        if(pipe(pipefd2) == -1)
        {
            exit(EXIT_FAILURE); // placeholder
        }
        
        if ((*pid = fork()) < 0) 
		{
			perror("fork error");
			exit(EXIT_FAILURE);
		}

        if(*pid == 0) // manager
        {
            
            if ((*pid = fork()) < 0) 
		    {
			    perror("fork error");
			    exit(EXIT_FAILURE);
		    }

            if(*pid == 0) // all children under care of manager
            {
                close(pipefd2[0]); // close pipe 2 
                close(pipefd2[1]);
                close(pipefd1[0]); // close read for pipe 1
                dup2(pipefd1[1], 1); // copy the write for pipe 1 into stdout 

                proc_info* proc = job->procs;
                *exec_result = execvp(proc->cmd, proc->argv);
                if (*exec_result < 0) 
                {  //Error checking
                    printf(EXEC_ERR, proc->cmd);
                    
                    // Cleaning up to make Valgrind happy 
                    // (not necessary because child will exit. Resources will be reaped by parent)
                    free_job(job);  
                    free(line);
                    validate_input(NULL);  // calling validate_input with NULL will free the memory it has allocated
                    exit(EXIT_FAILURE);
                }
            }
            else
            {   
                if ((*pid = fork()) < 0) 
                {
                    perror("fork error");
                    exit(EXIT_FAILURE);
                }
                
                if(*pid == 0)
                {
                    close(pipefd1[1]); // close write of pipe 1
                    close(pipefd2[0]); // close read of pipe2
                    dup2(pipefd2[1], 1); // change stdout to write of pipe 2 (for process C)
                    dup2(pipefd1[0], 0); // change stdin to read of pipe 1 (from process A)
                    // note: for the arrow operator chaining below, we may need a helper function loop for infinite piping implementation
                    proc_info* proc = job->procs->next_proc;
                    *exec_result = execvp(proc->cmd, proc->argv);
                    if (*exec_result < 0) 
                    {  //Error checking
                        printf(EXEC_ERR, proc->cmd);
                        
                        // Cleaning up to make Valgrind happy 
                        // (not necessary because child will exit. Resources will be reaped by parent)
                        free_job(job);  
                        free(line);
                        validate_input(NULL);  // calling validate_input with NULL will free the memory it has allocated
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    close(pipefd1[0]); // close pipe 1
                    close(pipefd1[1]);
                    close(pipefd2[1]); // close write of pipe 2 
                    dup2(pipefd2[0], 0); // change stdin to read of pipe 2
                    // note: for the arrow operator chaining below, we may need a helper function loop for infinite piping implementation
                    proc_info* proc = job->procs->next_proc->next_proc;
                    *exec_result = execvp(proc->cmd, proc->argv);
                    if (*exec_result < 0) 
                    {  //Error checking
                        printf(EXEC_ERR, proc->cmd);
                        
                        // Cleaning up to make Valgrind happy 
                        // (not necessary because child will exit. Resources will be reaped by parent)
                        free_job(job);  
                        free(line);
                        validate_input(NULL);  // calling validate_input with NULL will free the memory it has allocated
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
        else
        {
            close(pipefd1[0]);
            close(pipefd1[1]);
            close(pipefd2[0]);
            close(pipefd2[1]);
            *wait_result = waitpid(*pid, exit_status, 0);
            if (*wait_result < 0) 
            {
                printf(WAIT_ERR);
                exit(EXIT_FAILURE);
            }
            exit(0);
        }
    }
    else
    {
        // infinite piping goes here
        // if I get this to work it can replace the hard-coded segments for piping 2 or 3 processes
        /*
        int pipefd1[2];
        int pipefd2[2];

        proc_info* proc = job->procs;
        while(proc != NULL)
        {

            proc = proc->next_proc;
        }

        */


    }

} 

void ascii53()
{
    printf(
            "//////////    /////////    /////////        //////////     ////////         $$$$$      $$$$$      \n"
            "   ///        ////         ///              ///                 ///         $$$$$      $$$$$      \n"
            "   ///        ///          ///              ///                 ///         $$$$$      $$$$$      \n"
            "   ///        //           ///              ///                 ///                 >             \n"
            "   ///        //           ///                 ///        /////////                   >           \n"
            "   ///        //           /////////              ///           ///                 >             \n"
            "   ///        //                 ///              ///           ///         *****************     \n"
            "   ///        //                 ///              ///           ///         *                *    \n"
            "   ///        ////               ///           ///              ///          *              *     \n"
            "//////////    /////////    /////////        ///            ////////           **************      \n"
          );
}

void setShellPrompt(char* shellPromptMoreInfo)
{
    //clear the string to process
    shellPromptMoreInfo[0] = '\0';
    
    // shell
	char shell[200] = "\x1B[1;34m";  // please change to array notation where we allocate enough space 
    char* envShell = getenv("HOME"); // please change to array notation where we allocate enough space
    if(envShell == NULL)
    {
        return; // dont change anything
    }
	strcat(shell, envShell);
	// host name
	char hostName[200] = "\x1B[0;35m";
	char* envHostName = getenv("HOSTNAME");
    if(envHostName == NULL)
    {
        return; // dont change anything
    }
	strcat(hostName, envHostName);
	// path
	char promptShell[200] = "\x1B[1;31m";
	char* envPromptShell = getenv("SHELL");
    if(envPromptShell == NULL)
    {
        return; // dont change anything
    }
	strcat(promptShell, envPromptShell);

	char* endOfPrompt = "\x1B[1;32m<53shell>$ ";

	strcat(shellPromptMoreInfo, shell);
	strcat(shellPromptMoreInfo, hostName);
	strcat(shellPromptMoreInfo, promptShell);
	strcat(shellPromptMoreInfo, endOfPrompt);

    strcat(shellPromptMoreInfo, "\x1B[0m");
}

