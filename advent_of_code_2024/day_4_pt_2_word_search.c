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
 * have a fowards and backwards match so no mirror is required
 * only match in diagonal store where the a is in the match in a hash table then when matching for the other diagonal if there is an entry for that match that is an x
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/edsa.h"

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

typedef struct{
	int row;
	int col;
}cords;

//transforms a set of cords for an element in pos slope diagonal transformation to untransformed input
cords pos_slope_trans_to_norm(cords diag_cords, int line_count)
{
	cords norm;

	if(diag_cords.row >= line_count){
		norm.row = diag_cords.row - (diag_cords.col + diag_cords.row - line_count + 1);
		norm.col = diag_cords.col + diag_cords.row - line_count + 1;
	}else{
		norm.row = diag_cords.row - diag_cords.col;//every col to the right gets shifed down one row
		norm.col = diag_cords.col;
	}

	return norm;
}

//transforms a set of cords for an element in neg slope diagonal transformation to untransformed input
cords neg_slope_trans_to_norm(cords diag_cords, int line_count)
{
	cords norm;

	if(diag_cords.row < line_count){
		norm.row = diag_cords.col;
		norm.col = diag_cords.col + (line_count - 1 - diag_cords.row);
	}else{
		norm.row = diag_cords.row - (line_count - 1 - diag_cords.col);
		norm.col = diag_cords.col;
	}

	return norm;
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

	const char *fw_match = "MAS";
	const char *bw_match = "SAM";

	char trans_line[BUFFER_LEN];
	int xxmas_count = 0;

	//size_t ret_val = 0;
	edsa_htable *table = NULL;
	edsa_htable_init(&table, sizeof(cords), sizeof(char), 100);

	for(int i = 0; transform_diag_neg_slope(input, trans_line, line_count, i) > 0; ++i){
		cords norm;
		cords diag;
		char found = 1;
		char *temp_line_ptr = strstr(trans_line, fw_match);
		while(temp_line_ptr != NULL){
			diag.row = i;
			diag.col = (temp_line_ptr - trans_line) + 1;//one added so it centers on the 'a'
			norm = neg_slope_trans_to_norm(diag, line_count);
			edsa_htable_ins(table, &norm, &found);
			temp_line_ptr = strstr(temp_line_ptr + 1, fw_match);
		}

		temp_line_ptr = strstr(trans_line, bw_match);
		while(temp_line_ptr != NULL){
			diag.row = i;
			diag.col = (temp_line_ptr - trans_line) + 1;//one added so it centers on the 'a'
			norm = neg_slope_trans_to_norm(diag, line_count);
			edsa_htable_ins(table, &norm, &found);
			temp_line_ptr = strstr(temp_line_ptr + 1, bw_match);
		}
	}

	for(int i = 0; transform_diag_pos_slope(input, trans_line, line_count, i) > 0; ++i){
		cords norm;
		cords diag;
		char ret = 0;
		char *temp_line_ptr = strstr(trans_line, fw_match);
		while(temp_line_ptr != NULL){
			diag.row = i;
			diag.col = (temp_line_ptr - trans_line) + 1;//one added so it centers on the 'a'
			norm = pos_slope_trans_to_norm(diag, line_count);
			if(edsa_htable_read(table, &norm, &ret) == EDSA_SUCCESS){//if there is already an entry for that a
				++xxmas_count;
			}
			temp_line_ptr = strstr(temp_line_ptr + 1, fw_match);
		}

		temp_line_ptr = strstr(trans_line, bw_match);
		while(temp_line_ptr != NULL){
			diag.row = i;
			diag.col = (temp_line_ptr - trans_line) + 1;//one added so it centers on the 'a'
			norm = pos_slope_trans_to_norm(diag, line_count);
			if(edsa_htable_read(table, &norm, &ret) == EDSA_SUCCESS){//if there is already an entry for that a
				++xxmas_count;
			}
			temp_line_ptr = strstr(temp_line_ptr + 1, bw_match);
		}
	}

	printf("%d\n", xxmas_count);

	free(input_line);
	return 0;
}
