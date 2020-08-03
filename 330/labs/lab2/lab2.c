/*************************************************************

CIS330: Lab 2
Operations on a 2-D array of strings

Instructions:

(1) Read, understand, build, and test the code in this file.

(2) Complete the 'capitalize_first_col' function.

(3) Add code in 'main' to test the 'capitalize_first_col' function and print the result.
    Hint: re-use the call to 'apply_by_row' with different arguments.

(4) Complete the 'apply_by_col' function.

(5) Add code to main to test 'apply_by_col'.
    Hint: adapt the original calls to 'apply_by_row'

(6) Add a comment AFTER your 'apply_by_col' that addresses the following question:
    "Which is more efficient: apply_by_row or apply_by_col?"

Rubric (out of 10 points):

5 points for (2) and (3)
5 points for (4) and (5)

up to 2 points extra credit for really good answers to (6) or efficient implementations of 'apply_by_col'.

**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * Static data used to provide some values: feel free to changes these words if you like
 */
#define STATIC_DATA_LENGTH 6
char *static_data[STATIC_DATA_LENGTH] = {
  "this",
  "is",
  "a",
  "list",
  "of",
  "words"
};

int allocate_data(char ****data, int rows, int cols);
void free_data(char ***data, int rows, int cols);

void apply_by_row(void (*func)(char *, int, int, int, int), char ***data, int rows, int cols);
void capitalize_first_col(char *in_str, int row, int col, int rows, int cols);
void apply_by_col(void (*func)(char *, int, int, int, int), char ***data, int rows, int cols);
void print_str(char *in_str, int row, int col, int rows, int cols);

int
main(int argc, char *argv[])
{
  char ***data;
  int data_rows = 4;
  int data_cols = 3;
  int rc;

  rc = allocate_data(&data, data_rows, data_cols);
  if (rc != 0) {
    return rc;
  } 
  
  printf("\nOriginal matrix:\n");
  apply_by_row(print_str, data, data_rows, data_cols);

  // TODO: add your test calls for parts (3) and (5) here

 	apply_by_row(capitalize_first_col, data, data_rows, data_cols);	
	printf("------Capitalized First Column ------ \n\n");
  	apply_by_row(print_str, data, data_rows, data_cols);
	
	free_data(data, data_rows, data_cols);
 	
	rc = allocate_data(&data, data_rows, data_cols);
 	if (rc != 0) 
	{
    		return rc;
 	} 
  	
	printf("------ Apply by column test ------ \n\n");
	apply_by_col(print_str, data, data_rows, data_cols);
 	apply_by_col(capitalize_first_col, data, data_rows, data_cols);	
	printf("------Capitalized First Column ------ \n\n");
	apply_by_col(print_str, data, data_rows, data_cols);

	free_data(data, data_rows, data_cols);
	return 0;
}

/*
 * Allocates a two dimensional array of strings
 * initialized with values from 'static_data'.
 *
 * Returns non-zero if anything went wrong.
 *
 */
int
allocate_data(char ****data, int rows, int cols)
{
  int i, j;
  *data = (char ***)malloc(sizeof(char **) * rows);
  if (*data == NULL) {
    fprintf(stderr, "Out of memory!\n");
    return -1;
  }
  for (i = 0; i < rows; i++) {
    (*data)[i] = (char **)malloc(sizeof(char *) * cols);
    if ((*data)[i] == NULL) {
      fprintf(stderr, "Out of memory!\n");
      return -1;
    }
    for (j = 0; j < cols; j++) {
      (*data)[i][j] = strdup(static_data[(i * cols + j) % STATIC_DATA_LENGTH]);
    }
  }
  return 0;
}


/*
 * Free memory held by two dimentional array of strings.
 *
 */
void
free_data(char ***data, int rows, int cols)
{
  int i, j;
  for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
        free(data[i][j]);
      }
    free(data[i]);
  }
  free(data);
}


/*
 * Applies the given function to each entry in a two-dimensional array.
 * Operates one row at a time.
 *
 * The arguments to the applied function are as follows:
 *   1) the current entry;
 *   2) the row number of the current entry;
 *   3) the column number of the current entry;
 *   4) the total number of rows;
 *   5) the total number of columns.
 */
void
apply_by_row(void (*func)(char *, int, int, int, int),
             char ***data,
             int rows, int cols)
{
  int i,j;
  for (i = 0; i < rows; i++)
  {
    for (j = 0; j < cols; j++)
    {
      func(data[i][j], i, j, rows, cols);
    }
  }
}

/*
 * Functions to be applied to a two dimentional array:
 * prints the array's contents, adding new lines after each row.
 */
void
print_str(char *in_str, int row, int col, int rows, int cols)
{
  // printf("[%d][%d]: \"%s\"\n", row, col, in_str);
  printf("%s ", in_str);
  if ((col + 1) % cols == 0) {
    printf("\n");
  }
}

/*
 * Functions to be applied to a two dimentional array:
 * changes all characters in all strings in the first column to be uppercase.
 */
void
capitalize_first_col(char *in_str, int row, int col, int rows, int cols)
{
	int i;
	int length = strlen(in_str);
	for(i = 0; i < length; i++)
	{
		if (col == 0)
		{
			in_str[i] = toupper(in_str[i]);
		}
	}	
}

/*
 * Applies the given function to each entry in a two-dimensional array.
 * Operates one column at a time.
 *
 * The arguments to the applied function are as follows:
 *   1) the current entry;
 *   2) the row number of the current entry;
 *   3) the column number of the current entry;
 *   4) the total number of rows;
 *   5) the total number of columns.
 */
void
apply_by_col(void (*func)(char *, int, int, int, int),
             char ***data,
             int rows, int cols)
{
  // TODO: add your code for part (4) here
{
  int i,j;
  for (i = 0; i < cols; i++)
  {
    for (j = 0; j < rows; j++)
    {
      func(data[j][i], i, j, cols, rows);
    }
  }
}
}

/******* TODO: add you discussion for part (6) here ***************************

Apply by row is more efficient than apply by col because of the fact that the data is far more accessible 
due to temporal locality. In apply by col, everytime to iterate you have to jump to the next row instead of the 
next column. The next column, assuming int type, is 8 bits away from the current cell. If you need to jump to the 
next row, you have to jump 8 * (number of columns) to get to the appropriate cell.   


*******************************************************************************/
