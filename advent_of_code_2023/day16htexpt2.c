/*
have 2 2D arrays one to hold the mirrors and splitters and the other to hold where the energy has been
store each ray in a struct containing it's direction and origin point
for each beam look till it hits something that will change its direction
mark all space inbetween it
call the function on it's new direction(s)
count marked tiles
and run again for all starting positions
print the most energy got from any starting position
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum direction{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SPLIT
};

struct beam{
	int row;
	int col;
	int direct;
};

int memo[4][200][200];

char mirror_data[200][200];
char powered_tiles[200][200];

int row_length = 0;//number of rows in input
int col_length = 0;//number of columns in input

int main(void)
{
	char *line = NULL;
	size_t lim = 0;
	struct beam start_beam;
	long int num_of_pw_tile = 0;
	long int most_power = 0;

	start_beam.col = 0;
	start_beam.row = 0;
	start_beam.direct = RIGHT;

	void pr_pow(void);
	void pr_mir(void);
	void mark_pow(int, int, int, int);
	int find_block(int, int, int);
	void beam_logic(struct beam);
	int find_dir(int, int, int);
	void reset_pwd(void);
	void reset_memo(void);
	int count_pwd(void);

	//gets input
	while(getline(&line, &lim, stdin) > 1){
		strcpy(mirror_data[row_length++], line);
	}
	free(line);

	//gets col_length
	for(int i = 0; mirror_data[0][i] != '\n'; ++i)
		col_length = i + 1;

	//for right side starts
	for(int i = 0; i < row_length; ++i){
		reset_memo();
		reset_pwd();

		start_beam.direct = RIGHT;
		start_beam.row = i;
		start_beam.col = 0;

		start_beam.direct = find_dir(start_beam.direct, start_beam.row, start_beam.col);

		beam_logic(start_beam);

		num_of_pw_tile = count_pwd();

		if(num_of_pw_tile > most_power)
			most_power = num_of_pw_tile;
	}

	//for left side starts
	for(int i = 0; i < row_length; ++i){
		reset_memo();
		reset_pwd();

		start_beam.direct = LEFT;
		start_beam.row = i;
		start_beam.col = col_length - 1;

		start_beam.direct = find_dir(start_beam.direct, start_beam.row, start_beam.col);

		beam_logic(start_beam);

		num_of_pw_tile = count_pwd();

		if(num_of_pw_tile > most_power)
			most_power = num_of_pw_tile;
	}

	//for up side starts
	for(int i = 0; i < col_length; ++i){
		reset_memo();
		reset_pwd();

		start_beam.direct = UP;
		start_beam.row = row_length - 1;
		start_beam.col = i;

		start_beam.direct = find_dir(start_beam.direct, start_beam.row, start_beam.col);

		beam_logic(start_beam);

		num_of_pw_tile = count_pwd();

		if(num_of_pw_tile > most_power)
			most_power = num_of_pw_tile;
	}

	//for down side starts
	for(int i = 0; i < col_length; ++i){
		reset_memo();
		reset_pwd();

		start_beam.direct = DOWN;
		start_beam.row = 0;
		start_beam.col = i;

		start_beam.direct = find_dir(start_beam.direct, start_beam.row, start_beam.col);

		beam_logic(start_beam);

		num_of_pw_tile = count_pwd();

		if(num_of_pw_tile > most_power)
			most_power = num_of_pw_tile;
	}

	printf("%ld\n", most_power);
	return 0;
}

//calls mark_pow and handels splitting of beams and changing of directions
void beam_logic(struct beam input)
{
	struct beam temp_a;
	struct beam temp_b;
	if(input.direct == SPLIT){
		if(mirror_data[input.row][input.col] == '-'){
			temp_a.direct = RIGHT;
			temp_a.row = input.row;
			temp_a.col = input.col;
			temp_b.direct = LEFT;
			temp_b.row = input.row;
			temp_b.col = input.col;
			beam_logic(temp_a);
			beam_logic(temp_b);
		}
		else{
			temp_a.direct = UP;
			temp_a.row = input.row;
			temp_a.col = input.col;
			temp_b.direct = DOWN;
			temp_b.row = input.row;
			temp_b.col = input.col;
			beam_logic(temp_a);
			beam_logic(temp_b);
		}
		return;
	}
	if(memo[input.direct][input.row][input.col] == 1)
		return;
	memo[input.direct][input.row][input.col] = 1;

	void mark_pow(int, int, int, int);
	int find_dir(int, int, int);
	int find_block(int, int, int);
	void pr_pow(void);

	int new_cord = -1;
	int new_dir = -1;

	new_cord = find_block(input.direct, input.row, input.col);

	while(1){
		//if beam go off edge of input
		if(new_cord == -1){
			switch(input.direct){
				case UP:
					mark_pow(input.row, input.col, 0, input.col);
					break;
				case DOWN:
					mark_pow(input.row, input.col, row_length - 1, input.col);
					break;
				case RIGHT:
					mark_pow(input.row, input.col, input.row, col_length - 1);
					break;
				case LEFT:
					mark_pow(input.row, input.col, input.row, 0);
					break;
			}
			return;
		}
		if(input.direct == UP || input.direct == DOWN){
			new_dir = find_dir(input.direct, new_cord, input.col);
			if(new_dir != input.direct)
				break;
			else
				new_cord = find_block(input.direct, new_cord, input.col);
		}
		else{
			new_dir = find_dir(input.direct, input.row, new_cord);
			if(new_dir != input.direct)
				break;
			else
				new_cord = find_block(input.direct, input.row, new_cord);
		}
	}

	switch(input.direct){
		case UP:
			mark_pow(input.row, input.col, new_cord, input.col);
			break;
		case DOWN:
			mark_pow(input.row, input.col, new_cord, input.col);
			break;
		case RIGHT:
			mark_pow(input.row, input.col, input.row, new_cord);
			break;
		case LEFT:
			mark_pow(input.row, input.col, input.row, new_cord);
			break;
	}

	switch(new_dir){
		case UP:
			temp_a.direct = UP;
			temp_a.row = input.row;
			temp_a.col = new_cord;
			beam_logic(temp_a);
			break;
		case DOWN:
			temp_a.direct = DOWN;
			temp_a.row = input.row;
			temp_a.col = new_cord;
			beam_logic(temp_a);
			break;
		case RIGHT:
			temp_a.direct = RIGHT;
			temp_a.row = new_cord;
			temp_a.col = input.col;
			beam_logic(temp_a);
			break;
		case LEFT:
			temp_a.direct = LEFT;
			temp_a.row = new_cord;
			temp_a.col = input.col;
			beam_logic(temp_a);
			break;
		case SPLIT:
			if(input.direct == UP || input.direct == DOWN){
				temp_a.direct = RIGHT;
				temp_a.row = new_cord;
				temp_a.col = input.col;
				temp_b.direct = LEFT;
				temp_b.row = new_cord;
				temp_b.col = input.col;
				beam_logic(temp_a);
				beam_logic(temp_b);
			}
			else{
				temp_a.direct = UP;
				temp_a.row = input.row;
				temp_a.col = new_cord;
				temp_b.direct = DOWN;
				temp_b.row = input.row;
				temp_b.col = new_cord;
				beam_logic(temp_a);
				beam_logic(temp_b);
			}
			break;
	}
}

//finds first non '.' in path of beam starting form mirror_data[row][col] returns row or col depending on direction -1 value means beam it end
int find_block(int dir, int row, int col)
{
	switch(dir){
		case UP:
			for(int i = row - 1; i >= 0; --i)
				if(mirror_data[i][col] != '.')
					return i;
			return -1;
		case DOWN:
			for(int i = row + 1; i < row_length; ++i)
				if(mirror_data[i][col] != '.')
					return i;
			return -1;
		case LEFT:
			for(int i = col - 1; i >= 0; --i)
				if(mirror_data[row][i] != '.')
					return i;
			return -1;
		case RIGHT:
			for(int i = col + 1; i < col_length; ++i)
				if(mirror_data[row][i] != '.')
					return i;
			return -1;
	}
	return -1;
}

//returns how mirror_data[row][col] would effect beam going cur_dir returns UP, DOWN, LEFT, RIGHT or SPLIT
int find_dir(int cur_dir, int row, int col){
	switch(mirror_data[row][col]){
		case '.':
			return cur_dir;
		case '-':
			if(cur_dir == LEFT || cur_dir == RIGHT)
				return cur_dir;
			return SPLIT;
		case '|':
			if(cur_dir == UP || cur_dir == DOWN)
				return cur_dir;
			return SPLIT;
		case '\\':
			switch(cur_dir){
				case UP:
					return LEFT;
				case DOWN:
					return RIGHT;
				case LEFT:
					return UP;
				case RIGHT:
					return DOWN;
			}
		case '/':
			switch(cur_dir){
				case UP:
					return RIGHT;
				case DOWN:
					return LEFT;
				case LEFT:
					return DOWN;
				case RIGHT:
					return UP;
			}
	}
	return -1;
}

//marks from row_1, col_1 to row_2, col_2 as energized int powered_tiles inclusive
void mark_pow(int row_1, int col_1, int row_2, int col_2)
{
	if(col_1 == col_2){
		for(int i = 0; i <= abs(row_1 - row_2); ++i){
			if(row_1 > row_2)
				powered_tiles[i + row_2][col_1] = '#';
			else
				powered_tiles[i + row_1][col_1] = '#';
		}
	}
	else{
		for(int i = 0; i <= abs(col_1 - col_2); ++i){
			if(col_1 > col_2)
				powered_tiles[row_1][i + col_2] = '#';
			else
				powered_tiles[row_1][i + col_1] = '#';
		}
	}
}

//counts total '#' in powered_tiles
int count_pwd(void)
{
	int num_of_pw_tile = 0;

	for(int i = 0; i < row_length; ++i)
		for(int j = 0; j < col_length; ++j)
			if(powered_tiles[i][j] == '#')
				++num_of_pw_tile;

	return num_of_pw_tile;
}
//sets powered tiles to '.'
void reset_pwd(void)
{
	for(int i = 0; i < row_length; ++i)
		for(int j = 0; j < col_length; ++j)
			powered_tiles[i][j] = '.';
}

//sets memo = -1
void reset_memo(void)
{
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 200; ++j)
			for(int k = 0; k < 200; ++k)
				memo[i][j][k] = -1;
}

//prints powered tile array
void pr_pow(void)
{
	for(int i = 0; i < row_length; ++i){
		for(int j = 0; j < col_length; ++j)
			printf("%c", powered_tiles[i][j]);
		printf("\n");
	}
}

//prints mirror array
void pr_mir(void)
{
	for(int i = 0; i < row_length; ++i)
		printf("%s", mirror_data[i]);
}
