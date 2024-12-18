/*
 * dijkstra's algorithm
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/edsa.h"

enum{READ_AMOUNT = 1024};
enum{ROW_AMOUNT = 71, COL_AMOUNT = 71};

typedef struct{
	long row;
	long col;
}cord;

typedef struct{
	cord pos;
	int points;
}point_tile;

/*
 * sets possable_vists to the tiles that could be visited from cur_pos
 * sets the points to the correct value
 * excludes tiles with '#' on them
 * excludes tiles that are already visited
 * returns the number of tiles that could be visited from this one
*/
int get_next_visit(const char maze[ROW_AMOUNT][COL_AMOUNT], const point_tile cur_pos, point_tile *possable_visits)
{
	int vist_amount = 0;

	//NORTH
	if(cur_pos.pos.row > 0 && maze[cur_pos.pos.row - 1][cur_pos.pos.col] != '#'){
		possable_visits[vist_amount].pos.row = cur_pos.pos.row - 1;
		possable_visits[vist_amount].pos.col = cur_pos.pos.col;
		possable_visits[vist_amount].points = cur_pos.points + 1;

		++vist_amount;
	}

	//SOUTH
	if(cur_pos.pos.row < ROW_AMOUNT - 1 && maze[cur_pos.pos.row + 1][cur_pos.pos.col] != '#'){
		possable_visits[vist_amount].pos.row = cur_pos.pos.row + 1;
		possable_visits[vist_amount].pos.col = cur_pos.pos.col;
		possable_visits[vist_amount].points = cur_pos.points + 1;

		++vist_amount;
	}

	//WEST
	if(cur_pos.pos.col > 0 && maze[cur_pos.pos.row][cur_pos.pos.col - 1] != '#'){
		possable_visits[vist_amount].pos.row = cur_pos.pos.row;
		possable_visits[vist_amount].pos.col = cur_pos.pos.col - 1;
		possable_visits[vist_amount].points = cur_pos.points + 1;

		++vist_amount;
	}

	//EAST
	if(cur_pos.pos.col < COL_AMOUNT - 1 && maze[cur_pos.pos.row][cur_pos.pos.col + 1] != '#'){
		possable_visits[vist_amount].pos.row = cur_pos.pos.row;
		possable_visits[vist_amount].pos.col = cur_pos.pos.col + 1;
		possable_visits[vist_amount].points = cur_pos.points + 1;

		++vist_amount;
	}

	return vist_amount;
}

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	char mem_space[ROW_AMOUNT][COL_AMOUNT];

	for(int i = 0; i < ROW_AMOUNT; ++i){
		for(int j = 0; j < COL_AMOUNT; ++j){
			mem_space[i][j] = '.';
		}
	}

	char delim[] = ",\n";
	int count = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		if(count >= READ_AMOUNT){
			break;
		}

		cord temp_cord;

		char *temp_ptr = strtok(input_line, delim);
		temp_cord.col = atol(temp_ptr);

		if(temp_cord.col >= COL_AMOUNT){
			++count;
			continue;
		}

		temp_ptr = strtok(NULL, delim);
		temp_cord.row = atol(temp_ptr);

		if(temp_cord.row >= ROW_AMOUNT){
			++count;
			continue;
		}

		mem_space[temp_cord.row][temp_cord.col] = '#';

		++count;
	}

	free(input_line);
	return 0;
}
