/*
 * mark each path with it's distance from the start
 * the time a shortcut saves is equal to the differences of dist from start - cut len
 * a cut is only valid if it is adjcent to the normal path
 * i'm going on the assumpiton that .##. would have a valid cut
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum{BUF_SIZE = 200};

typedef struct{
	int row;
	int col;
}cord;

cord get_next(const char input[BUF_SIZE][BUF_SIZE], cord cur_pos, cord last_pos)
{
	const cord NORTH = {-1, 0};
	const cord SOUTH = {1, 0};
	const cord WEST = {0, -1};
	const cord EAST = {0, 1};

	cord next_pos;

	//NORTH
	next_pos.row = cur_pos.row + NORTH.row;
	next_pos.col = cur_pos.col + NORTH.col;
	if(input[next_pos.row][next_pos.col] != '#' && last_pos.row != next_pos.row && last_pos.col != next_pos.col){
		return next_pos;
	}

	//SOUTH
	next_pos.row = cur_pos.row + SOUTH.row;
	next_pos.col = cur_pos.col + SOUTH.col;
	if(input[next_pos.row][next_pos.col] != '#' && last_pos.row != next_pos.row && last_pos.col != next_pos.col){
		return next_pos;
	}

	//WEST
	next_pos.row = cur_pos.row + WEST.row;
	next_pos.col = cur_pos.col + WEST.col;
	if(input[next_pos.row][next_pos.col] != '#' && last_pos.row != next_pos.row && last_pos.col != next_pos.col){
		return next_pos;
	}

	//EAST
	next_pos.row = cur_pos.row + EAST.row;
	next_pos.col = cur_pos.col + EAST.col;
	if(input[next_pos.row][next_pos.col] != '#' && last_pos.row != next_pos.row && last_pos.col != next_pos.col){
		return next_pos;
	}
}

int main(void)
{
	char input[BUF_SIZE][BUF_SIZE];
	int line_count = 0;

	char *input_line = NULL;
	size_t lim = 0;

	cord st_pos;
	cord end_pos;

	while(getline(&input_line, &lim, stdin) > 1){
		for(int i = 0; input_line[i] != '\n'; ++i){
			input[line_count][i] = input_line[i];

			if(input_line[i] == 'S'){
				st_pos.row = line_count;
				st_pos.col = i;
			}

			if(input_line[i] == 'E'){
				end_pos.row = line_count;
				end_pos.col = i;
			}
		}

		++line_count;
	}

	free(input_line);
	return 0;
}
