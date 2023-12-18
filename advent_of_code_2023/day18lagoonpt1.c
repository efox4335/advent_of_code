/*
take input
make outline of trench based on it in graph
flood fill from border of graph
count all non filled squares
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ROWS 1000
#define MAX_COLS 1000

struct instruction{
	int dist;
	char direction;
};

char build_area[MAX_ROWS][MAX_COLS];
struct instruction build_instructions[1000];

int instruction_num = 0;

int main(void)
{
	void print_build(void);
	void reset_build(void);
	void build(struct instruction *, int *, int *);
	int flood_call(void);
	int count_not_flooded(void);

	char *line = NULL;
	size_t lim = 0;
	char *token = NULL;
	int working_row = (MAX_ROWS / 2) - 1;
	int working_col = (MAX_COLS / 2) - 1;
	int not_flooded = 0;

	while((getline(&line, &lim, stdin)) > 1){
		token = strtok(line, " ");
		build_instructions[instruction_num].direction = *token;
		token = strtok(NULL, " ");
		build_instructions[instruction_num].dist = atoi(token);
		++instruction_num;
	}

	reset_build();

	for(int i = 0; i <instruction_num; ++i)
		build(&build_instructions[i], &working_row, &working_col);

	build_area[0][0] = 'f';

	while(flood_call() != 0);

	not_flooded = count_not_flooded();
//	print_build();

	printf("%d\n", not_flooded);

	return 0;
}

//runs through array and calls flood wraper for each square an already flooded square is found
//returns > 0 for newly flooded squares
int flood_call(void)
{
	int flood_wraper(int, int);

	int num_flooded = 0;

	for(int i = 0; i < MAX_ROWS; ++i){
		for(int j = 0; j < MAX_COLS; ++j){
			if(build_area[i][j] == 'f'){
				num_flooded += flood_wraper(i, j);
			}
		}
	}
	return num_flooded;
}

//calls add_square for all four squares around it
//checks if square is empty
//returns number of floded squares
int flood_wraper(int row, int col)
{
	int add_square(int, int, char);

	int num_sucess = 0;
	if(build_area[row - 1][col] == '.')
		num_sucess += add_square(row - 1, col, 'f');
	if(build_area[row + 1][col] == '.')
		num_sucess += add_square(row + 1, col, 'f');
	if(build_area[row][col + 1] == '.')
		num_sucess += add_square(row, col + 1, 'f');
	if(build_area[row][col - 1] == '.')
		num_sucess += add_square(row, col - 1, 'f');

	return num_sucess;
}

//calls add_square with the spesfied number of times in build_instructions at row and col
//updates row and col to new pos
void build(struct instruction *ins, int *row, int *col)
{
	int temp_mod_cord;
	int add_square(int, int, char);
	switch(ins->direction){
		case 'U':
			add_square(*row, *col, '#');
			for(int i = 0; i < ins->dist; ++i){
				add_square(*row - i - 1, *col, '#');
				temp_mod_cord = *row - i - 1;
			}
			*row = temp_mod_cord;
			return;
		case 'D':
			add_square(*row, *col, '#');
			for(int i = 0; i < ins->dist; ++i){
				add_square(*row + i + 1, *col, '#');
				temp_mod_cord = *row + i + 1;
			}
			*row = temp_mod_cord;
			return;
		case 'L':
			add_square(*row, *col, '#');
			for(int i = 0; i < ins->dist; ++i){
				add_square(*row, *col - i - 1, '#');
				temp_mod_cord = *col - i - 1;
			}
			*col = temp_mod_cord;
			return;
		case 'R':
			add_square(*row, *col, '#');
			for(int i = 0; i < ins->dist; ++i){
				add_square(*row, *col + i + 1, '#');
				temp_mod_cord = *col + i + 1;
			}
			*col = temp_mod_cord;
			return;
	}
}

//adds c to build_area[row][col] if row and col are valid
//returns 1 on successful add
int add_square(int row, int col, char c)
{
	if(row < 0 || col < 0 || row >= MAX_ROWS || col >= MAX_COLS)
		return 0;
	build_area[row][col] = c;
	return 1;
}

//prints build_area
void print_build(void)
{
	for(int i = 0; i < MAX_ROWS; ++i){
		for(int j = 0; j < MAX_COLS; ++j)
			printf("%c", build_area[i][j]);
		printf("\n");
	}
}

//sets build_area to all '.'
void reset_build(void)
{
	for(int i = 0; i < MAX_ROWS; ++i)
		for(int j = 0; j < MAX_COLS; ++j)
			build_area[i][j] = '.';
}

//counts all non flooded squares
int count_not_flooded(void)
{
	int not_flooded = 0;

	for(int i = 0; i < MAX_ROWS; ++i)
		for(int j = 0; j < MAX_COLS; ++j)
			if(build_area[i][j] != 'f')
				++not_flooded;

	return not_flooded;
}
