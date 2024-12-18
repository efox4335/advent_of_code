/*
 * dijkstra's algorithm
 * keep running till the path is blocked
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

int cmp_func(const void *heap_ele_1, const void *heap_ele_2)
{
	return (*((point_tile **) heap_ele_1))->points < (*((point_tile **) heap_ele_2))->points;
}

/*
 * sets possable_vists to the tiles that could be visited from cur_pos
 * sets the points to the correct value
 * excludes tiles with '#' on them
 * excludes tiles that are already visited
 * returns the number of tiles that could be visited from this one
*/
int get_next_visit(const char maze[ROW_AMOUNT][COL_AMOUNT], const point_tile *cur_pos, point_tile *possable_visits)
{
	int vist_amount = 0;

	//NORTH
	if(cur_pos->pos.row > 0 && maze[cur_pos->pos.row - 1][cur_pos->pos.col] != '#'){
		possable_visits[vist_amount].pos.row = cur_pos->pos.row - 1;
		possable_visits[vist_amount].pos.col = cur_pos->pos.col;
		possable_visits[vist_amount].points = cur_pos->points + 1;

		++vist_amount;
	}

	//SOUTH
	if(cur_pos->pos.row < ROW_AMOUNT - 1 && maze[cur_pos->pos.row + 1][cur_pos->pos.col] != '#'){
		possable_visits[vist_amount].pos.row = cur_pos->pos.row + 1;
		possable_visits[vist_amount].pos.col = cur_pos->pos.col;
		possable_visits[vist_amount].points = cur_pos->points + 1;

		++vist_amount;
	}

	//WEST
	if(cur_pos->pos.col > 0 && maze[cur_pos->pos.row][cur_pos->pos.col - 1] != '#'){
		possable_visits[vist_amount].pos.row = cur_pos->pos.row;
		possable_visits[vist_amount].pos.col = cur_pos->pos.col - 1;
		possable_visits[vist_amount].points = cur_pos->points + 1;

		++vist_amount;
	}

	//EAST
	if(cur_pos->pos.col < COL_AMOUNT - 1 && maze[cur_pos->pos.row][cur_pos->pos.col + 1] != '#'){
		possable_visits[vist_amount].pos.row = cur_pos->pos.row;
		possable_visits[vist_amount].pos.col = cur_pos->pos.col + 1;
		possable_visits[vist_amount].points = cur_pos->points + 1;

		++vist_amount;
	}

	return vist_amount;
}

//returns the length of the path returns -1 on no path
int path_find(const char mem_space[ROW_AMOUNT][COL_AMOUNT])
{
	edsa_heap *heap = NULL;
	edsa_htable *visited = NULL;

	point_tile tile_arr[10000];
	int tile_arr_index = 0;

	tile_arr[tile_arr_index].pos.row = 0;
	tile_arr[tile_arr_index].pos.col = 0;
	tile_arr[tile_arr_index].points = 0;//points are the number of steps needed so no need for backtracking

	edsa_heap_init(&heap, 1000, sizeof(point_tile *), cmp_func);
	point_tile *temp_ptr = &tile_arr[tile_arr_index];
	edsa_heap_ins(heap, &temp_ptr);

	++tile_arr_index;

	edsa_htable_init(&visited, sizeof(cord), sizeof(int), 1000);

	point_tile *cur_pos;

	do{
		edsa_heap_remove(heap, &cur_pos);

		point_tile next[4];
		int next_amount = get_next_visit(mem_space, cur_pos, next);

		for(int i = 0; i < next_amount; ++i){
			int temp_val = 0;
			cord temp_cord;
			temp_cord.row = next[i].pos.row;
			temp_cord.col = next[i].pos.col;

			if(edsa_htable_read(visited, &temp_cord, &temp_val) == EDSA_SUCCESS){
				continue;
			}

			edsa_htable_ins(visited, &temp_cord, &temp_val);

			tile_arr[tile_arr_index] = next[i];
			temp_ptr = &tile_arr[tile_arr_index];
			edsa_heap_ins(heap, &temp_ptr);

			++tile_arr_index;
		}

	}while(cur_pos->pos.row != ROW_AMOUNT - 1 || cur_pos->pos.col != COL_AMOUNT - 1);

	edsa_heap_free(heap);
	edsa_htable_free(visited);

	return cur_pos->points;
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

	printf("%d\n", path_find(mem_space));

	free(input_line);
	return 0;
}
