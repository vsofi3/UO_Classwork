/* Project 2 Header File */

void signaler(pid_t pid[], int signum, int numProgs); //signaler function used in part2 & part3

int ultimatum(); 	//function determines if all children have exited. Used in part3

void alrm_hndlr();	//handler for alarm signal. Used in part3
