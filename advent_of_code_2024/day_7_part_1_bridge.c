/*
 * for the last number in the list it can only be multiplied by a large number or add to a large number
 * the only way for it to multiply a large number is for it to be a factor of the total
 * if not the only way the total can be correct is if the last operation is '+'
 * do this for the next last number with the total being '-' or '/' depending on the operation before it
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
