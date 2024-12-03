/*
 * scan through looking for string "mul(" then check if both numers are good then multiply them
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	char find[] = "mul(";
	char *input_line = NULL;
	size_t lim = 0;
	char *mul_ptr = NULL;

	while(getline(&input_line, &lim, stdin) > 1){
		mul_ptr = strstr(input_line, find);

		while(mul_ptr != NULL){
			mul_ptr = strstr(mul_ptr + 1, find);//skips past the last found "mul("
		}
	}

	free(input_line);
	return 0;
}
