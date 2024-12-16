/*
 * a simple dijkstra's algorithm to find the shortest path length
 * a recursive dfs to find all paths equal to the shortest path length if it ends in finish than mark it down
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
typedef struct{
	cord pos;
	int dir;//the direction this tile was entered in
	long points;//the amount of points to travel to this tile including itself
}point_tile;

//used to store previously visited points
typedef struct{
	cord pos;
	int dir;//the direction this tile was entered in
}visited_tile;

int cmp_func(const void *heap_ele_1, const void *heap_ele_2)
{
	return ((point_tile *) heap_ele_1)->points < ((point_tile *) heap_ele_2)->points;
}

/*
 * sets possable_vists to the tiles that could be visited from cur_pos
 * sets the points to the correct value
 * excludes tiles with '#' on them
 * excludes tiles that have been already visited
 * returns the number of tiles that could be visited from this one
*/
int get_next_visit(const char maze[BUF_SIZE][BUF_SIZE], edsa_htable *prev_visited, const point_tile cur_pos, point_tile *possable_visits)
{
	int vist_amount = 0;
	int temp = 0;

	visited_tile foward;
	visited_tile left;
	visited_tile right;

	switch(cur_pos.dir){
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
	temp_visited.pos.row = cur_pos.pos.row + foward.pos.row;
	temp_visited.pos.col = cur_pos.pos.col + foward.pos.col;
	temp_visited.dir = foward.dir;
	if(maze[temp_visited.pos.row][temp_visited.pos.col] != '#' && edsa_htable_read(prev_visited, &temp_visited, &temp) != EDSA_SUCCESS){
		possable_visits[vist_amount].pos.row = temp_visited.pos.row;
		possable_visits[vist_amount].pos.col = temp_visited.pos.col;
		possable_visits[vist_amount].dir = temp_visited.dir;
		possable_visits[vist_amount].points = cur_pos.points + 1;
		//it's ok to insert here because this will be called at the lowest possable point count and there is no advantage to backtracking
		edsa_htable_ins(prev_visited, &temp_visited, &temp);
		++vist_amount;
	}

	//left
	temp_visited.pos.row = cur_pos.pos.row + left.pos.row;
	temp_visited.pos.col = cur_pos.pos.col + left.pos.col;
	temp_visited.dir = left.dir;
	if(maze[temp_visited.pos.row][temp_visited.pos.col] != '#' && edsa_htable_read(prev_visited, &temp_visited, &temp) != EDSA_SUCCESS){
		possable_visits[vist_amount].pos.row = temp_visited.pos.row;
		possable_visits[vist_amount].pos.col = temp_visited.pos.col;
		possable_visits[vist_amount].dir = temp_visited.dir;
		possable_visits[vist_amount].points = cur_pos.points + 1001;
		//it's ok to insert here because this will be called at the lowest possable point count and there is no advantage to backtracking
		edsa_htable_ins(prev_visited, &temp_visited, &temp);
		++vist_amount;
	}

	//right
	temp_visited.pos.row = cur_pos.pos.row + right.pos.row;
	temp_visited.pos.col = cur_pos.pos.col + right.pos.col;
	temp_visited.dir = right.dir;
	if(maze[temp_visited.pos.row][temp_visited.pos.col] != '#' && edsa_htable_read(prev_visited, &temp_visited, &temp) != EDSA_SUCCESS){
		possable_visits[vist_amount].pos.row = temp_visited.pos.row;
		possable_visits[vist_amount].pos.col = temp_visited.pos.col;
		possable_visits[vist_amount].dir = temp_visited.dir;
		possable_visits[vist_amount].points = cur_pos.points + 1001;
		//it's ok to insert here because this will be called at the lowest possable point count and there is no advantage to backtracking
		edsa_htable_ins(prev_visited, &temp_visited, &temp);
		++vist_amount;
	}

	return vist_amount;
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

	//s is always in the same pos
	point_tile cur_tile;
	cur_tile.pos.row = line_count - 2;
	cur_tile.pos.col = 1;
	cur_tile.dir = EAST;
	cur_tile.points = 0;

	point_tile visits[3];

	edsa_heap *heap = NULL;
	edsa_htable *visited = NULL;

	edsa_heap_init(&heap, 1000, sizeof(point_tile), cmp_func);
	edsa_heap_ins(heap, &cur_tile);

	edsa_htable_init(&visited, sizeof(visited_tile), sizeof(int), 1000);

	while(input[cur_tile.pos.row][cur_tile.pos.col] != 'E'){
		edsa_heap_remove(heap, &cur_tile);

		int next_visit = get_next_visit(input, visited, cur_tile, visits);

		for(int i = 0; i < next_visit; ++i){
			edsa_heap_ins(heap, &visits[i]);
		}
	}

	edsa_htable_free(visited);
	edsa_heap_free(heap);

	printf("%ld\n", cur_tile.points);

	free(input_line);
	return 0;
}
