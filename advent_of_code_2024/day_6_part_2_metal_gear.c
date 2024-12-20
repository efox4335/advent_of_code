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

cords move_guard_opt(char input[INPUT_BUFF_SIZE][INPUT_BUFF_SIZE], cords guard_pos, int line_count)
{
	switch(input[guard_pos.row][guard_pos.col]){
		case '^':
			if(input[guard_pos.row - 1][guard_pos.col] != '.'){
				input[guard_pos.row][guard_pos.col] = '>';
				return guard_pos;
			}else{
				cords temp_pos = guard_pos;
				while(temp_pos.row > 0 && input[temp_pos.row - 1][temp_pos.col] == '.'){
					temp_pos.row -= 1;
				}
				input[guard_pos.row][guard_pos.col] = '.';
				input[temp_pos.row][temp_pos.col] = '^';
				return temp_pos;
			}
		case '>':
			if(input[guard_pos.row][guard_pos.col + 1] != '.'){
				input[guard_pos.row][guard_pos.col] = 'v';
				return guard_pos;
			}else{
				cords temp_pos = guard_pos;
				while(temp_pos.col < line_count && input[temp_pos.row][temp_pos.col + 1] == '.'){
					temp_pos.col += 1;
				}
				input[guard_pos.row][guard_pos.col] = '.';
				input[temp_pos.row][temp_pos.col] = '>';
				return temp_pos;
			}
		case '<':
			if(input[guard_pos.row][guard_pos.col - 1] != '.'){
				input[guard_pos.row][guard_pos.col] = '^';
				return guard_pos;
			}else{
				cords temp_pos = guard_pos;
				while(temp_pos.col > 0 && input[temp_pos.row][temp_pos.col - 1] == '.'){
					temp_pos.col -= 1;
				}
				input[guard_pos.row][guard_pos.col] = '.';
				input[temp_pos.row][temp_pos.col] = '<';
				return temp_pos;
			}
		case 'v':
			if(input[guard_pos.row + 1][guard_pos.col] != '.'){
				input[guard_pos.row][guard_pos.col] = '<';
				return guard_pos;
			}else{
				cords temp_pos = guard_pos;
				while(temp_pos.row < line_count && input[temp_pos.row + 1][temp_pos.col] == '.'){
					temp_pos.row += 1;
				}
				input[guard_pos.row][guard_pos.col] = '.';
				input[temp_pos.row][temp_pos.col] = 'v';
				return temp_pos;
			}
	}

	cords bad_val = {-1, -1};
	return bad_val;
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

	//add in the last guard position
	prev_guard_pos[guard_pos.row][guard_pos.col] = 1;

	input[ini_guard_pos.row][ini_guard_pos.col] = '^';//reset guard pos
	input[guard_pos.row][guard_pos.col] = '.';//erase last guard pos

	edsa_htable *move_guard_opt_cashe = NULL;
	edsa_htable_init(&move_guard_opt_cashe, sizeof(guard_state), sizeof(guard_state), 100);

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
			if(i == ini_guard_pos.row && j == ini_guard_pos.col){
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

				//cashe move_guard output if it is not in line with inserted box
				if(guard_pos.row == i || guard_pos.col == j){
					guard_pos = move_guard(input, guard_pos);
				}else{
					guard_state out_state;
					guard_state in_state;
					in_state.guard_pos = guard_pos;
					in_state.dir = input[guard_pos.row][guard_pos.col];
					//if guard pos has already been called with the same arguments
					if(edsa_htable_read(move_guard_opt_cashe, &in_state, &out_state) == EDSA_SUCCESS){
						input[guard_pos.row][guard_pos.col] = '.';
						guard_pos = out_state.guard_pos;
						input[guard_pos.row][guard_pos.col] = out_state.dir;
					}else{
						out_state.guard_pos = move_guard_opt(input, guard_pos, line_count);
						out_state.dir = input[out_state.guard_pos.row][out_state.guard_pos.col];
						edsa_htable_ins(move_guard_opt_cashe, &in_state, &out_state);
						guard_pos = out_state.guard_pos;
					}
				}

				state.guard_pos = guard_pos;
				state.dir = input[guard_pos.row][guard_pos.col];

				if(edsa_htable_read(pos_hash, &state, &def_value) == EDSA_SUCCESS){
					++block_spots_count;
					break;
				}
			}

			input[guard_pos.row][guard_pos.col] = '.';//erase last guard pos
			input[ini_guard_pos.row][ini_guard_pos.col] = '^';//reset guard pos

			input[i][j] = '.';

			edsa_htable_free(pos_hash);
		}
	}

	printf("%d\n", block_spots_count);

	edsa_htable_free(move_guard_opt_cashe);
	free(input_line);
	return 0;
}
