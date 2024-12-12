/*
 * for a polygon made of squares where you have points in the center of each edge
 * the area is equal to the shoelace theorm + 1/2 perimeter - 1
 * which is picks theorm
 * when processing each regon each plant within it will be marked to avoid processing it twice
*/
#include <stdio.h>
#include <stdlib.h>
#include "include/edsa.h"

enum{VISITED, NOT_VISITED};
enum{BUF_SIZE = 200};

typedef struct{
	char plant;
	int visited;
}plant;

typedef struct{
	int row;
	int col;
}cord;

//starts from point
//returns perimeter and sets polygon to a list of points along the edges including inner corners
//simulates walking perimeter by keeping track of each plant + edge if a plant + edge has already been seen the perimeter is found
//stores all plant + edge combinations in a hash table
//explores clockwise
int get_perimeter(const cord point, cord *restrict polygon, plant input[BUF_SIZE][BUF_SIZE], const int line_count)
{
	enum{TOP, BOTTOM, LEFT, RIGHT};

	typedef struct{
		cord loc;
		int dir;
	}plant_dir;

	int perimeter = 0;
	int polygon_index = 0;

	edsa_htable *seen_before = NULL;
	edsa_htable_init(&seen_before, sizeof(plant_dir), sizeof(int), 100);

	int cur_edge_dir;
	if(point.row == 0 || input[point.row][point.col].plant != input[point.row - 1][point.col].plant){
		cur_edge_dir = TOP;
	}else if(point.row == line_count - 1 || input[point.row][point.col].plant != input[point.row + 1][point.col].plant){
		cur_edge_dir = BOTTOM;
	}else if(point.col == 0 || input[point.row][point.col].plant != input[point.row][point.col - 1].plant){
		cur_edge_dir = LEFT;
	}else if(point.col == line_count - 1 || input[point.row][point.col].plant != input[point.row][point.col + 1].plant){
		cur_edge_dir = RIGHT;
	}else{
		printf("error passed non edge point %d %d\n", point.row, point.col);
		exit(1);
	}

	int temp = 1;
	plant_dir seen_key;
	seen_key.loc.row = point.row;
	seen_key.loc.col = point.col;
	seen_key.dir = cur_edge_dir;

	do{
		edsa_htable_ins(seen_before, &seen_key, &temp);
		input[seen_key.loc.row][seen_key.loc.col].visited = VISITED;

		polygon[polygon_index].row = seen_key.loc.row;
		polygon[polygon_index].col = seen_key.loc.col;

		++polygon_index;

		polygon[polygon_index].row = -1;
		polygon[polygon_index].col = -1;

		plant_dir next_plant;

		switch(seen_key.dir){
		case TOP:
			//outer corner
			if(seen_key.loc.col == line_count - 1 ||
			input[seen_key.loc.row][seen_key.loc.col].plant != input[seen_key.loc.row][seen_key.loc.col + 1].plant){//if on the edge of input or if on the edge of shape
				next_plant.dir = RIGHT;
				next_plant.loc.row = seen_key.loc.row;
				next_plant.loc.col = seen_key.loc.col;

				--polygon_index;//avoids double counting the same square
			//line
			}else if(seen_key.loc.row == 0 ||
			input[seen_key.loc.row][seen_key.loc.col].plant != input[seen_key.loc.row - 1][seen_key.loc.col + 1].plant){//no need to check neighbor as we already know it matches
				next_plant.dir = TOP;
				next_plant.loc.row = seen_key.loc.row;
				next_plant.loc.col = seen_key.loc.col + 1;
			//inner corner
			}else{
				polygon[polygon_index].row = seen_key.loc.row;
				polygon[polygon_index].col = seen_key.loc.col + 1;

				++polygon_index;

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

				--polygon_index;//avoids double counting the same square
			//line
			}else if(seen_key.loc.row == line_count - 1 ||
			input[seen_key.loc.row][seen_key.loc.col].plant != input[seen_key.loc.row + 1][seen_key.loc.col - 1].plant){//no need to check neighbor as we already know it matches
				next_plant.dir = BOTTOM;
				next_plant.loc.row = seen_key.loc.row;
				next_plant.loc.col = seen_key.loc.col - 1;
			//inner corner
			}else{
				polygon[polygon_index].row = seen_key.loc.row;
				polygon[polygon_index].col = seen_key.loc.col - 1;

				++polygon_index;

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

				--polygon_index;//avoids double counting the same square
			//line
			}else if(seen_key.loc.col == 0 ||
			input[seen_key.loc.row][seen_key.loc.col].plant != input[seen_key.loc.row - 1][seen_key.loc.col - 1].plant){//no need to check neighbor as we already know it matches
				next_plant.dir = LEFT;
				next_plant.loc.row = seen_key.loc.row - 1;
				next_plant.loc.col = seen_key.loc.col;
			//inner corner
			}else{
				polygon[polygon_index].row = seen_key.loc.row - 1;
				polygon[polygon_index].col = seen_key.loc.col;

				++polygon_index;

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

				--polygon_index;//avoids double counting the same square
			//line
			}else if(seen_key.loc.col == line_count - 1 ||
			input[seen_key.loc.row][seen_key.loc.col].plant != input[seen_key.loc.row + 1][seen_key.loc.col + 1].plant){//no need to check neighbor as we already know it matches
				next_plant.dir = RIGHT;
				next_plant.loc.row = seen_key.loc.row + 1;
				next_plant.loc.col = seen_key.loc.col;
			//inner corner
			}else{
				polygon[polygon_index].row = seen_key.loc.row + 1;
				polygon[polygon_index].col = seen_key.loc.col;

				++polygon_index;

				next_plant.dir = TOP;
				next_plant.loc.row = seen_key.loc.row + 1;
				next_plant.loc.col = seen_key.loc.col + 1;
			}
			break;
		}

		++perimeter;
		seen_key.loc.row = next_plant.loc.row;
		seen_key.loc.col = next_plant.loc.col;
		seen_key.dir = next_plant.dir;
	}while(edsa_htable_read(seen_before, &seen_key, &temp) != EDSA_SUCCESS);

	edsa_htable_free(seen_before);
	return perimeter;
}

//-1 acts as the sentinal value
//returns the area from the shorlace fromula
int shoelace(cord *polygon)
{
	int area = 0;
	int index = 0;

	for(index = 0; polygon[index + 1].row != -1; ++index){
		area += (polygon[index].row * polygon[index + 1].col) - (polygon[index].col * polygon[index + 1].row);
	}

	area += (polygon[index].row * polygon[0].col) - (polygon[index].col * polygon[0].row);

	area = abs(area);

	return area >> 1;
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

	char type = '\n';//used to skip past already seen types
	int total_price = 0;

	for(int i = 0; i < line_count; ++i){
		for(int j = 0; j < line_count; ++j){
			if(type == input[i][j].plant || input[i][j].visited == VISITED){
				type = input[i][j].plant;
				continue;
			}
			cord polygon[BUF_SIZE];
			cord point;
			point.row = i;
			point.col = j;

			int perimeter = get_perimeter(point, polygon, input, line_count);
			total_price += (shoelace(polygon) + (perimeter / 2) - 1) * perimeter;

			type = input[i][j].plant;
		}

		type = '\n';//to avoid excluding where a line ends with the same type of plant another line ends with
	}

	printf("%d\n", total_price);

	free(input_line);
	return 0;
}
