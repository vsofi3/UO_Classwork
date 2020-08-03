/*
* Description: A simple lab showing signal processing
*
* Author: Jared Hall
*
* Date: 10/21/2019
*
* Notes:
* 1. to be done in lab
*/

/*-------------------------Preprocessor Directives---------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
/*---------------------------------------------------------------------------*/
/*-----------------------------Program Main----------------------------------*/
void handler(int signum) {
	if (signum == SIGUSR1) {
		sigset_t sigset;
		int sig;
		sigemptyset(&sigset);
		sigaddset(&sigset, SIGUSR1);
		sigprocmask(SIG_BLOCK, &sigset, NULL);
		printf("Child Process %d  Received Signal %d\n", getpid(), signum);
		sigwait(&sigset, &sig);
	}
	if (signum == SIGINT) {
		printf("Goodbye cruel world\n");
	}
	return;
}
void signaler(pid_t pid, int signum) {
	kill(pid, signum);
	return;
}
int  main()
{
	//variable definitions
	struct sigaction sa;
	sa.sa_handler = handler;
	sigaction(SIGUSR1, &sa, NULL);
/*	int i = 0;
	while (i < 100) {
		printf("--------\n");
		sleep(1);
		signaler(getpid(), SIGUSR1);	
		i++;
	}
	return 1;*/
	pid_t  pid[5], w[5];
	int wstatus, i;
	i = 0;
	for (i=0; i < 5; i++) {
		pid[i] = fork();
		if (pid[i] == 0) {
			printf("Child Process: %i - Starting infinite loop...\n", getpid());
			while(1) {
			i++;
			if(i%10000) {
				printf("Child Process: %i - Running infinite loop...\n", getpid());
				i=0;
				sleep(1);
				}
			}
		  }
	
		else {
			sleep(2);
			signaler(pid[i], SIGUSR1);
		//	printf("calling sigusr1 again\n");
			sleep(2);
			signaler(pid[i], SIGINT); 
		} 
	
	}
	for (i = 0; i < 5; i++) { 
		printf("Parent Process: %i, Waiting for child to finish...\n", getpid());
		w[i] = waitpid(pid, &wstatus, WUNTRACED | WCONTINUED);
		printf("All child processes joined. Exiting.\n");
	}
	
}
/*-----------------------------Program End-----------------------------------*/
