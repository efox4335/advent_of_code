/*
 * just simulate the guard till it goes off screen while counting the positions
*/
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 1){
	}

	free(input_line);
	return 0;
}
