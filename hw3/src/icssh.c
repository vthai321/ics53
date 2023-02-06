#include "icssh.h"
#include "helpers.h"
#include "linkedList.h"
#include <readline/readline.h>

// flag for terminated child (bg) (global)
int terminatedChild = 0;

int main(int argc, char* argv[]) {
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

    	// print the prompt & wait for the user to enter commands string
	while ((line = readline(SHELL_PROMPT)) != NULL) {
			// MAGIC HAPPENS! Command string is parsed into a job struct
        	// Will print out error message if command string is invalid
		job_info* job = validate_input(line);
        	if (job == NULL) { // Command was empty string or invalid
			free(line);
			continue;
		}

		// temporary? place for my signal handler (ask if we can use unix_error for this assignment)
		signal(SIGCHLD, sigchldHandler);

		// check if conditional flag for terminatedChild is set 
		if(terminatedChild)
		{
			// reap all terminated background child processes, 1 at a time
			// to reap a child, call waitpid() before exit()
			// use pid to determine which process to terminate?
			waitpid(pid, &exit_status, 0); 
			// remove it from the bgentry

		}

        	//Prints out the job linked list struture for debugging
        	#ifdef DEBUG   // If DEBUG flag removed in makefile, this will not longer print
            		debug_print_job(job);
        	#endif

		// example built-in: exit
		if (strcmp(job->procs->cmd, "exit") == 0) {
			// Terminating the shell
			free(line);
			free_job(job);
            		validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
            		return 0;
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
					continue;
				}
				else
				{
					fprintf(stderr, DIR_ERR);
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
					continue;
				}
				else
				{
					fprintf(stderr, DIR_ERR);
					continue;
				}

			}
		}

		//estatus
		//builtin command to get exit status of most recently reaped program (aka child process)
		if(strcmp(job->procs->cmd, "estatus") == 0)
		{
			fprintf(stdout, "%d\n", exit_status); // is it this easy
			continue; 
		}

		// example of good error handling!
		// the part where we fork
		if ((pid = fork()) < 0) {
			perror("fork error");
			exit(EXIT_FAILURE);
		}

		// part 2
		// the job info struct returned by the tokenizer will set the bg field to yes <-- use that!!!
		// put it after fork
		if(job->bg == 1 && pid == 0)
		{
			printf("INITIATING BACKGROUND PROCESS CODE\n");
			
			bgentry_t* newBgentry = malloc(sizeof(bgentry_t));
			newBgentry->job = job;
			newBgentry->pid = pid; 
			newBgentry->seconds = time(NULL); 

			// make the node here
			node_t* newBgNode = malloc(sizeof(node_t)); // dynamically allocated to perserve it 
			newBgNode->value = newBgentry;
			newBgNode->next = NULL;
			
			if(bgentry_List.head == NULL)
			{
				// stick the node in as the head
				bgentry_List.head = newBgNode;
			}
			else
			{
				// use the comparator to insert into the list
				// otherwise, insert based on time
				insertInOrder(&bgentry_List, newBgNode);
			}
			++bgentry_List.length;
		}

		// SIGCHLD handler goes here (or anywhere or even at the beginning of loop???)


		// the part where we do jobs with parents and children (for fg?)
		if (pid == 0) {  //If zero, then it's the child process
            	//get the first command in the job list
		    proc_info* proc = job->procs;
			exec_result = execvp(proc->cmd, proc->argv);
			if (exec_result < 0) {  //Error checking
				printf(EXEC_ERR, proc->cmd);
				
				// Cleaning up to make Valgrind happy 
				// (not necessary because child will exit. Resources will be reaped by parent)
				free_job(job);  
				free(line);
    				validate_input(NULL);  // calling validate_input with NULL will free the memory it has allocated

				exit(EXIT_FAILURE);
			}
		} else {
            		// As the parent, wait for the foreground job to finish
			wait_result = waitpid(pid, &exit_status, 0);
			if (wait_result < 0) {
				printf(WAIT_ERR);
				exit(EXIT_FAILURE);
			}
		}

		free_job(job);  // if a foreground job, we no longer need the data
		free(line);
	}

    	// calling validate_input with NULL will free the memory it has allocated
    	validate_input(NULL);

#ifndef GS
	fclose(rl_outstream);
#endif
	return 0;
}