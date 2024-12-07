/*
 * for the last number in the list it can only be multiplied by a large number or add to a large number
 * the only way for it to multiply a large number is for it to be a factor of the total
 * if not the only way the total can be correct is if the last operation is '+'
 * do this for the next last number with the total being '-' or '/' depending on the operation before it
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//returns 1 if the total is possible from num_arr
//when any number that could be a factor also check if it could be added to get the total
int is_possible(size_t total, size_t *num_arr, int num_count)
{
	long long int temp_total = total;

	for(int i = num_count - 1; i >= 1; --i){
		if(temp_total <= 0){
			break;
		}else if((temp_total % num_arr[i]) == 0ll){
			if(is_possible(temp_total - num_arr[i], num_arr, i)){
				return 1;
			}
			temp_total /= num_arr[i];
		}else{
			temp_total -= num_arr[i];
		}
	}

	//will always fail if temp_total <= 0 as all numbers are > 0
	if(temp_total == num_arr[0]){
		return 1;
	}

	return 0;
}

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	size_t total = 0;
	size_t input[100];
	int num_count = 0;
	size_t good_total = 0;

	char delim[] = ": \n";
	char *temp_num = NULL;

	while(getline(&input_line, &lim, stdin) > 1){
		temp_num = strtok(input_line, delim);
		total = atol(temp_num);

		num_count = 0;
		temp_num = strtok(NULL, delim);
		while(temp_num != NULL){
			input[num_count] = atol(temp_num);
			++num_count;
			temp_num = strtok(NULL, delim);
		}

		if(is_possible(total, input, num_count)){
			good_total += total;
		}
	}

	printf("%lu\n", good_total);

	free(input_line);
	return 0;
}
