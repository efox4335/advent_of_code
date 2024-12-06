/*
 * just simulate the guard till it goes off screen while brute forcing all possable positions
 * a cycle happens when the guard goes in the same position and direction
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum{INPUT_BUFF_SIZE = 200};

typedef struct{
	int row;
	int col;
}cords;

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
				input[guard_pos.row][guard_pos.col] = '.';
				input[guard_pos.row][guard_pos.col + 1] = '>';
				guard_pos.col += 1;
				return guard_pos;
			}else{
				input[guard_pos.row][guard_pos.col] = '.';
				input[guard_pos.row - 1][guard_pos.col] = '^';
				guard_pos.row -= 1;
				return guard_pos;
			}
		case '>':
			if(input[guard_pos.row][guard_pos.col + 1] != '.'){
				input[guard_pos.row][guard_pos.col] = '.';
				input[guard_pos.row + 1][guard_pos.col] = 'v';
				guard_pos.row += 1;
				return guard_pos;
			}else{
				input[guard_pos.row][guard_pos.col] = '.';
				input[guard_pos.row][guard_pos.col + 1] = '>';
				guard_pos.col += 1;
				return guard_pos;
			}
		case '<':
			if(input[guard_pos.row][guard_pos.col - 1] != '.'){
				input[guard_pos.row][guard_pos.col] = '.';
				input[guard_pos.row - 1][guard_pos.col] = '^';
				guard_pos.row -= 1;
				return guard_pos;
			}else{
				input[guard_pos.row][guard_pos.col] = '.';
				input[guard_pos.row][guard_pos.col - 1] = '<';
				guard_pos.col -= 1;
				return guard_pos;
			}
		case 'v':
			if(input[guard_pos.row + 1][guard_pos.col] != '.'){
				input[guard_pos.row][guard_pos.col] = '.';
				input[guard_pos.row][guard_pos.col - 1] = '<';
				guard_pos.col -= 1;
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

void clear_prev_pos (char prev_pos[INPUT_BUFF_SIZE][INPUT_BUFF_SIZE])
{
	for(int i = 0; i < INPUT_BUFF_SIZE; ++i){
		for(int k = 0; k < INPUT_BUFF_SIZE; ++k){
			prev_pos[i][k] = '.';
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

	char prev_guard_pos[INPUT_BUFF_SIZE][INPUT_BUFF_SIZE];

	clear_prev_pos(prev_guard_pos);

	//set to 1 to count inital pos
	int path_len = 1;
	//input is square so line count == col length
	while(guard_pos.col != 0 && guard_pos.col < line_count - 1 && guard_pos.row != 0 && guard_pos.row < line_count - 1){
		prev_guard_pos[guard_pos.row][guard_pos.col] = input[guard_pos.row][guard_pos.col];
		guard_pos = move_guard(input, guard_pos);

		if(prev_guard_pos[guard_pos.row][guard_pos.col] == '.'){
			++path_len;
		}
	}

	printf("%d\n", path_len);

	free(input_line);
	return 0;
}
