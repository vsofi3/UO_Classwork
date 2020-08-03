/*************************************

CIS330: Lab 4: Using gdb and valgrind

Instructions:

(0) Download and add this file and add to a new directory, /labs/lab4, in your class repo.

(1) Compile this program with the -g flag (e.g. `$ gcc -g lab4.c`), and run. What happens?

(3) Run it in gdb, find the issue, and fix it.

(4) Verify that your fixed program executes as expected.

(5) Use valgrind to check for memory leaks.

(6) Fix the code so that valgrind does not report any issues. You should get a message similar to the following after valgrind completes:

==XXXXX== HEAP SUMMARY:
==XXXXX==     in use at exit: 0 bytes in 0 blocks
==XXXXX==   total heap usage: 7 allocs, 7 frees, 1,264 bytes allocated
==XXXXX== 
==XXXXX== All heap blocks were freed -- no leaks are possible



Note that gdb and valgrind are both installed on ix-dev.

Here's a useful cheatsheet: http://darkdust.net/files/GDB%20Cheat%20Sheet.pdf

NOTE FOR MAC USERS:

Since you might not actually be using a GNU compiler, gdb will likely not work on your system. Instead you should use lldb which has the same functionality with a slightly different interface.
See:
  https://lldb.llvm.org/use/tutorial.html
and
  https://lldb.llvm.org/use/map.html




*************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int **num;
  int size = 5, i, j;

  // Allocate memory for 2-D int array
  num = (int **)malloc(size * sizeof(int *));
  for (i = 0; i < size; i++) {
    num[i] = (int *)malloc(size * sizeof(int *));
  }

  // Set values
  for (j = 0; j < size; j++) {
    num[i][j] = i * size + j;
  }

  // Print values
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      printf("%d ", num[i][j]);
    }
    printf("\n");
  }

  // Deallocate memory
  free(num);

  return 0;
}
