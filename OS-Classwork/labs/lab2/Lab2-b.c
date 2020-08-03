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
/*---------------------------------------------------------------------------*/

/*-----------------------------Program Main----------------------------------*/
int main(int argc, char* argv[]) {
	setbuf(stdout, NULL);
	
	/* Main Function Variables */
	FILE* fin;	//input file
	FILE* fout;	//output file

	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	const char delim[2] = " "; //delimeter used for tokenizing input
	char* token; 
	
	if (argc != 2) {
		printf("Requires 2 arguments");
		exit(EXIT_FAILURE);
	}	

	fin = fopen(argv[1], "r"); // opens input file
 
	if (fin == NULL) {
		exit(EXIT_FAILURE);
	}
	
	fout = fopen("output.txt", "w"); //opens/creates output file

	while((nread = getline(&line, &len, fin) != -1)) {
		int tokenCount = 0; 	
		
		token = strtok(line, delim);
		
		while (token != NULL) {
			fprintf(fout, "T%d: %s\n", tokenCount, token);
			tokenCount++;
			token = strtok(NULL, delim);
		}
	} 	
	free(line);	
	fclose(fin);
	fclose(fout); 
	
	return 1;
}
