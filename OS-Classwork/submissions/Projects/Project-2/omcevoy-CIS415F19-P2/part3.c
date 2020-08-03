#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

int NUMPROG;
int ind = 0; 
int finProg = 0;
pid_t pid[10];
int ultimatum () {
	int i, waitstatus;
	int status = 1;
	pid_t test;
	for (i=0;  i < NUMPROG ; i++) {
		test =	waitpid(pid[i], &waitstatus, WNOHANG);
		if (test == -1) {
			if (pid[i] != 55555) {	//simple way to avoid counting a finished process twice
				finProg++;		
			//	printf("%d processes have finished\n", finProg);
				pid[i] = 55555;
			}
		}		
	}
	if (finProg ==  NUMPROG) 
		status = 0; //when all processes have finished, the should exit
	return status;
}

void signaler(pid_t pid[], int signum, int limit) {
	int i = 0;
	if (limit == 1) {
		ind = i;
		i++;
	}
	for(i; i < NUMPROG; i++) {
		kill(pid[i], signum);
	}
	return;
}

void alrm_hndlr() {
	sleep(1);
	int anything = ultimatum();	
	int i = 0;
	while(anything) {
		if(!kill(pid[ind], SIGSTOP)){
			printf("Process: %d is stopping\n", pid[ind]);
			ind ++;
			if (ind > NUMPROG - 1) //ensures ind is within reasonable bounds
				ind = 0;
			break;
		}
		else {
			ind++;
			if (ind > NUMPROG - 1){
				ind = 0;
				i++;
				if (i > NUMPROG)
					return;
			}
		}
	}	
	while(anything) {
		if (!kill(pid[ind], SIGCONT)){
			printf("Process: %d is starting\n", pid[ind]);
			break;
		}
		else {
			ind++;
			if (ind > NUMPROG - 1){
				ind = 0;
				i++;
				if (i > NUMPROG)
					return;
			}
		}
	}
	return;		
}

int main(int argc, char* argv[]) {
	FILE* fin;
	char* line = NULL;
	size_t len = 0;
	ssize_t nread;
	int numProgs = 0;
	char* token;
	char* cmds[100];
	char** args[100];
	fin = fopen(argv[1], "r");
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);	 
	sigprocmask(SIG_BLOCK, &sigset, NULL);
	struct sigaction sa;
	sa.sa_handler = &alrm_hndlr;
	sigaction(SIGALRM, &sa, NULL);

	if (fin == NULL){
		free(fin);
		exit(EXIT_FAILURE);
	}
	while((nread = getline(&line, &len, fin) != -1)) {
		char *dummy; //for duplication of line
		dummy = (char*) malloc(sizeof(char*) * len);
		dummy = strcpy(dummy, line);
		args[numProgs] = malloc(sizeof(char**) * 100);		
		token = strtok(dummy, " "); //acquires command from line
		int tokenLen = strlen(token);
		if (token[tokenLen - 1] == '\n')
			token[tokenLen - 1] = '\0';
		cmds[numProgs] = token;
		int i = 0;
		while(token) {
			int argLen = strlen(token);
			if (token[argLen - 1] == '\n')
				token[argLen - 1] = '\0';
			args[numProgs][i] = token;
			i++;
			token = strtok(NULL, " "); 
					
		}
		args[numProgs][i] = NULL;
		numProgs++;
	}
	fclose(fin);
	NUMPROG = numProgs; //set global variable NUMPROG
	int i;
	for (i = 0; i < numProgs; i ++) {
		int sig;
		pid[i] = fork();
		if (pid[i] < 0) {
			exit(EXIT_FAILURE);
		}	
		if (!pid[i]) {
			printf("Process: %d waiting for SIGUSR1 to continue\n", getpid());
			sigaction(SIGUSR1, &sa, NULL);
			sigwait(&sigset, &sig);
			printf("Process: %d received SIGUSR1, beginning execution\n", getpid());
			if (execvp(cmds[i], args[i]) < 0) {
				printf("Unrecognized Command: %s\n", cmds[i]);
				exit(EXIT_FAILURE);
			}
			printf("child process exiting\n"); 
			exit(-1);
		}
		
	}
	sleep(2);
        signaler(pid, SIGUSR1, 0); //starts all processes
	sleep(1);
	printf("Stopping all but 1 processes\n");
	signaler(pid, SIGSTOP, 1);
	int condition = 1;
	while(condition) {
		alarm(2); //will send alarm signal
		system("sleep 2"); //wait for alarm signal to be sent
		condition = ultimatum(); //check if all processes have finished
	}

	for (i = 0; i < numProgs; i++) {
		free(cmds[i]);
		free(args[i]);
	} 
	free(token); 
	free(line); 
	return 1;
}
