#include "icssh.h"
#include "helpers.h"
#include "linkedList.h"
#include <readline/readline.h>

// flag for terminated child (bg) (global)
//int terminatedChild = 0;
volatile sig_atomic_t terminatedChild = 0;

int main(int argc, char* argv[]) 
{
	int exec_result;
	int exit_status;
	pid_t pid;
	pid_t wait_result;
	char* line;

	// initialize the bgentry list
	List_t bgentry_List;
	bgentry_List.head = NULL;
	bgentry_List.length = 0;
	
	// function pointer stuff
	int (*timeCompare)(void*, void*) = &bGentryTime_Comparator;
	bgentry_List.comparator = timeCompare; // is this the right way to use a function pointer?

#ifdef GS
    rl_outstream = fopen("/dev/null", "w");
#endif

	// Setup segmentation fault handler
	if (signal(SIGSEGV, sigsegv_handler) == SIG_ERR) {
		perror("Failed to set signal handler");
		exit(EXIT_FAILURE);
	}

	// Setup SIGCHLD fault handler
	if(signal(SIGCHLD, sigchldHandler) == SIG_ERR)
	{
		perror("Failed to set signal handler");
		exit(EXIT_FAILURE);
	}

	// Setup SIGGUSR2 fault handler
	if(signal(SIGUSR2, sigusr2Handler) == SIG_ERR)
	{
		perror("Failed to set signal handler");
		exit(EXIT_FAILURE);
	}

    	// print the prompt & wait for the user to enter commands string
	 
	char shellPromptMoreInfo[200] = SHELL_PROMPT;
	//shellPromptMoreInfo[0] = '\0';
    // shell
	//char* shell = "\x1B[1;34m";
    //char* envShell = getenv("HOME");
	//char* test = getenv("HOME");
	#ifdef DEBUG
		setShellPrompt(shellPromptMoreInfo);
		//clear the string to process
	#endif
	while ((line = readline(shellPromptMoreInfo)) != NULL) 
	{
		// MAGIC HAPPENS! Command string is parsed into a job struct
		// Will print out error message if command string is invalid
		job_info* job = validate_input(line);
		if (job == NULL) 
		{ // Command was empty string or invalid
		free(line);
		continue;
		}

		// check if conditional flag for terminatedChild is set 
		// reap all terminated background child processes, 1 at a time
		if(terminatedChild == 1)
		{	
			// while loop thru every node
			int listLen = 0;
			while(listLen < bgentry_List.length)
			{
				int terminatePID = waitpid(0, &exit_status, WNOHANG);
				if(terminatePID == 0 || terminatePID == -1)
				{
					++listLen;
					continue;
				}
				else
				{ 
					//remove it from the bgentry
					++listLen;
					terminateDeleter(&bgentry_List, terminatePID);
				}
			}
			terminatedChild = 0;
		}

        	// Prints out the job linked list struture for debugging
			// Will also come in handy for part 2 of extra credit 
        	#ifdef DEBUG   // If DEBUG flag removed in makefile, this will not longer print
            		debug_print_job(job);
        	#endif


		// example built-in: exit
		// for part 3: modify to kill ALL running background jobs before termination
		// print BG_TERM, aim to use delete_list
		if (strcmp(job->procs->cmd, "exit") == 0) 
		{
			// clear the bg process list and kill all bg processes
			terminateAll(&bgentry_List);
			// Terminating the shell
			free(line);
			free_job(job);
            validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
            return 0;
		}

		//built-in command to draw ascii art (extra credit)
		if(strcmp(job->procs->cmd, "ascii53") == 0)
		{
			ascii53();
			free_job(job);
			free(line);
			continue; 
		}

		// built-in command to change directories
		// job is a job_info struct that contains metadata about job (entered line) and a list of the processes (procs)
		// proc info works like a linked list; job points to its head
		if(strcmp(job->procs->cmd, "cd") == 0)
		{
			// use chdir using path supplied (if there is no directory supplied, value of HOME environment variable is used)
			if(job->procs->argc == 1) // if it's equal to 1, the only argument was the name
			{
				// use getenv? returns a string pointer to specified variable
				char* homeCD = getenv("HOME");
				if(chdir(homeCD) == 0)
				{
					char cdBuff[500]; //inquire if this is necessary
					char* absPath = getcwd(cdBuff, 500);
					fprintf(stdout, "%s\n", absPath); 
					free_job(job);
					free(line);				
					continue;
				}
				else
				{
					fprintf(stderr, DIR_ERR);
					free_job(job);
					free(line);	
					continue;
				}
			}
			else
			{
				char* newDirectory = job->procs->argv[1]; // it works
				if(chdir(newDirectory) == 0)
				{
					char cdBuff[500]; //inquire if this is necessary
					char* absPath = getcwd(cdBuff, 500);
					fprintf(stdout, "%s\n", absPath); 
					free_job(job);
					free(line);	
					continue;
				}
				else
				{
					free_job(job);
					free(line);	
					fprintf(stderr, DIR_ERR);
					continue;
				}
			}
		}

		//estatus
		//builtin command to get exit status of most recently reaped program (aka child process)
		if(strcmp(job->procs->cmd, "estatus") == 0)
		{
			fprintf(stdout, "%d\n", WEXITSTATUS(exit_status)); // is it this easy
			free_job(job);
			free(line);
			continue; 
		}

		//bglist
		if(strcmp(job->procs->cmd, "bglist") == 0)
		{
			node_t* bglistCurrent = bgentry_List.head;
			while(bglistCurrent != NULL)
			{
				
				print_bgentry((bgentry_t*)(bglistCurrent->value));
				bglistCurrent = bglistCurrent->next;
			}
			free_job(job);
			free(line);
			continue;
		}

		// part 4
		if(shellRedirection(job, line, &wait_result, &exit_status, &bgentry_List) != 2)
		{
			continue; // in case of error, let us ask for another command to enter
		}

		// example of good error handling!
		// the part where we fork
		if ((pid = fork()) < 0) 
		{
			perror("fork error");
			exit(EXIT_FAILURE);
		}

		// part 2
		// the job info struct returned by the tokenizer will set the bg field to yes <-- use that!!!
		// put it after fork
		if(job->bg == 1 && pid != 0) // parent only 
		{	
			bgentry_t* newBgentry = malloc(sizeof(bgentry_t));
			newBgentry->job = job;
			newBgentry->pid = pid; 
			newBgentry->seconds = time(NULL); 

			// use the comparator to insert into the list (already makes the node)
			// memory "leak" occurs because we technically lose the pointer to that node without freeing
			insertInOrder(&bgentry_List, newBgentry);
		}
		
		// part 5 goes here 
		if(job->nproc > 1)
		{
			if(pid == 0)
			{
				doPipe(job, &pid, &exec_result, &exit_status, &wait_result, line);
			}
			else
			{		
				if(job->bg == 0)
				{
					// wait for doPipe to finish
					wait_result = waitpid(pid, &exit_status, 0);
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
			continue;
		}
		else
		{
			execute(line, pid, &wait_result, &exec_result, &exit_status, job); // helper function to handle regular case of execvp
		}
	}
    // calling validate_input with NULL will free the memory it has allocated
	validate_input(NULL);

#ifndef GS
	fclose(rl_outstream);
#endif
	return 0;
}