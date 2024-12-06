/*
 * just simulate the guard till it goes off screen while brute forcing all possable positions
 * a cycle happens when the guard goes in the same position and direction
 * store previous positions in a hash table
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/edsa.h"

enum{INPUT_BUFF_SIZE = 200};

typedef struct{
	int row;
	int col;
}cords;

typedef struct{
	cords guard_pos;
	char dir;
}guard_state;

//returns the column the guard is found in
int find_guard(char *line)
{
	for(int i = 0; line[i] != '\n'; ++i){
		if(line[i] == '^'){
			return i;
		}
	}

	return -1;
}

cords move_guard(char input[INPUT_BUFF_SIZE][INPUT_BUFF_SIZE], cords guard_pos)
{
	switch(input[guard_pos.row][guard_pos.col]){
		case '^':
			if(input[guard_pos.row - 1][guard_pos.col] != '.'){
				input[guard_pos.row][guard_pos.col] = '>';
				return guard_pos;
			}else{
				input[guard_pos.row][guard_pos.col] = '.';
				input[guard_pos.row - 1][guard_pos.col] = '^';
				guard_pos.row -= 1;
				return guard_pos;
			}
		case '>':
			if(input[guard_pos.row][guard_pos.col + 1] != '.'){
				input[guard_pos.row][guard_pos.col] = 'v';
				return guard_pos;
			}else{
				input[guard_pos.row][guard_pos.col] = '.';
				input[guard_pos.row][guard_pos.col + 1] = '>';
				guard_pos.col += 1;
				return guard_pos;
			}
		case '<':
			if(input[guard_pos.row][guard_pos.col - 1] != '.'){
				input[guard_pos.row][guard_pos.col] = '^';
				return guard_pos;
			}else{
				input[guard_pos.row][guard_pos.col] = '.';
				input[guard_pos.row][guard_pos.col - 1] = '<';
				guard_pos.col -= 1;
				return guard_pos;
			}
		case 'v':
			if(input[guard_pos.row + 1][guard_pos.col] != '.'){
				input[guard_pos.row][guard_pos.col] = '<';
				return guard_pos;
			}else{
				input[guard_pos.row][guard_pos.col] = '.';
				input[guard_pos.row + 1][guard_pos.col] = 'v';
				guard_pos.row += 1;
				return guard_pos;
			}
	}

	cords bad_val = {-1, -1};
	return bad_val;
}

void clear_prev_pos (char prev_pos[INPUT_BUFF_SIZE][INPUT_BUFF_SIZE][128])
{
	for(int i = 0; i < INPUT_BUFF_SIZE; ++i){
		for(int k = 0; k < INPUT_BUFF_SIZE; ++k){
			for(int j = 0; j < 128; ++j){
				prev_pos[i][k][j] = 0;
			}
		}
	}
}

int main(void)
{
	enum{FOUND, NOT_FOUND};

	char *input_line = NULL;
	size_t lim = 0;

	char input[INPUT_BUFF_SIZE][INPUT_BUFF_SIZE];
	int line_count = 0;
	cords guard_pos;
	int is_guard_found = NOT_FOUND;

	while(getline(&input_line, &lim, stdin) > 1){
		strcpy(input[line_count], input_line);
		if(is_guard_found == NOT_FOUND){
			guard_pos.col = find_guard(input_line);
			if(guard_pos.col != -1){
				is_guard_found = FOUND;
				guard_pos.row = line_count;
			}
		}
		++line_count;
	}

	cords ini_guard_pos = guard_pos;

	int prev_guard_pos[INPUT_BUFF_SIZE][INPUT_BUFF_SIZE];

	for(int i = 0; i < INPUT_BUFF_SIZE; ++i){
		for(int k = 0; k < INPUT_BUFF_SIZE; ++k){
			prev_guard_pos[i][k] = 0;
		}
	}

	//input is square so line count == col length
	while(guard_pos.col != 0 && guard_pos.col < line_count - 1 && guard_pos.row != 0 && guard_pos.row < line_count - 1){
		prev_guard_pos[guard_pos.row][guard_pos.col] = 1;
		guard_pos = move_guard(input, guard_pos);
	}

	input[ini_guard_pos.row][ini_guard_pos.col] = '^';//reset guard pos
	input[guard_pos.row][guard_pos.col] = '.';//erase last guard pos

	//set to 1 to count inital pos
	int block_spots_count = 0;
	//input is square so line count == col length
	for(int i = 0; i < line_count; ++i){
		for(int j = 0; j < line_count; ++j){
			//limits search to places the guard has been
			if(prev_guard_pos[i][j] == 0){
				continue;
			}
			//skips positions next to or in guard
			if(i == ini_guard_pos.row && (j == ini_guard_pos.col || j == ini_guard_pos.col - 1 || j == ini_guard_pos.col + 1)){
				continue;
			}
			if(j == ini_guard_pos.col && (i == ini_guard_pos.row || i == ini_guard_pos.row + 1 || i == ini_guard_pos.row - 1)){
				continue;
			}
			if(input[i][j] == '.'){//to not erase already existing '#'
				input[i][j] = '#';
			}else{
				continue;
			}
			guard_pos = ini_guard_pos;

			edsa_htable *pos_hash;
			edsa_htable_init(&pos_hash, sizeof(guard_state), sizeof(char), 100);

			while(guard_pos.col != 0 && guard_pos.col < line_count - 1 && guard_pos.row != 0 && guard_pos.row < line_count - 1){
				int def_value = 0;
				guard_state state;
				state.guard_pos = guard_pos;
				state.dir = input[guard_pos.row][guard_pos.col];
				edsa_htable_ins(pos_hash, &state, &def_value);

				guard_pos = move_guard(input, guard_pos);

				state.guard_pos = guard_pos;
				state.dir = input[guard_pos.row][guard_pos.col];

				if(edsa_htable_read(pos_hash, &state, &def_value) == EDSA_SUCCESS){
					++block_spots_count;
					break;
				}
			}

			input[ini_guard_pos.row][ini_guard_pos.col] = '^';//reset guard pos
			input[guard_pos.row][guard_pos.col] = '.';//erase last guard pos
			input[i][j] = '.';

			edsa_htable_free(pos_hash);
		}
	}

	printf("%d\n", block_spots_count);

	free(input_line);
	return 0;
}
