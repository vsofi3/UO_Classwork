#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int **num;
	int size = 5;

	// Allocate memory for 2-D int array
	num = (int **)malloc(size * sizeof(int *));
	for (int i = 0; i < size; i++){
		num[i] = (int *)malloc(size * sizeof(int *)); 
	}
	// Set values
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			num[i][j] = i * size + j; //error was because i and j were not explicitly defined. 
		}
	}

	// Print values
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			printf("%d ", num[i][j]);
		}
		printf("\n");
	}
	
	//Deallocate memory
	for (int i = 0; i < size; i++){  // memory leak was occurring because the memory needs to be freed by row
		free(num[i]);		// instead of all at once 
	}
	free(num);
	return 0;
}	
