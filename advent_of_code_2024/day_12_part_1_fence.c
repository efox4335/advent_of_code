/*
 * gets area via flood fill
*/
#include <stdio.h>
#include <stdlib.h>
#include "include/edsa.h"

enum{VISITED, NOT_VISITED};
enum{BUF_SIZE = 200};
enum{TOP, BOTTOM, LEFT, RIGHT};

typedef struct{
	char plant;
	int visited;
}plant;

typedef struct{
	int row;
	int col;
}cord;

typedef struct{
	int perimeter;
	int area;
}shape;

typedef struct{
	cord loc;
	int dir;
}plant_dir;

//starts from point
//returns perimeter and sets polygon to a list of points along the edges including inner corners
//simulates walking perimeter by keeping track of each plant + edge if a plant + edge has already been seen the perimeter is found
//stores all plant + edge combinations in a hash table
//explores clockwise
//perim_cashe keeps track of all direction+location so no double counting happens
int get_perimeter(const cord point, const int direction, const plant input[BUF_SIZE][BUF_SIZE], const int line_count, edsa_htable *perim_cashe)
{
	int perimeter = 0;
	int temp = 1;
	plant_dir seen_key;
	seen_key.loc.row = point.row;
	seen_key.loc.col = point.col;
	seen_key.dir = direction;

	while(edsa_htable_read(perim_cashe, &seen_key, &temp) != EDSA_SUCCESS){
		edsa_htable_ins(perim_cashe, &seen_key, &temp);
		plant_dir next_plant;

		switch(seen_key.dir){
		case TOP:
			//outer corner
			if(seen_key.loc.col == line_count - 1 ||
			input[seen_key.loc.row][seen_key.loc.col].plant != input[seen_key.loc.row][seen_key.loc.col + 1].plant){//if on the edge of input or if on the edge of shape
				next_plant.dir = RIGHT;
				next_plant.loc.row = seen_key.loc.row;
				next_plant.loc.col = seen_key.loc.col;
			//line
			}else if(seen_key.loc.row == 0 ||
			input[seen_key.loc.row][seen_key.loc.col].plant != input[seen_key.loc.row - 1][seen_key.loc.col + 1].plant){//no need to check neighbor as we already know it matches
				next_plant.dir = TOP;
				next_plant.loc.row = seen_key.loc.row;
				next_plant.loc.col = seen_key.loc.col + 1;
			//inner corner
			}else{
				next_plant.dir = LEFT;
				next_plant.loc.row = seen_key.loc.row - 1;
				next_plant.loc.col = seen_key.loc.col + 1;
			}
			break;
		case BOTTOM:
			//outer corner
			if(seen_key.loc.col == 0 ||
			input[seen_key.loc.row][seen_key.loc.col].plant != input[seen_key.loc.row][seen_key.loc.col - 1].plant){//if on the edge of input or if on the edge of shape
				next_plant.dir = LEFT;
				next_plant.loc.row = seen_key.loc.row;
				next_plant.loc.col = seen_key.loc.col;
			//line
			}else if(seen_key.loc.row == line_count - 1 ||
			input[seen_key.loc.row][seen_key.loc.col].plant != input[seen_key.loc.row + 1][seen_key.loc.col - 1].plant){//no need to check neighbor as we already know it matches
				next_plant.dir = BOTTOM;
				next_plant.loc.row = seen_key.loc.row;
				next_plant.loc.col = seen_key.loc.col - 1;
			//inner corner
			}else{
				next_plant.dir = RIGHT;
				next_plant.loc.row = seen_key.loc.row + 1;
				next_plant.loc.col = seen_key.loc.col - 1;
			}
			break;
		case LEFT:
			//outer corner
			if(seen_key.loc.row == 0 ||
			input[seen_key.loc.row][seen_key.loc.col].plant != input[seen_key.loc.row - 1][seen_key.loc.col].plant){//if on the edge of input or if on the edge of shape
				next_plant.dir = TOP;
				next_plant.loc.row = seen_key.loc.row;
				next_plant.loc.col = seen_key.loc.col;
			//line
			}else if(seen_key.loc.col == 0 ||
			input[seen_key.loc.row][seen_key.loc.col].plant != input[seen_key.loc.row - 1][seen_key.loc.col - 1].plant){//no need to check neighbor as we already know it matches
				next_plant.dir = LEFT;
				next_plant.loc.row = seen_key.loc.row - 1;
				next_plant.loc.col = seen_key.loc.col;
			//inner corner
			}else{
				next_plant.dir = BOTTOM;
				next_plant.loc.row = seen_key.loc.row - 1;
				next_plant.loc.col = seen_key.loc.col - 1;
			}
			break;
		case RIGHT:
			//outer corner
			if(seen_key.loc.row == line_count - 1 ||
			input[seen_key.loc.row][seen_key.loc.col].plant != input[seen_key.loc.row + 1][seen_key.loc.col].plant){//if on the edge of input or if on the edge of shape
				next_plant.dir = BOTTOM;
				next_plant.loc.row = seen_key.loc.row;
				next_plant.loc.col = seen_key.loc.col;
			//line
			}else if(seen_key.loc.col == line_count - 1 ||
			input[seen_key.loc.row][seen_key.loc.col].plant != input[seen_key.loc.row + 1][seen_key.loc.col + 1].plant){//no need to check neighbor as we already know it matches
				next_plant.dir = RIGHT;
				next_plant.loc.row = seen_key.loc.row + 1;
				next_plant.loc.col = seen_key.loc.col;
			//inner corner
			}else{
				next_plant.dir = TOP;
				next_plant.loc.row = seen_key.loc.row + 1;
				next_plant.loc.col = seen_key.loc.col + 1;
			}
			break;
		}

		seen_key.loc.row = next_plant.loc.row;
		seen_key.loc.col = next_plant.loc.col;
		seen_key.dir = next_plant.dir;

		++perimeter;
	}


	return perimeter;
}

//gets perimeter and area
//gets area by floodfill
shape get_shape_data(cord point, plant input[BUF_SIZE][BUF_SIZE], int line_count)
{
	shape shape_data;
	shape_data.area = 0;
	shape_data.perimeter = 0;
	cord visit_stack[10000];
	int visit_stack_ind = 0;

	edsa_htable *perim_cashe = NULL;
	edsa_htable_init(&perim_cashe, sizeof(plant_dir), sizeof(int), 100);

	visit_stack[visit_stack_ind] = point;
	++visit_stack_ind;

	while(visit_stack_ind != 0){
		--visit_stack_ind;
		point = visit_stack[visit_stack_ind];
		if(input[point.row][point.col].visited == VISITED){
			continue;
		}
		input[point.row][point.col].visited = VISITED;

		if(point.row == 0 || input[point.row][point.col].plant != input[point.row - 1][point.col].plant){
			shape_data.perimeter += get_perimeter(point, TOP, input, line_count, perim_cashe);
		}else if(input[point.row - 1][point.col].visited != VISITED){
			visit_stack[visit_stack_ind].row = point.row - 1;
			visit_stack[visit_stack_ind].col = point.col;
			++visit_stack_ind;
		}
		if(point.row == line_count - 1 || input[point.row][point.col].plant != input[point.row + 1][point.col].plant){
			shape_data.perimeter += get_perimeter(point, BOTTOM, input, line_count, perim_cashe);
		}else if(input[point.row + 1][point.col].visited != VISITED){
			visit_stack[visit_stack_ind].row = point.row + 1;
			visit_stack[visit_stack_ind].col = point.col;
			++visit_stack_ind;
		}
		if(point.col == 0 || input[point.row][point.col].plant != input[point.row][point.col - 1].plant){
			shape_data.perimeter += get_perimeter(point, LEFT, input, line_count, perim_cashe);
		}else if(input[point.row][point.col - 1].visited != VISITED){
			visit_stack[visit_stack_ind].row = point.row;
			visit_stack[visit_stack_ind].col = point.col - 1;
			++visit_stack_ind;
		}
		if(point.col == line_count - 1 || input[point.row][point.col].plant != input[point.row][point.col + 1].plant){
			shape_data.perimeter += get_perimeter(point, RIGHT, input, line_count, perim_cashe);
		}else if(input[point.row][point.col + 1].visited != VISITED){
			visit_stack[visit_stack_ind].row = point.row;
			visit_stack[visit_stack_ind].col = point.col + 1;
			++visit_stack_ind;
		}

		shape_data.area += 1;
	}

	edsa_htable_free(perim_cashe);
	return shape_data;
}

int main(void)
{
	int line_count = 0;
	plant input[BUF_SIZE][BUF_SIZE];

	char *input_line = NULL;
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		for(int i = 0; input_line[i] != '\n'; ++i){
			input[line_count][i].plant = input_line[i];
			input[line_count][i].visited = NOT_VISITED;
		}

		++line_count;
	}

	int total_price = 0;

	for(int i = 0; i < line_count; ++i){
		for(int j = 0; j < line_count; ++j){
			if(input[i][j].visited == NOT_VISITED){
				cord point;
				point.row = i;
				point.col = j;
				shape shape_data = get_shape_data(point, input, line_count);

				total_price += (shape_data.area * shape_data.perimeter);
			}
		}
	}

	printf("%d\n", total_price);

	free(input_line);
	return 0;
}
