/*
 * the stones can be tracked individually
 * a hash table can be kept to keep track of how many stones are prodused by an input number and a number of blinks
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/edsa.h"

typedef struct{
	long num;
	int blink_count;
}args;

int base_10_digit_count(long input)
{
	int digit_count = 1;
	long power_of_10s = 10;
	while(input >= power_of_10s){
		power_of_10s *= 10;
		++digit_count;
	}

	return digit_count;
}

long get_stone_count_after_blinks(long input, int blink_count, edsa_htable *cashe)
{
	if(blink_count == 0){
		return 1;
	}

	long ret_val = 0;
	args arguments;
	arguments.num = input;
	arguments.blink_count = blink_count;

	if(edsa_htable_read(cashe, &arguments, &ret_val) == EDSA_SUCCESS){
		return ret_val;
	}

	if(input == 0){
		ret_val = get_stone_count_after_blinks(1, blink_count - 1, cashe);
		edsa_htable_ins(cashe, &arguments, &ret_val);
		return ret_val;
	}

	int digit_count = base_10_digit_count(input);
	if(digit_count & 1){
		ret_val = get_stone_count_after_blinks(input * 2024L, blink_count - 1, cashe);
		edsa_htable_ins(cashe, &arguments, &ret_val);
		return ret_val;
	}else{
		long split = 1;
		digit_count >>= 1;
		for(int i = 0; i < digit_count; ++i){
			split *= 10;
		}

		ret_val = get_stone_count_after_blinks(input / split, blink_count - 1, cashe);
		ret_val += get_stone_count_after_blinks(input % split, blink_count - 1, cashe);
		edsa_htable_ins(cashe, &arguments, &ret_val);
		return ret_val;
	}
}

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	long input_arr[10];
	int num_count = 0;
	char delim[] = " \n";

	while(getline(&input_line, &lim, stdin) > 1){
		char *temp_num = NULL;

		temp_num = strtok(input_line, delim);

		while(temp_num != NULL){
			input_arr[num_count] = atol(temp_num);
			++num_count;
			temp_num = strtok(NULL, delim);
		}
	}

	edsa_htable *cashe = NULL;
	edsa_htable_init(&cashe, sizeof(args), sizeof(long), 20000);

	long stone_count = 0;
	for(int i = 0; i < num_count; ++i){
		stone_count += get_stone_count_after_blinks(input_arr[i], 75, cashe);
	}

	printf("%ld\n", stone_count);

	edsa_htable_free(cashe);
	free(input_line);
	return 0;
}
