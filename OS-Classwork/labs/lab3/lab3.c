/*
* Description: <This program reads and processes text from an input file. It
		then creates a new file where it writes each individual input
		from the input file. >
*
* Author: <Owen McEvoy>
*
* Date: <October 6th, 2019>
*
* Notes: 
* 1. <Credit to the links posted on the lab description for showing me how to use the getline and strtok functions>
*/

/*-------------------------Preprocessor Directives---------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE
#include <dirent.h>
#include <unistd.h>
/*---------------------------------------------------------------------------*/

/*-----------------------------Program Main----------------------------------*/

void lfcat() {
	struct dirent *ls;
	char cwdbuff[256];
	size_t size;
	ssize_t nread;
	char* line;
	size_t len = 0;
	FILE* fout;
	char* token;

	fout = fopen("output.txt", "w");
	getcwd(cwdbuff, size);
	DIR *dir = opendir(cwdbuff);
	while((ls = readdir(dir)) !=NULL) {
		int verdict = 1;
		int i = 0;
		char noReadFiles[5][20] = {"output.txt", "lab3.c", "a.out", ".", ".."};
		for (i; i < 5; i++) {
			if(strcmp(ls->d_name, noReadFiles[i]) == 0)
				verdict = 0;
		}
		if (verdict){
			int j=0;
			fprintf(fout, "\nFile: %s\n", ls->d_name);
			for (j; j < 80; j++) {
				fprintf(fout, "-");
			}
			fprintf(fout, "\n");
			FILE* fin = fopen(ls->d_name, "r");
			while((nread = getline(&line, &len, fin) != -1)) {
					fprintf(fout, "%s\n", line); 		
			}
			fclose(fin);
		}  
		
	}
	free(token);
	free(line);
	closedir(dir);
	fclose(fout);
	return;
}

int main(int argc, char* argv[]) {
	setbuf(stdout, NULL);
        /* Main Function Variables */
        char *buffer;
        size_t strsize = 30;
        size_t userinput;
        const char delim[2] = " "; //delimeter used for tokenizing input
        const char* safeword = "exit\n"; //input that will break the loop
        char* token;
        int condition = 1;
        /* Allocate memory for the input buffer. */

        buffer = (char *) malloc (strsize * sizeof(char));

        /*main run loop*/

        while(condition) {
                int tokenCount = 0;
                /* Print >>> then get the input string */

                printf(">>> ");
                userinput = getline(&buffer, &strsize, stdin);

                /* Tokenize the input string */
                token = strtok(buffer, delim);
                /* Display each token */
                while (token != NULL) {
                        if (strcmp(token, safeword) == 0) {
                                condition = 0;
                                break;
                        }
			if (strcmp(token, "lfcat\n") == 0) {
				lfcat();
				break;
			}
                        else {
                                printf("T%d: %s\n", tokenCount, token);
                                tokenCount++;
                                token = strtok(NULL, delim);
                        }
                }
        }
	free(buffer);
	return 1;
}
