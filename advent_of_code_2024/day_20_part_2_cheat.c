/*
 * mark each path with it's distance from the start
 * the time a shortcut saves is equal to the differences of dist from start - cut len
 * a cut is only valid if it is adjcent to the normal path
 * i'm going on the assumpiton that .##. would not be a valid cut
 * get all distances between part of the path and calculate cuts that are close enough
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

int get_taxi_dist(cord pos_1, cord pos_2)
{
	return abs(pos_1.row - pos_2.row) + abs(pos_1.col - pos_2.col);
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

	cord path[10000];
	int path_len = 0;

	for(int i = 0; cur_pos.row != end_pos.row || cur_pos.col != end_pos.col; ++i){
		cord next_pos = get_next(input, cur_pos, las_pos);

		input[cur_pos.row][cur_pos.col] = i;

		path[i].row = cur_pos.row;
		path[i].col = cur_pos.col;

		las_pos = cur_pos;
		cur_pos = next_pos;

		input[cur_pos.row][cur_pos.col] = i + 1;

		path[i + 1].row = cur_pos.row;
		path[i + 1].col = cur_pos.col;

		path_len = i + 2;
	}

	int good_cut = 0;

	for(int i = 0; i < path_len; ++i){
		for(int j = i; j < path_len; ++j){
			int dist = get_taxi_dist(path[i], path[j]);
			if(dist <= 20){
				int cut_len = (j - i) - dist;

				if(cut_len >= 100){
					++good_cut;
				}
			}
		}
	}

	printf("%d\n", good_cut);

	free(input_line);
	return 0;
}
