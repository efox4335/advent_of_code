/*
 * the stones can be tracked individually
 * a hash table can be kept to keep track of how many stones are prodused by an input number and a number of blinks
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	free(input_line);
	return 0;
}
