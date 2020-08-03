#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	pid_t pid;
	pid = fork();
	int child_status;
	if (!pid){
		execvp(argv[1], argv);
		exit(0);
	}
	else{
		pid_t tpid = wait(&child_status);
	}
	return 1;
	
}
