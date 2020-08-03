/*-----------------------------------------------------------------------------
File name: Lab6.c
Description: A simple lab showing the use of signals.
Author: Jared Hall
-----------------------------------------------------------------------------*/

/*---------------------------Preprocessor Directives-------------------------*/
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
/*---------------------------------------------------------------------------*/

/*---------------------------------Functions---------------------------------*/
/*---------------------------------------------------------------------------*/

/*--------------------------------Program Main-------------------------------*/
void signaler(pid_t pid, int signum) {
	printf("Sending %d to child process %d\n", signum, pid);
	kill(pid, signum);
	return;
}
int main(void) 
{
    //variable declarations
    pid_t pid, w;
    int wstatus, eStatus;
    int condition = 1;
    double total;
    int i = 0;
    //create a child process
    pid = fork();
    if(pid < 0) {
        perror("Error! could not create a new process.\n");
        exit(EXIT_FAILURE);
    } else if(pid == 0) {
        //This code runs in the child process only
	printf("    Child process: %d - Starting...\n", getpid());
	while (condition) {
		if (i  >= 10){ 
			condition = 0;
			break;
		}
		else {
			printf("Child Process %d - still alive after %d seconds\n", getpid(), i);
		}
		sleep(1);
		i++;
	}
	exit(EXIT_SUCCESS);

    } else if(pid > 0) {
	sleep(1);
	printf("Parent process: %d - Sending signals to child...\n", getpid());
	int j = 1;
	while (j) {	
		signaler(pid, SIGSTOP);
		sleep(3);
		signaler(pid, SIGCONT);
		sleep(1);
		j++;
		if (j > 10)
			break;
	}	
	printf("Parent process: %d - Waiting for child to complete...\n", getpid());
	w = waitpid(pid, &wstatus, 0);
	printf("Parent process: %d - Finished\n", getpid());
    }
    
    //exit out of program
    return EXIT_SUCCESS;
}
/*----------------------------------------------------------------------------*/
