/*
* Description: <This lab takes input from the command line and returns
		each individual command given. Once the user enters "exit"
		the program stops. >
*
* Author: <Owen McEvoy>
*
* Date: <October 6th, 2019>
*
* Notes: 
* 1. <I give credit to the links posted on the lab description for showing
      me how to use the getlines and strtok functions. >
*/

/*-------------------------Preprocessor Directives---------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*---------------------------------------------------------------------------*/

/*-----------------------------Program Main----------------------------------*/
int main() {
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
			else {
				printf("T%d: %s\n", tokenCount, token);
				tokenCount++;
				token = strtok(NULL, delim);
			}
		}	
	}	

	/*Free the allocated memory*/
	free(buffer);
	return 1;
}
/*-----------------------------Program End-----------------------------------*/
