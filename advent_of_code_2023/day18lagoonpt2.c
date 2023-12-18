/*
get inputs first five didgits of hex
get direction from last one
get all cords of corners
use sholace formula to get area
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum direction{
	UP,
	DOWN,
	EAST,
	WEST
};

enum inside_dir{
	LEFT,
	RIGHT
};

struct vert{
	long int row;
	long int col;
	char t_type;
	char u_dir;
	char d_dir;
	char e_dir;
	char w_dir;
};

struct instruction{
	long int dist;
	char direction;
	char prev_direction;
};

struct instruction build_instructions[1000];
struct vert vertex_array[10000];

int instruction_num = 0;
int vertex_ammount = 0;
int inside_side;

//lookup table for the incoming turn direction and outgoing turn direction to left or right turn
char turn_type[4][4] = {//incoming outgoing left or right turn
	{'e', 'e', 'r', 'l'},//for up {up, down, east, west}
	{'e', 'e', 'l', 'r'},//for down {up, down, east, west}
	{'l', 'r', 'e', 'e'},//for east {up, down, east, west}
	{'r', 'l', 'e', 'e'}//for west {up, down, east, west}
};

int main(void)
{
	void build(struct instruction *, long int *, long int *);
	void mark_dir(struct vert *, int);
	void fix_cords(struct vert *);
	void check_inside(void);
	void fix_cords(struct vert *vertex);
	long int ind_shoelace(struct vert *, struct vert *);

	char *line = NULL;
	size_t lim = 0;
	char *token = NULL;
	char *temp;
	char dist_num[10];
	long int working_row = 0;
	long int working_col = 0;
	long int shoelace_total = 0;


	while(getline(&line, &lim, stdin) > 1){
		token = strtok(line, " \n()#");
		token = strtok(NULL, " \n()#");
		token = strtok(NULL, " \n()#");
		strncpy(dist_num, token, 5);
		dist_num[5] = '\0';
		build_instructions[instruction_num].dist = strtol(dist_num, &temp, 16);
		switch(token[5]){
			case '0':
				build_instructions[instruction_num].direction = EAST;
				build_instructions[instruction_num + 1].prev_direction = EAST;
				break;
			case '1':
				build_instructions[instruction_num].direction = DOWN;
				build_instructions[instruction_num + 1].prev_direction = DOWN;
				break;
			case '2':
				build_instructions[instruction_num].direction = WEST;
				build_instructions[instruction_num + 1].prev_direction = WEST;
				break;
			case '3':
				build_instructions[instruction_num].direction = UP;
				build_instructions[instruction_num + 1].prev_direction = UP;
				break;
		}
		++instruction_num;
	}

	build_instructions[0].prev_direction = build_instructions[instruction_num].prev_direction;

	//gets the top left cords of each corner square
	for(int i = 0; i < instruction_num; ++i){
		mark_dir(&vertex_array[i], build_instructions[i].direction);
		mark_dir(&vertex_array[i], build_instructions[i].prev_direction);

		vertex_array[i].t_type = turn_type[build_instructions[i].prev_direction][build_instructions[i].direction];
		vertex_array[i].row = working_row;
		vertex_array[i].col = working_col;

		build(&build_instructions[i], &working_row, &working_col);
		++vertex_ammount;
	}

	check_inside();

	for(int i = 0; i < vertex_ammount; ++i){
		fix_cords(&vertex_array[i]);
	}

	shoelace_total += ind_shoelace(&vertex_array[vertex_ammount - 1], &vertex_array[0]);

	for(int i = 0; i < vertex_ammount - 1; ++i){
		shoelace_total += ind_shoelace(&vertex_array[i], &vertex_array[i + 1]);
	}

	printf("total area: %ld\n", labs(shoelace_total/2));

}

//move the cords on vertex_array to the outside of snape
void fix_cords(struct vert *vertex)
{
	if(inside_side == RIGHT){
		if(vertex->u_dir == 'l' && vertex->e_dir == 'l'){
			vertex->col += 1;
			return;
		}
		if(vertex->u_dir == 'l' && vertex->w_dir == 'l'){
			return;
		}
		if(vertex->d_dir == 'l' && vertex->e_dir == 'l'){
			vertex->row += 1;
			vertex->col += 1;
			return;
		}
		if(vertex->d_dir == 'l' && vertex->w_dir == 'l'){
			vertex->row += 1;
			return;
		}
	}
	else{
		if(vertex->u_dir == 'r' && vertex->e_dir == 'r'){
			vertex->col += 1;
			return;
		}
		if(vertex->u_dir == 'r' && vertex->w_dir == 'r'){
			return;
		}
		if(vertex->d_dir == 'r' && vertex->e_dir == 'r'){
			vertex->row += 1;
			vertex->col += 1;
			return;
		}
		if(vertex->d_dir == 'r' && vertex->w_dir == 'r'){
			vertex->row += 1;
			return;
		}
	}
}

//checks weather the right or left is on the inside of the shape
void check_inside(void)
{
	long int l_r_tally = 0;
	for(int i = 0; i < vertex_ammount; ++i){
		if(vertex_array[i].t_type == 'r')
			++l_r_tally;
		else
			--l_r_tally;
	}

	if(l_r_tally > 0)
		inside_side = RIGHT;
	else
		inside_side = LEFT;
}

//gets the cords of the top right of each corner
void build(struct instruction *ins, long int *row, long int *col)
{
	long int temp_mod_cord;
	switch(ins->direction){
		case UP:
			for(long int i = 0; i < ins->dist; ++i){
				temp_mod_cord = *row - i - 1;
			}
			*row = temp_mod_cord;
			return;
		case DOWN:
			for(long int i = 0; i < ins->dist; ++i){
				temp_mod_cord = *row + i + 1;
			}
			*row = temp_mod_cord;
			return;
		case WEST:
			for(long int i = 0; i < ins->dist; ++i){
				temp_mod_cord = *col - i - 1;
			}
			*col = temp_mod_cord;
			return;
		case EAST:
			for(long int i = 0; i < ins->dist; ++i){
				temp_mod_cord = *col + i + 1;
			}
			*col = temp_mod_cord;
			return;
	}
}

//marks the direction each line was going when it was intersected
void mark_dir(struct vert *ver, int dir)
{
	switch(dir){
		case UP:
			ver->e_dir = 'r';
			ver->w_dir = 'l';
			return;
		case DOWN:
			ver->w_dir = 'r';
			ver->e_dir = 'l';
			return;
		case EAST:
			ver->d_dir = 'r';
			ver->u_dir = 'l';
			return;
		case WEST:
			ver->u_dir = 'r';
			ver->d_dir = 'l';
			return;
	}
}

//does an individual shoelace step
long int ind_shoelace(struct vert *ver_1, struct vert *ver_2)
{
	double temp_a = (ver_1->row) * (ver_2->col);
	double temp_b = (ver_1->col) * (ver_2->row);

	return (long int) temp_a - (long int) temp_b;
}
