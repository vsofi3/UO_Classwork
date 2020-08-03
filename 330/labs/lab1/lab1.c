/*************************************

CIS330: Lab 1

(1) Setup work environment (ix-dev, VM, etc.)
  minimum requirements: gcc, git


The following steps should be completed at the command line:

(2) Clone class repo in work environment
  use the link(s) given by bitbucket, choose ssh or html

(3) Create the following directory structure:
  <your-repo-name>/labs/lab1

(4) Download this file into the new lab1 directory
  use wget or curl

(5) Compile and run this file to make sure everything works
  use gcc

(6) Add this file to repo, commit, push

(7) Make another clone of your repo
  add "<your-repo-name>_copy" after the original "git clone ..." command

(8) In this copy, modify lab1.c to also compute and report the mean (average) of the numbers.

(9) Compile, run, verify that it works

(10) Add, commit, push

(11) Delete this copy of the repo
  e.g. rm -r <your-repo-name>_copy

(12) Is your work lost? If not, how can you get the updated version in your original clone?

If there's extra time:

(13) Add min, max features using branches



*************************************/

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
  int i;
  int sum;

  // Account for argv[0] which is the command name
  argc--;
  argv++;

  for (i = 0; i < argc; i++) {
    sum += atoi(argv[i]);
  }
  
  printf("The sum is %d\n", sum);
  
  return 0;
}
