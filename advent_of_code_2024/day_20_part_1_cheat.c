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
enum{END_VAL = -2, START_VAL = -3, WALL = -1, PATH = -4};

typedef struct{
	int row;
	int col;
}cord;

cord get_next(const int input[BUF_SIZE][BUF_SIZE], cord cur_pos, cord last_pos)
{
	const cord NORTH = {-1, 0};
	const cord SOUTH = {1, 0};
	const cord WEST = {0, -1};
	const cord EAST = {0, 1};

	cord next_pos;

	//NORTH
	next_pos.row = cur_pos.row + NORTH.row;
	next_pos.col = cur_pos.col + NORTH.col;
	if(input[next_pos.row][next_pos.col] != WALL && (last_pos.row != next_pos.row || last_pos.col != next_pos.col)){
		return next_pos;
	}

	//SOUTH
	next_pos.row = cur_pos.row + SOUTH.row;
	next_pos.col = cur_pos.col + SOUTH.col;
	if(input[next_pos.row][next_pos.col] != WALL && (last_pos.row != next_pos.row || last_pos.col != next_pos.col)){
		return next_pos;
	}

	//WEST
	next_pos.row = cur_pos.row + WEST.row;
	next_pos.col = cur_pos.col + WEST.col;
	if(input[next_pos.row][next_pos.col] != WALL && (last_pos.row != next_pos.row || last_pos.col != next_pos.col)){
		return next_pos;
	}

	//EAST
	next_pos.row = cur_pos.row + EAST.row;
	next_pos.col = cur_pos.col + EAST.col;
	if(input[next_pos.row][next_pos.col] != WALL && (last_pos.row != next_pos.row || last_pos.col != next_pos.col)){
		return next_pos;
	}
}

int main(void)
{
	int input[BUF_SIZE][BUF_SIZE];
	int line_count = 0;

	char *input_line = NULL;
	size_t lim = 0;

	cord st_pos;
	cord end_pos;

	while(getline(&input_line, &lim, stdin) > 1){
		for(int i = 0; input_line[i] != '\n'; ++i){
			switch(input_line[i]){
			case '.':
				input[line_count][i] = PATH;
				break;
			case '#':
				input[line_count][i] = WALL;
				break;
			case 'S':
				input[line_count][i] = START_VAL;
				break;
			case 'E':
				input[line_count][i] = END_VAL;
				break;
			}

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

	cord cur_pos;
	cur_pos.row = st_pos.row;
	cur_pos.col = st_pos.col;
	cord las_pos = cur_pos;

	for(int i = 0; input[cur_pos.row][cur_pos.col] != END_VAL; ++i){
		cord next_pos = get_next(input, cur_pos, las_pos);

		input[cur_pos.row][cur_pos.col] = i;

		las_pos = cur_pos;
		cur_pos = next_pos;
	}

	free(input_line);
	return 0;
}
