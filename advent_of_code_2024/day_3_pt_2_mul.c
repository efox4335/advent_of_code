/*
 * scan through looking for string "mul(" then check if both numers are good then multiply them
 * also look for the next don't() block if it is encountered skip ahead to the next do() block
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
	char do_find[] = "do()";
	char dont_find[] = "don't()";
	char *input_line = NULL;
	size_t lim = 0;
	char *mul_ptr = NULL;
	char *dont_ptr = NULL;
	int arg_1 = 0;
	int arg_2 = 0;
	int prod_sum = 0;
	int in_dont = 0;//used to check if the last line ended in a don't block

	while(getline(&input_line, &lim, stdin) > 1){
		if(in_dont == 0){
			mul_ptr = strstr(input_line, find);
			dont_ptr = strstr(input_line, dont_find);
		}else{
			mul_ptr = strstr(input_line, do_find);
			if(mul_ptr != NULL){
				dont_ptr = strstr(mul_ptr, dont_find);//if there is a don't before the next do when the last line already ended in a don't
				in_dont = 0;
			}else{//if there is no do() instruction in the line
				continue;
			}
		}

		while(mul_ptr != NULL){
			if(dont_ptr == NULL || mul_ptr < dont_ptr){
				get_nums(mul_ptr, &arg_1, &arg_2);
				prod_sum += arg_1 * arg_2;
				mul_ptr = strstr(mul_ptr + 1, find);//skips past the last found "mul("
			}else{//if mul_ptr is past the dont_ptr
				mul_ptr = strstr(dont_ptr, do_find);//can't look from mul_ptr because there might be a do instruction before the next mul instruction after the don't instruction
				if(mul_ptr == NULL){
					in_dont = 1;
				}else{
					mul_ptr = strstr(mul_ptr, find);//next mul after do instruction
					dont_ptr = strstr(mul_ptr, dont_find);
				}
			}
		}
	}

	printf("%d\n", prod_sum);

	free(input_line);
	return 0;
}
