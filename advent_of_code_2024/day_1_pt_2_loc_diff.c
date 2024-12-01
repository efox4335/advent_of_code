/*
 - get input
 - put left numbers on array
 - put right numbers in a hash table with the number as the key and the number of occurrences being the data
 - sum the left number array with each element multiplied by its data in the hash table(*0 for no entry)
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
	char *save_ptr = NULL;
	char *val = NULL;
	int line_count = 0;

	char *input_line = NULL;
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		val = strtok_r(input_line, " \n", &save_ptr);
		left_nums[line_count] = atoi(val);

		val = strtok_r(NULL, " \n", &save_ptr);

		++line_count;
	}

	qsort(left_nums, line_count, sizeof(int), cmp_func);

	free(input_line);
	return 0;
}
