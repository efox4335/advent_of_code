/*
 - get input in 2 arrays
 - sort the 2 arrays then get the difference between them
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	int left_nums[1000];
	int right_nums[1000];
	char *save_ptr = NULL;
	char *val = NULL;
	int line_count = 0;

	char *input_line = NULL;
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		val = strtok_r(input_line, " \n", &save_ptr);
		left_nums[line_count] = atoi(val);

		val = strtok_r(NULL, " \n", &save_ptr);
		right_nums[line_count] = atoi(val);

		++line_count;
	}

	free(input_line);
	return 0;
}
