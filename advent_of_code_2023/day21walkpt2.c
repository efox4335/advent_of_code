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
//in the real input there is a straight line through connecting the starting path to the rest of the edges
//so just find number of full garden plots in between then get the edges
//the internal ones switch from being odd distance to even distance from start for the corners
//number of internal garden plots form start to corner not including the start = (num_of_steps - 2(hight of plot - (s + 1)))/ hight of plot
//number of total internal plots with the same
//
//start fill with bfs()
#include <stdio.h>
#include <string.h>

#define QUE_MAX 100000
#define MAX_DIST 26501365

typedef struct{
	int is_open;
	long int dist_from_start;
}path;

typedef struct{
	long int row;
	long int col;
}node;

long int cashe[200][200][500];
path path_arr[200][200];
node start;
int path_row_max = 0;
int path_col_max = 0;
long int total_same_internal_plots = 0;
long int total_diff_internal_plots = 0;
int diff_path_count = 0;
int same_path_count = 0;

int main(void)
{
	long int bfs(long int, long int, long int);

	char *line = NULL;
	size_t lim = 0;
	long int num_of_paths = 0;
	long int internal_plot_length = 0;
	long int upper_edge_off = 0;
	long int lower_edge_off = 0;
	long int edge_size[4] = {0, 0, 0, 0};

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
					path_arr[path_row_max][i].is_open = 1;
					path_arr[path_row_max][i].dist_from_start = -1;
					start.row = path_row_max;
					start.col = i;
					break;
			}
			path_col_max = i + 1;
		}
		++path_row_max;
	}

	for(int i = 0; i < 200; ++i)
		for(int j = 0; j < 200; ++j)
			for(int k = 0; k < 500; ++k)
				cashe[i][j][k] = -1;

	same_path_count = bfs(start.row, start.col, MAX_DIST);
	diff_path_count = bfs(start.row, start.col, MAX_DIST - 1);
	internal_plot_length = (MAX_DIST - 2 * (path_row_max - (start.row + 1))) / path_row_max;
	printf("%d %d %ld\n", same_path_count, diff_path_count, internal_plot_length);

	//gets the internal path count
	for(long int i = 1; i <= internal_plot_length; ++i){
		if(i % 2 == 1){

			num_of_paths += diff_path_count + (diff_path_count * (i - 1));
		}
		else{
			num_of_paths += same_path_count + (same_path_count * (i - 1));
		}
	}
	num_of_paths *= 4;
	num_of_paths += same_path_count;

	//gets corners
	for(long int i = MAX_DIST - (path_row_max * internal_plot_length) - (path_row_max / 2) - 1; i > 0; i -= path_row_max){
		printf("%ld\n", i);
		num_of_paths += bfs(start.row, 0, i);
		num_of_paths += bfs(start.row, path_col_max - 1, i);
		num_of_paths += bfs(0, start.col, i);
		num_of_paths += bfs(path_row_max - 1, start.col, i);
		upper_edge_off = i;
		lower_edge_off = i + path_row_max;
		if(i > path_row_max){
			num_of_paths += bfs(0, 0, lower_edge_off - (path_row_max / 2) - 1);
			num_of_paths += bfs(path_row_max - 1, 0, lower_edge_off - (path_row_max / 2) - 1);
			num_of_paths += bfs(0, path_col_max - 1, lower_edge_off - (path_row_max / 2) - 1);
			num_of_paths += bfs(path_row_max - 1, path_col_max - 1, lower_edge_off - (path_row_max / 2) - 1);
		}
		num_of_paths += bfs(0, 0, upper_edge_off - (path_row_max / 2) - 1);
		num_of_paths += bfs(path_row_max - 1, 0, upper_edge_off - (path_row_max / 2) - 1);
		num_of_paths += bfs(0, path_col_max - 1, upper_edge_off - (path_row_max / 2) - 1);
		num_of_paths += bfs(path_row_max - 1, path_col_max - 1, upper_edge_off - (path_row_max / 2) - 1);
	}

	edge_size[0] = bfs(0, 0, lower_edge_off - (path_row_max / 2) - 1) +
	bfs(0, 0, upper_edge_off - (path_row_max / 2) - 1);

	edge_size[1] = bfs(path_row_max - 1, 0, lower_edge_off - (path_row_max / 2) - 1) +
	bfs(path_row_max - 1, 0, upper_edge_off - (path_row_max / 2) - 1);

	edge_size[2] = bfs(0, path_col_max - 1, lower_edge_off - (path_row_max / 2) - 1) +
	bfs(0, path_col_max - 1, upper_edge_off - (path_row_max / 2) - 1);

	edge_size[3] = bfs(path_row_max - 1, path_col_max - 1, lower_edge_off - (path_row_max / 2) - 1) +
	bfs(path_row_max - 1, path_col_max - 1, upper_edge_off - (path_row_max / 2) - 1);

	for(int i = 0; i < internal_plot_length; ++i){
		num_of_paths += edge_size[0];
		num_of_paths += edge_size[1];
		num_of_paths += edge_size[2];
		num_of_paths += edge_size[3];
	}

	printf("%ld\n", num_of_paths);
	return 0;
}

path local_path_arr[200][200];
node que[QUE_MAX];
int que_enter = 0;
int next_in_que = 0;

//does a bfs from st to end on graph
long int bfs(long int st_row, long int st_col, long int local_max)
{
	if(local_max <= 0)
		return 0;
	if(local_max > 499){
	//	printf("%ld\n", local_max % 2 == 1);
		if(local_max % 2l == 1){

			local_max = 499;
		}
		else{
			local_max = 498;
		}
	}
	if(cashe[st_row][st_col][local_max] != -1){
		return cashe[st_row][st_col][local_max];
	}
	void add_node(long int, long int, long int, int);
	void copy_graph(void);

	copy_graph();

	long int num_of_paths = 0;

	que[0].row = st_row;
	que[0].col = st_col;
	que_enter = 1;
	next_in_que = 0;
	local_path_arr[st_row][st_col].is_open = 0;
	local_path_arr[st_row][st_col].dist_from_start = 0;

	while(que_enter != next_in_que){
		add_node(que[next_in_que].row + 1, que[next_in_que].col,
		local_path_arr[que[next_in_que].row][que[next_in_que].col].dist_from_start, local_max);

		add_node(que[next_in_que].row - 1, que[next_in_que].col,
		local_path_arr[que[next_in_que].row][que[next_in_que].col].dist_from_start, local_max);

		add_node(que[next_in_que].row, que[next_in_que].col + 1,
		local_path_arr[que[next_in_que].row][que[next_in_que].col].dist_from_start, local_max);

		add_node(que[next_in_que].row, que[next_in_que].col - 1,
		local_path_arr[que[next_in_que].row][que[next_in_que].col].dist_from_start, local_max);

		++next_in_que;
	}

	for(int i = 0; i < path_row_max; ++i){
		for(int j = 0; j < path_col_max; ++j){
			if(local_path_arr[i][j].dist_from_start != -1 && local_path_arr[i][j].dist_from_start % 2 == local_max % 2)
				++num_of_paths;
		}
	}

	cashe[st_row][st_col][local_max] = num_of_paths;
	return num_of_paths;
}

//if node is open and is less then max_dist away from start adds it to que and marks it as closed and adds dist from start
void add_node(long int row, long int col, long int calling_dist_from_start, int local_max)
{
	if(row < 0 || row >= path_row_max || col < 0 || col >= path_col_max)
		return;
	if(calling_dist_from_start == local_max)
		return;
	if(local_path_arr[row][col].is_open == 0)
		return;

	local_path_arr[row][col].is_open = 0;
	local_path_arr[row][col].dist_from_start = calling_dist_from_start + 1;
	que[que_enter].row = row;
	que[que_enter].col = col;
	que_enter += 1;
}

//copies path_arr to local_path_arr
void copy_graph(void)
{
	for(int i = 0; i < path_row_max; ++i){
		for(int j = 0; j < path_col_max; ++j){
			local_path_arr[i][j].dist_from_start = -1;
			local_path_arr[i][j].is_open = path_arr[i][j].is_open;
		}
	}
}
