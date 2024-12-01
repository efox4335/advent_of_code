/*
 - get input in 2 arrays
 - sort the 2 arrays then get the difference between them
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp_func(const void *arg1, const void *arg2)
{
	return *((int *) arg1) > *((int *) arg2);
}

int main(void)
{
	enum{INPUT_LINE_COUNT = 1000};

	int left_nums[INPUT_LINE_COUNT];
	int right_nums[INPUT_LINE_COUNT];
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

	qsort(left_nums, line_count, sizeof(int), cmp_func);
	qsort(right_nums, line_count, sizeof(int), cmp_func);

	free(input_line);
	return 0;
}
