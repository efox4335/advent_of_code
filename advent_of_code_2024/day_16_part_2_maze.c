/*
 * a simple dijkstra's algorithm
 * recursivly traverse all paths shorter than the shortest path marking them down
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/edsa.h"

enum{BUF_SIZE = 200};
enum{NORTH, SOUTH, EAST, WEST};

typedef struct{
	int row;
	int col;
}cord;

//used on the heap
typedef struct point_tile{
	cord pos;
	int dir;//the direction this tile was entered in
	long points;//the amount of points to travel to this tile including itself
	struct point_tile *parent;
}point_tile;

//used to store previously visited points
typedef struct{
	cord pos;
	int dir;//the direction this tile was entered in
}visited_tile;

typedef struct{
	int ret_val;
	int points;
}points_retval;

int cmp_func(const void *heap_ele_1, const void *heap_ele_2)
{
	return (*((point_tile **) heap_ele_1))->points < (*((point_tile **) heap_ele_2))->points;
}

/*
 * sets possable_vists to the tiles that could be visited from cur_pos
 * sets the points to the correct value
 * excludes tiles with '#' on them
 * returns the number of tiles that could be visited from this one
*/
int get_next_visit(const char maze[BUF_SIZE][BUF_SIZE], point_tile *cur_pos, point_tile *possable_visits)
{
	int vist_amount = 0;

	visited_tile foward;
	visited_tile left;
	visited_tile right;

	switch(cur_pos->dir){
	case NORTH:
		foward.pos.row = -1;
		foward.pos.col = 0;
		foward.dir = NORTH;

		left.pos.row = 0;
		left.pos.col = -1;
		left.dir = WEST;

		right.pos.row = 0;
		right.pos.col = 1;
		right.dir = EAST;
		break;
	case SOUTH:
		foward.pos.row = 1;
		foward.pos.col = 0;
		foward.dir = SOUTH;

		left.pos.row = 0;
		left.pos.col = 1;
		left.dir = EAST;

		right.pos.row = 0;
		right.pos.col = -1;
		right.dir = WEST;
		break;
	case EAST:
		foward.pos.row = 0;
		foward.pos.col = 1;
		foward.dir = EAST;

		left.pos.row = -1;
		left.pos.col = 0;
		left.dir = NORTH;

		right.pos.row = 1;
		right.pos.col = 0;
		right.dir = SOUTH;
		break;
	case WEST:
		foward.pos.row = 0;
		foward.pos.col = -1;
		foward.dir = WEST;

		left.pos.row = 1;
		left.pos.col = 0;
		left.dir = SOUTH;

		right.pos.row = -1;
		right.pos.col = 0;
		right.dir = NORTH;
		break;
	}

	visited_tile temp_visited;
	//foward
	temp_visited.pos.row = cur_pos->pos.row + foward.pos.row;
	temp_visited.pos.col = cur_pos->pos.col + foward.pos.col;
	temp_visited.dir = foward.dir;
	if(maze[temp_visited.pos.row][temp_visited.pos.col] != '#'){
		possable_visits[vist_amount].pos.row = temp_visited.pos.row;
		possable_visits[vist_amount].pos.col = temp_visited.pos.col;
		possable_visits[vist_amount].dir = temp_visited.dir;
		possable_visits[vist_amount].points = cur_pos->points + 1;
		possable_visits[vist_amount].parent = cur_pos;
		++vist_amount;
	}

	//left
	temp_visited.pos.row = cur_pos->pos.row + left.pos.row;
	temp_visited.pos.col = cur_pos->pos.col + left.pos.col;
	temp_visited.dir = left.dir;
	if(maze[temp_visited.pos.row][temp_visited.pos.col] != '#'){
		possable_visits[vist_amount].pos.row = temp_visited.pos.row;
		possable_visits[vist_amount].pos.col = temp_visited.pos.col;
		possable_visits[vist_amount].dir = temp_visited.dir;
		possable_visits[vist_amount].points = cur_pos->points + 1001;
		possable_visits[vist_amount].parent = cur_pos;
		++vist_amount;
	}

	//right
	temp_visited.pos.row = cur_pos->pos.row + right.pos.row;
	temp_visited.pos.col = cur_pos->pos.col + right.pos.col;
	temp_visited.dir = right.dir;
	if(maze[temp_visited.pos.row][temp_visited.pos.col] != '#'){
		possable_visits[vist_amount].pos.row = temp_visited.pos.row;
		possable_visits[vist_amount].pos.col = temp_visited.pos.col;
		possable_visits[vist_amount].dir = temp_visited.dir;
		possable_visits[vist_amount].points = cur_pos->points + 1001;
		possable_visits[vist_amount].parent = cur_pos;
		++vist_amount;
	}

	return vist_amount;
}

int get_all_paths(char maze[BUF_SIZE][BUF_SIZE], long point_lim, point_tile cur_pos, edsa_htable *vis)
{
	if(cur_pos.points > point_lim){
		return 0;
	}

	visited_tile temp;
	temp.pos.row = cur_pos.pos.row;
	temp.pos.col = cur_pos.pos.col;
	temp.dir = cur_pos.dir;
	points_retval points;

	if(edsa_htable_read(vis, &temp, &points) == EDSA_SUCCESS){
		if(points.points < cur_pos.points){
			return 0;
		}
	}

	points.points = cur_pos.points;

	if(maze[cur_pos.pos.row][cur_pos.pos.col] == 'E'){
		printf("found\n");
		points.ret_val = 1;
		edsa_htable_ins(vis, &temp, &points);
		return 1;
	}

	point_tile next_points[3];
	int next_tiles = get_next_visit(maze, &cur_pos, next_points);
	int ret_val = 0;

	for(int i = 0; i < next_tiles; ++i){
		if(get_all_paths(maze, point_lim, next_points[i], vis) == 1){
			maze[cur_pos.pos.row][cur_pos.pos.col] = 'O';
			ret_val = 1;
		}
	}

	points.ret_val = 0;
	edsa_htable_ins(vis, &temp, &points);
	return ret_val;
}

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	char input[BUF_SIZE][BUF_SIZE];
	int line_count = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		strcpy(input[line_count], input_line);
		++line_count;
	}

	point_tile tile_arr[100000];
	int tile_count = 0;

	//s is always in the same pos
	tile_arr[tile_count].pos.row = line_count - 2;
	tile_arr[tile_count].pos.col = 1;
	tile_arr[tile_count].dir = EAST;
	tile_arr[tile_count].points = 0;
	tile_arr[tile_count].parent = NULL;
	point_tile *cur_tile_ptr = &tile_arr[tile_count];
	++tile_count;

	point_tile visits[3];

	edsa_heap *heap = NULL;
	edsa_htable *visited = NULL;

	edsa_heap_init(&heap, 1000, sizeof(point_tile *), cmp_func);
	edsa_heap_ins(heap, &cur_tile_ptr);

	edsa_htable_init(&visited, sizeof(visited_tile), sizeof(long), 1000);

	while(input[cur_tile_ptr->pos.row][cur_tile_ptr->pos.col] != 'E'){
		edsa_heap_remove(heap, &cur_tile_ptr);
		long temp = 0;

		visited_tile temp_visited;
		temp_visited.pos.row = cur_tile_ptr->pos.row;
		temp_visited.pos.col = cur_tile_ptr->pos.col;
		temp_visited.dir = cur_tile_ptr->dir;

		if(edsa_htable_read(visited, &temp_visited, &temp) == EDSA_SUCCESS){
			continue;
		}

		temp = cur_tile_ptr->points;

		edsa_htable_ins(visited, &temp_visited, &temp);

		int next_visit = get_next_visit(input, cur_tile_ptr, visits);

		for(int i = 0; i < next_visit; ++i){
			tile_arr[tile_count] = visits[i];
			point_tile *tile_ptr = &tile_arr[tile_count];
			edsa_heap_ins(heap, &tile_ptr);

			++tile_count;
		}
	}

	edsa_htable_free(visited);
	edsa_heap_free(heap);

	edsa_htable *vis = NULL;
	edsa_htable_init(&vis, sizeof(visited_tile), sizeof(points_retval), 1000);

	get_all_paths(input, cur_tile_ptr->points, tile_arr[0], vis);

	int paths_len = 1;
	for(int i = 0; i < line_count; ++i){
		for(int j = 0; j < line_count; ++j){
			if(input[i][j] == 'O'){
				++paths_len;
			}
		}
	}

	printf("%d\n", paths_len);

	edsa_htable_free(vis);

	free(input_line);
	return 0;
}
