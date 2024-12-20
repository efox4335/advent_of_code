/*
 * get the input line by line and transform it so each way a match and happen is all in one line
 * eg. diagonal matchs transformed this way make it look like a rhombus
 * 	ab
 * 	cd
 * becomes
 * 	a
 * 	cb
 * 	 d
 * and
 * 	 b
 * 	ad
 * 	c
 * and
 * 	bd
 * 	ac
 * have a fowards and backwards match so no mirror is required
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum{BUFFER_LEN = 1000};

//transforms input so that diagonal lines with slope = 1 have slope = 0
//outputs one line of transformed input based on trans_line_index
//returns the number of chars in output_line
int transform_diag_pos_slope(char input[BUFFER_LEN][BUFFER_LEN], char *output_line, int input_line_length, int trans_line_index)
{
	int row = 0;
	int col = 0;

	if(trans_line_index >= input_line_length){//if the starting point for the diagonal is on the bottom
		row = input_line_length - 1;
		col = trans_line_index - input_line_length + 1;
	}else{
		row = trans_line_index;
		col = 0;
	}

	int index = 0;

	//since input is square it can bound col
	for(;row >= 0 && col < input_line_length; --row, ++col, ++index){
		output_line[index] = input[row][col];
	}

	output_line[index] = '\0';

	return index;
}

//transforms input so that diagonal lines with slope = -1 have slope = 0
//outputs one line of transformed input based on trans_line_index
//returns the number of chars in output_line
int transform_diag_neg_slope(char input[BUFFER_LEN][BUFFER_LEN], char *output_line, int input_line_length, int trans_line_index)
{
	int row = 0;
	int col = 0;

	if(trans_line_index < input_line_length){//if the starting point for the diagonal is on the top
		row = 0;
		col = input_line_length - trans_line_index - 1;
	}else{
		row = trans_line_index - input_line_length + 1;
		col = 0;
	}

	int index = 0;

	//since input is square it can bound col
	for(;row < input_line_length && col < input_line_length; ++row, ++col, ++index){
		output_line[index] = input[row][col];
	}

	output_line[index] = '\0';

	return index;
}

//rotates 90 degrees counter clock wise
int transform_rot(char input[BUFFER_LEN][BUFFER_LEN], char *output_line, int input_line_length, int trans_line_index)
{
	int row = 0;
	int col = input_line_length - trans_line_index - 1;

	int index = 0;

	for(; row < input_line_length; ++row, ++index){
		output_line[index] = input[row][col];
	}

	output_line[index] = '\0';

	return index;
}

int main(void)
{
	int line_count = 0;

	char *input_line = NULL;
	char input[BUFFER_LEN][BUFFER_LEN];
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		strcpy(input[line_count], input_line);
		++line_count;
	}

	const char *fw_match = "XMAS";
	const char *bw_match = "SAMX";

	char trans_line[BUFFER_LEN];
	int match_count = 0;

	for(int i = 0; transform_diag_neg_slope(input, trans_line, line_count, i) > 0; ++i){
		char *temp_line_ptr = strstr(trans_line, fw_match);
		while(temp_line_ptr != NULL){
			++match_count;
			temp_line_ptr = strstr(temp_line_ptr + 1, fw_match);
		}

		temp_line_ptr = strstr(trans_line, bw_match);
		while(temp_line_ptr != NULL){
			++match_count;
			temp_line_ptr = strstr(temp_line_ptr + 1, bw_match);
		}
	}

	for(int i = 0; transform_diag_pos_slope(input, trans_line, line_count, i) > 0; ++i){
		char *temp_line_ptr = strstr(trans_line, fw_match);
		while(temp_line_ptr != NULL){
			++match_count;
			temp_line_ptr = strstr(temp_line_ptr + 1, fw_match);
		}

		temp_line_ptr = strstr(trans_line, bw_match);
		while(temp_line_ptr != NULL){
			++match_count;
			temp_line_ptr = strstr(temp_line_ptr + 1, bw_match);
		}
	}

	for(int i = 0; i < line_count; ++i){
		transform_rot(input, trans_line, line_count, i);
		char *temp_line_ptr = strstr(trans_line, fw_match);
		while(temp_line_ptr != NULL){
			++match_count;
			temp_line_ptr = strstr(temp_line_ptr + 1, fw_match);
		}

		temp_line_ptr = strstr(trans_line, bw_match);
		while(temp_line_ptr != NULL){
			++match_count;
			temp_line_ptr = strstr(temp_line_ptr + 1, bw_match);
		}
	}

	for(int i = 0; i < line_count; ++i){
		char *temp_line_ptr = strstr(input[i], fw_match);
		while(temp_line_ptr != NULL){
			++match_count;
			temp_line_ptr = strstr(temp_line_ptr + 1, fw_match);
		}

		temp_line_ptr = strstr(input[i], bw_match);
		while(temp_line_ptr != NULL){
			++match_count;
			temp_line_ptr = strstr(temp_line_ptr + 1, bw_match);
		}
	}

	printf("%d\n", match_count);

	free(input_line);
	return 0;
}
