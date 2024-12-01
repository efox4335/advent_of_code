/*
 - get input
 - put left numbers on array
 - put right numbers in a hash table with the number as the key and the number of occurrences being the data
 - sum the left number array with each element multiplied by its data in the hash table(*0 for no entry)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/edsa.h"

int main(void)
{
	enum{INPUT_LINE_COUNT = 1000};

	int left_nums[INPUT_LINE_COUNT];
	char *save_ptr = NULL;
	char *val = NULL;
	int line_count = 0;

	char *input_line = NULL;
	size_t lim = 0;

	edsa_htable *hash_table = NULL;
	edsa_htable_init(&hash_table, sizeof(int), sizeof(int), 1000);

	while(getline(&input_line, &lim, stdin) > 1){
		val = strtok_r(input_line, " \n", &save_ptr);
		left_nums[line_count] = atoi(val);

		val = strtok_r(NULL, " \n", &save_ptr);
		int key_buff = atoi(val);
		int data_buff = 0;
		size_t ret_val = 0;
		ret_val = edsa_htable_read(hash_table, &key_buff, &data_buff);//checks if there is already an entry for key_buff
		if(ret_val == EDSA_HTABLE_READ_NO_ENTRY){
			data_buff = 1;
		}else{
			++data_buff;
		}

		edsa_htable_ins(hash_table, &key_buff, &data_buff);

		++line_count;
	}

	int sum = 0;

	for(int i = 0; i < line_count; ++i){
		int count = 0;
		size_t ret_val = 0;

		ret_val = edsa_htable_read(hash_table, &(left_nums[i]), &count);

		if(ret_val == EDSA_HTABLE_READ_NO_ENTRY){//equivalent to multpilying by 0 for number that don't appear in right nums
			continue;
		}

		sum += (left_nums[i] * count);
	}

	printf("%d\n", sum);

	edsa_htable_free(hash_table);
	free(input_line);
	return 0;
}
