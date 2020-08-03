#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_NAME "test.dat"
#define uint32_t u_int32_t

int  arg_test(int argc, char **argv);
void conversion_test();
void load_data(int **int_array, uint32_t *array_size);
int  find_nth(int *int_array, uint32_t array_size, int n);
void print_array(uint32_t size, int *arr);
int cmpfunc (const void *a, const void *b);

int main(int argc, char **argv)
{
    int valid = arg_test(argc, argv);
    // Test function for atoi
    conversion_test();
    
    int nth = -3;
 
    if(valid) {
        int *int_array = NULL;
        uint32_t array_size = 0;
        load_data(&int_array, &array_size);

        // This function prints the content of the array - debugging tool
        print_array(array_size, int_array);

        // Insert your code here (1)
        //int n = -1;
	int n = atoi(argv[1]);

        // ------------------------

        nth = find_nth(int_array, array_size, n);
        if(nth >= 0) {
            printf("---- Answer ----\n");
            printf("The nth value is %d\n", int_array[nth]);
            printf("--------\n");
        } else if(nth == -1) {
            printf("---- Answer ----\n");
            printf("n is too large!\n");
            printf("--------\n");
        } else if(nth == -2) {
            printf("---- Answer ----\n");
            printf("Some error!\n");
            printf("--------\n");
        } else {
            // Do nothing
        }
    } else {
        // Do nothing
    }
    
    return 0;
}



int arg_test(int argc, char **argv)
{
    int return_val = 0;
    if(argc < 2) {
        fprintf(stderr, "Error: no input entered\n");
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        fprintf(stderr, "\n");
    } else if(argc > 2) {
        fprintf(stderr, "Error: too many inputs\n");
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        fprintf(stderr, "\n");
    } else {
        return_val = 1;
    }
    return return_val;
}

void conversion_test()
{
    printf("---- ATOI Sample Usage ----\n");
    char *num_str = "101";
    int num = atoi(num_str);
    printf("The number is %d\n", num);
    printf("--------\n\n");
}

void load_data(int **int_array, uint32_t *array_size)
{
    FILE *fp = NULL;
    fp = fopen(FILE_NAME, "r");
    if(fp == NULL) {
        fprintf(stderr, "Error while loading the file\n");
        exit(EXIT_FAILURE);
    }

    int cnt = 0;
    int tmp = 0;
    while (fscanf(fp, "%d", &tmp) == 1) {
        cnt++;
    }
    fclose(fp);

    int *tmp_array = (int*) malloc(sizeof(int) * cnt);
    fp = fopen(FILE_NAME, "r");
    if(fp == NULL) {
        fprintf(stderr, "Error while loading the file\n");
        exit(EXIT_FAILURE);
    }
    cnt = 0;
    tmp = 0;
    while (fscanf(fp, "%d", &tmp) == 1) {
        tmp_array[cnt] = tmp;
        cnt++;
    }
    fclose(fp);
    
    *int_array = tmp_array;
    *array_size = cnt;
}

int cmpfunc (const void *a, const void *b) // taken from anyexample.com
{
	const int *ia = (const int *) a;
	const int *ib = (const int *) b;
	return *ia - *ib; 
}
// Given n as input, find the nth largest value
// in the list of integers loaded from the file.
// If n is larger than the number of integers,
// return -1.
// Return -2 for any other errors.
// NOTE 1:
// The file used for grading will be different from
// the file given with the homework - it will have
// different number of integers and different integer
// values

int find_nth(int *int_array, uint32_t array_size, int n)
{
    // Insert your code here (2)
	printf("N is %d\n", n);
	printf("size is %d\n", array_size);
	if (n > array_size)			//checks to see if n is greater than array size 
	{
		return -1;
	}
	else if (n <= 0)			// makes sure n is a valid input
	{
		return -2;
	}

	qsort(int_array, array_size, sizeof(int), cmpfunc);	//sorts array 
								//counting from right to left

	int numCheck = 0;
	int properIndex = 0;
	for (int i = array_size - 1; i >= 0; i--)
	{	
		int j = i - 1;
		if (int_array[i] != int_array[j])
		{
			properIndex += 1; 	// every time it encounters a new number
			numCheck +=1;		// update properIndex
		}
		
		if (properIndex == n)		// once we have reached the nth unique
		{				// number, return the index of int_array
			return (i); 
		}
	}
	if (numCheck < n)
	{
		return -2;			// if n is greater than the number of unique numbers in the list
						// return -2
	}
	return -2;
    // ------------------------

}

void print_array(uint32_t size, int *arr)
{
   printf("---- Print Array ----\n");
    printf("This file has %d elements\n", size);
    printf("#\tValue\n");
    for(int i = 0; i < size; i++) {
        printf("%d\t%d\n", i, arr[i]);
    }
   printf("--------\n\n");
}

