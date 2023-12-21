//proof if you can get to a square in an even number of steps you can only get there in an even number of steps and the same for and odd number of steps
//assuming that the only way to change an even number into and odd and vice versa one through addition is to add an odd number to it
//also assuming that for any closed polygon with only right angles the total distance of sides with the inside of the shape to the left
//is equal to the total distance of the sides where the inside of the shape is to the right and the same for sides where the top and bottom is the insde
//of the shape
//we can ignore all cases where the path to the point we want to get to isn't a straight line because a bent line is just 2 straight lines
//imagine the shortest path to the point
//every step you take that does not go towards the point you must take a step in the opposite direction because any potental new path with a different
//steps % 2 number will be closed if you take into account the shortest path when going in the opposite direction it's the same as going 2 steps whatch
//cant change steps % 2 so to change this number you can't take any steps off the shortest path and there is on one
//
//use dfs to find the shortest distances to each garden plots from the start
//take only those paths that can be arrived at in and even number of steps and less then 64 steps
#include <stdio.h>
#include <string.h>

#define QUE_MAX 100000
#define MAX_DIST 64

typedef struct{
	int is_open;
	int dist_from_start;
}path;

typedef struct{
	int row;
	int col;
}node;

path path_arr[200][200];
node que[QUE_MAX];
node start;
int que_enter = 0;
int next_in_que = 0;
int path_row_max = 0;
int path_col_max = 0;

int main(void)
{
	void add_node(int, int, int);

	char *line = NULL;
	size_t lim = 0;
	int num_of_paths = 0;

	while(getline(&line, &lim, stdin) > 1){
		for(int i = 0; line[i] !=  '\n'; ++i){
			switch(line[i]){
				case '.':
					path_arr[path_row_max][i].is_open = 1;
					path_arr[path_row_max][i].dist_from_start = -1;
					break;
				case '#':
					path_arr[path_row_max][i].is_open = 0;
					path_arr[path_row_max][i].dist_from_start = -1;
					break;
				case 'S':
					path_arr[path_row_max][i].is_open = 0;
					path_arr[path_row_max][i].dist_from_start = 0;
					start.row = path_row_max;
					start.col = i;
					break;
			}
			path_col_max = i + 1;
		}
		++path_row_max;
	}

	que[0].row = start.row;
	que[0].col = start.col;
	que_enter = 1;
	next_in_que = 0;

	while(que_enter != next_in_que){
		add_node(que[next_in_que].row + 1, que[next_in_que].col,
		path_arr[que[next_in_que].row][que[next_in_que].col].dist_from_start);

		add_node(que[next_in_que].row - 1, que[next_in_que].col,
		path_arr[que[next_in_que].row][que[next_in_que].col].dist_from_start);

		add_node(que[next_in_que].row, que[next_in_que].col + 1,
		path_arr[que[next_in_que].row][que[next_in_que].col].dist_from_start);

		add_node(que[next_in_que].row, que[next_in_que].col - 1,
		path_arr[que[next_in_que].row][que[next_in_que].col].dist_from_start);

		++next_in_que;
	}

	for(int i = 0; i < path_row_max; ++i){
		for(int j = 0; j < path_col_max; ++j){
			if(path_arr[i][j].dist_from_start != -1 && path_arr[i][j].dist_from_start % 2 == MAX_DIST % 2)
				++num_of_paths;
		}
	}

	printf("%d\n", num_of_paths);
	return 0;
}

//if node is open and is less then MAX_DIST away from start adds it to que and marks it as closed and adds dist from start
void add_node(int row, int col, int calling_dist_from_start)
{
	if(calling_dist_from_start == MAX_DIST)
		return;
	if(path_arr[row][col].is_open == 0)
		return;

	path_arr[row][col].is_open = 0;
	path_arr[row][col].dist_from_start = calling_dist_from_start + 1;
	que[que_enter].row = row;
	que[que_enter].col = col;
	++que_enter;
}
