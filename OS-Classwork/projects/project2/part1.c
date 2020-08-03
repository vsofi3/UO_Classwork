#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

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
	pid_t pid[numProgs];

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
	
	int i;
	for (i = 0; i < numProgs; i ++) {
		pid[i] = fork();
		if (pid[i] < 0) {
			exit(EXIT_FAILURE);
		}	
		if (!pid[i]) {
			if (execvp(cmds[i], args[i]) < 0) {
				printf("Unrecognized Command: %s\n", cmds[i]);
				exit(EXIT_FAILURE);
			};
			printf("child exited\n"); 
			exit(-1);
		}
		
	}
	for (i = 0;  i < numProgs; i++) {
		waitpid(pid[i], NULL, 0);

	}

	for (i = 0; i < numProgs; i++) {
		free(cmds[i]);
		free(args[i]);
	} 
	free(line); 
	free(token);  
	return 1;
}
