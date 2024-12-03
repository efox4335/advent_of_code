/*
 * scan through looking for string "mul(" then check if both numers are good then multiply them
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//puts chars in buff as long as they are didigts and null terminates buff
//returns the length of the int
int num_to_str(char *input, char *buff)
{
	int index = 0;

	for(index = 0; isdigit(input[index]); ++index){
		buff[index] = input[index];
	}

	buff[index] = '\0';
	return index;
}

//sets both arg_1 and arg_2 to 0 on fail
void get_nums(char *input, int *arg_1, int *arg_2)
{
	char buff[4];
	int num_len_arg_1;
	int num_len_arg_2;

	num_len_arg_1 = num_to_str(input + 4, buff);
	if(num_len_arg_1 <= 0 || num_len_arg_1 >= 4){
		goto num_error;
	}
	*arg_1 = atoi(buff);

	if(input[4 + num_len_arg_1] != ','){
		goto num_error;
	}

	num_len_arg_2 = num_to_str(input + 4 + num_len_arg_1 + 1, buff);
	if(num_len_arg_2 <= 0 || num_len_arg_2 >= 4){
		goto num_error;
	}

	if(input[4 + num_len_arg_1 + 1 + num_len_arg_2] != ')'){
		goto num_error;
	}

	*arg_2 = atoi(buff);

	return;

num_error:
	*arg_1 = 0;
	*arg_2 = 0;
}

int main(void)
{
	char find[] = "mul(";
	char *input_line = NULL;
	size_t lim = 0;
	char *mul_ptr = NULL;
	int arg_1 = 0;
	int arg_2 = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		mul_ptr = strstr(input_line, find);

		while(mul_ptr != NULL){
			get_nums(mul_ptr, &arg_1, &arg_2);
			mul_ptr = strstr(mul_ptr + 1, find);//skips past the last found "mul("
		}
	}

	free(input_line);
	return 0;
}
