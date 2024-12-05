/*
 * input rules into an array where the left value is an index in an array that contains all the right values
 * when inputing the updates mark all numbers previously visited in an array by index and compare it with the
 * right values in the rule array
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 0){
	}

	free(input_line);
	return 0;
}
