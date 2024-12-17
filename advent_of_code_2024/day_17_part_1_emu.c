/*
 * write an emulator to emulate the cpu
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
