#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "command.c"

int main(int argc, char* argv[]) {
	
	if (argc == 1) {
		char *buffer; 
		size_t strsize = 320; 
		const char delim[2] = " ";
		const char* exit = "exit\n";
		char* token; 
		int condition = 1; 

		buffer = (char *) malloc (strsize * sizeof(char));

		while(condition) {
			printf(">>> ");
			getline(&buffer, &strsize, stdin);
			char* commands[strsize];
			token = strtok(buffer, " ");
			int i = 0;
			while(token) {
				commands[i] = token;
				token = strtok(NULL, " ");
				i++;
			}
			int newLine = strlen(commands[i-1]);
			if (commands[i-1][newLine - 1] == '\n')
				commands[i-1][newLine - 1] = '\0';
			
			condition = commandMaster(commands, i);
		}
		
	free(buffer);
	return 1;
	}
	
	if (argc == 3) {
		if(strcmp(argv[1], "-f") == 0) {
			FILE* fin; 
			char* line = NULL;
			size_t len = 320;
			ssize_t nread;
			char* token;
			fin = fopen(argv[2], "r");
			if (fin == NULL) {
				exit(EXIT_FAILURE);
			}
			FILE* fout = freopen("output.txt", "w", stdout);
			while((nread = getline(&line, &len, fin) != -1)) {
				int i = 0;
				token = strtok(line, " ");
				char* commands[len];
				while (token) {
					commands[i] = token;
					token = strtok(NULL, " ");
					i++;
				}
				int j = strlen(commands[i-1]) - 1; //yes this makes no sense, but this
				char *plz;	  //is the only thing that's worked after
				strcpy(plz, commands[i-1]);	// 2 hours of addressing the newline character
				plz[j - 1] = '\0';
				commands[i-1] = plz;
				int verdict = commandMaster(commands, i);
				if (!verdict)
					break;
			}
			free(token);
			free(line);
			fclose(fin);
			fclose(fout);
			return 1;
		}	
		else {
			printf("Incorrect arguments. To run program in filemode, use\n pseudo-shell -f <filename>\n");
			return -1;
		}
	}

	else{
		printf("Program requires one of these two formats. \npseudo-shell for interactive mode or pseudo-shell -f <filename> for filemode\n");
		return -1;
	}
	
}
