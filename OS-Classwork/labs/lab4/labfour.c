#include <stdio.h>
#include <stdlib.h>

int main(){
	int i = 1;
	for (i; i < 11; ++i) {
		printf(" %d", i);
	}
	printf("\ndone counting\n");
	return 1;
}
