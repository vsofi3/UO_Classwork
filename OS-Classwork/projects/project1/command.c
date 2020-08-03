#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <dirent.h>
#include <fcntl.h>
void listDir() {
	char cwd[256];
	struct dirent *ls;
	getcwd(cwd, sizeof(cwd));
	DIR *dr = opendir(cwd);
	char megaString[1024];
	char* ms = megaString;
	while ((ls = readdir(dr)) != NULL) {
		ms = strcat(ms, ls->d_name);
		ms = strcat(ms, " ");
	}
	ms = strcat(ms, "\n");
	write(1, ms, strlen(ms));
	ms[0] = '\0';
	closedir(dr);
	return;
}

void showCurrentDir() {
	char cwd[256];
	getcwd(cwd, sizeof(cwd));
	char* cw = cwd;
	cw = strcat(cw, "\n");
	write(1, cwd, strlen(cwd));
	cw[0] = '\0';
	return;
}

void makeDir(char* dirName) {
	char* err = "Error! Does directory already exist?\n";
	if (mkdir(dirName, S_IRWXU) != 0) {
		write(1, err, strlen(err));
	}
	return;
}

void changeDir(char* dirName) {
	char* err = "Error! Failed to move to directory\n";
	if (chdir(dirName) != 0) {
		write(1, err, strlen(err));
	} 
	return;
}

void copyFile(char* sourceFile, char* destinationPath){
	ssize_t size;
	char buffer[1024];
	int file1, file2;
	file1 = open(sourceFile, O_RDONLY );
	file2 = open(destinationPath, O_WRONLY | O_CREAT | S_IRWXU);
	char* err1 = "Error! Unable to open source file.\n";
	char* err2 = "Error! Destination may already exist.\n";
	if (file1 == -1){
		write(1, err1, strlen(err1));
		return;
	}
	if (file2 == -1) {
		close(file1);
		write(1, err2, strlen(err2));
		return;	
	} 
	while ((size = read(file1, buffer, sizeof(buffer))) != 0) {
		write(file2, buffer, size);
	}
	close(file1);
	chmod(destinationPath, S_IRWXU);
	close(file2);
	return;
}

void deleteFile(char* filename) {
	char* err = "Error! Unable to delete file.\n";
	if (unlink(filename) != 0){
		write(1, err, strlen(err));
	}
	return;
}
void displayFile(char* filename) {
	ssize_t size;
	char buffer[1024];
	char* err = "Error! Unable to open file.\n";
	int file1;
	file1 = open(filename, O_RDONLY);
	if (file1 == -1){
		write(1, err, strlen(err));
		return;
	}
	while ((size = read(file1, buffer, sizeof(buffer))) != 0) {
		write(1, buffer, strlen(buffer));
	}
	close(file1);
	return;
}

void moveFile(char* sourcePath, char* destinationPath) {
	int verdict;
	char* err = "Error! Unable to move file\n";
	verdict = rename(sourcePath, destinationPath);
	if (!verdict ) {
		return;
	}
	else{
		write(1, err, strlen(err));
	} 
	return;
}

int isCommand(char* cmd){
	char commands[8][6] = {"ls", "pwd", "mkdir", "cd", "cp", "mv", "rm", "cat"};
	int i = 0;
	for (i; i < 8; i++) {
		if (strcmp(cmd, commands[i]) == 0) {
			return 1;
		}
	}
	return 0;		
}
int isControl(char* cmd){
	if(strcmp(cmd, ";") == 0 ) {
		return 1;
	}
	if(strcmp(cmd, "\n") == 0) {
		return 1;
	}
	return 0;
}

int commandMaster(char* cmds[], int cap) {
	int j = 0;
	char* errc = "Error! Incorrect syntax. No control code found.\n";
	char* arg = "Error! Command requires an argument. 0 given.\n";
	char* arg0 = "Error! Command requires 2 arguments. 0 given.\n";
	char* arg1 = "Error! Command requires 2 arguments. 1 given.\n";
	char* inc = "Error! Incorrect argument provided. Cannot be command or a control.\n";
	char* unrec = "Error! Unrecognized command.\n";
	for(j; j < cap; j++){
		if(isCommand(cmds[j])) {
			// 2 cases below with no arguments
			if (strcmp(cmds[j], "ls") == 0) {
				j++;		//if cmd has no argument, next cmd item must be control
				if(j == cap || isControl(cmds[j])){
					listDir();
					if (j == cap)
						return 1;
				}
				else {
					write(1, errc, strlen(errc));
					return 1;	
				}
			}
			if (strcmp(cmds[j], "pwd") == 0) {
				j++;		//if cmd has no argument, next cmd item must be control
				if(j == cap || isControl(cmds[j])){
					showCurrentDir();
					if (j == cap)
						return 1;
				}
				else {
					write(1, errc, strlen(errc));
					return 1;	
				}
			} //4 cases below that have 1 argument
			if (strcmp(cmds[j], "mkdir") == 0) {
				j++;
				if (j == cap) { //no argument provided
					write(1, arg, strlen(arg));		
					return 1;
				}
				if(!isCommand(cmds[j]) && !isControl(cmds[j])) {
					if( (j+1 == cap) || isControl(cmds[j+1])) {
						makeDir(cmds[j]);
						j++;
						if (j == cap)
							return 1;
					}
					else { // no control statement following the command
						write(1, errc, strlen(errc));
						return 1;
					}
				}
				else {
					write(1, inc, strlen(inc));
					return 1;
				} 
			}
			if (strcmp(cmds[j], "cd") == 0) {
				j++;
				if (j == cap) { //no argument provided
					write(1, arg, strlen(arg));
					return 1;
				}
				if(!isCommand(cmds[j]) && !isControl(cmds[j])) {
					if( (j+1 == cap) || isControl(cmds[j+1])) {
						changeDir(cmds[j]);
						j++;
						if (j == cap)
							return 1;
					}
					else { // no control statement following the command
						write(1, errc, strlen(errc));
						return 1;
					}
				}
				else {
					write(1, inc, strlen(inc));
					return 1;
				} 
			}

			if (strcmp(cmds[j], "rm") == 0) {
				j++;
				if (j == cap) { //no argument provided
					write(1, arg, strlen(arg));
					return 1;
				}
				if(!isCommand(cmds[j]) && !isControl(cmds[j])) {
					if( (j+1 == cap) || isControl(cmds[j+1])) {
						deleteFile(cmds[j]);
						j++;
						if (j == cap)
							return 1;
					}
					else { // no control statement following the command
						write(1, errc, strlen(errc));
						return 1;
					}
				}
				else {
					write(1, inc, strlen(inc));
					return 1;
				} 
			}
			if (strcmp(cmds[j], "cat") == 0) {
				j++;
				if (j == cap) { //no argument provided
					write(1, arg, strlen(arg));
					return 1;
				}
				if(!isCommand(cmds[j]) && !isControl(cmds[j])) {
					if( (j+1 == cap) || isControl(cmds[j+1])) {
						displayFile(cmds[j]);
						j++;
						if (j == cap)
							return 1;
					}
					else { // no control statement following the command
						write(1, errc, strlen(errc));
						return 1;
					}
				}
				else {
					write(1, inc, strlen(inc));
					return 1;
				} 
			}

			if (strcmp(cmds[j], "cp") == 0) {
				j++;
				if (j == cap) {
					write(1, arg0, strlen(arg0));
					return 1;
				}
				if (!isCommand(cmds[j]) && !isControl(cmds[j])){
					int i = j;
					j++; //now check for 2nd argument
					if (j == cap) {
						write(1, arg1, strlen(arg1));
						return 1;
					}
					if (!isCommand(cmds[j]) && !isControl(cmds[j])) {
						if ( (j+1 == cap) || isControl(cmds[j+1])) {
							copyFile(cmds[i], cmds[j]);
							j++;
							if (j == cap)
								return 1;
						}	
						else {
							write(1, errc, strlen(errc));
							return 1;
						}
					} 
					else {
						write(1, inc, strlen(inc));
						return 1;
					}
				}
			}
			if (strcmp(cmds[j], "mv") == 0) {
				j++;
				if (j == cap) {
					write(1, arg0, strlen(arg0));
					return 1;
				}
				if (!isCommand(cmds[j]) && !isControl(cmds[j])){
					int i = j;
					j++; //now check for 2nd argument
					if (j == cap) {
						write(1, arg1, strlen(arg1));
						return 1;
					}
					if (!isCommand(cmds[j]) && !isControl(cmds[j])) {
						if ( (j+1 == cap) || isControl(cmds[j+1])) {
							moveFile(cmds[i], cmds[j]);
							j++;
							if (j == cap)
								return 1;
						}	
						else {
							write(1, errc, strlen(errc));
							return 1;
						}
					} 
					else {
						write(1, inc, strlen(inc));
						return 1;
					}
				}
			}
		}			
		if (strcmp(cmds[j], "exit") == 0) {
			return 0;
		}
		if (isControl(cmds[j]) && j != cap - 1){
			continue;
		}	
		else{
			write(1, unrec, strlen(unrec));
			return 1;
		}
	}
	return 1;
}
