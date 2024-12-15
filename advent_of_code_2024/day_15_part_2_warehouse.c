/*
 * for each box that is moved check the cords in front of it
 * 	if there is a '#' no move happens
 * 	if there is another box check what's in front of it
 * 	if there is a '.' the move can happen
 * if the check passes for all boxes the move can happen
*/
#include <stdio.h>
#include <stdlib.h>

enum{BUF_SIZE = 200};
enum{UP, DOWN, LEFT, RIGHT};
enum{BOX, WALL, EMPTY};

typedef struct{
	int row;
	int col;
}cord;

//moves box at box_pos dir without checking for collisions
void move_box(char warehouse[BUF_SIZE][BUF_SIZE], cord box_pos, int dir)
{
	if(warehouse[box_pos.row][box_pos.col] != '[' && warehouse[box_pos.row][box_pos.col] != ']'){
		printf("error tried to move non box at %d %d\n", box_pos.row, box_pos.col);
		exit(1);
	}

	switch(dir){
	case UP:
		if(warehouse[box_pos.row][box_pos.col] == '['){
			warehouse[box_pos.row][box_pos.col] = '.';
			warehouse[box_pos.row][box_pos.col + 1] = '.';
			warehouse[box_pos.row - 1][box_pos.col] = '[';
			warehouse[box_pos.row - 1][box_pos.col + 1] = ']';
		}else{
			warehouse[box_pos.row][box_pos.col] = '.';
			warehouse[box_pos.row][box_pos.col - 1] = '.';
			warehouse[box_pos.row - 1][box_pos.col - 1] = '[';
			warehouse[box_pos.row - 1][box_pos.col] = ']';
		}
		break;
	case DOWN:
		if(warehouse[box_pos.row][box_pos.col] == '['){
			warehouse[box_pos.row][box_pos.col] = '.';
			warehouse[box_pos.row][box_pos.col + 1] = '.';
			warehouse[box_pos.row + 1][box_pos.col] = '[';
			warehouse[box_pos.row + 1][box_pos.col + 1] = ']';
		}else{
			warehouse[box_pos.row][box_pos.col] = '.';
			warehouse[box_pos.row][box_pos.col - 1] = '.';
			warehouse[box_pos.row + 1][box_pos.col - 1] = '[';
			warehouse[box_pos.row + 1][box_pos.col] = ']';
		}
		break;
	case LEFT:
		if(warehouse[box_pos.row][box_pos.col] == '['){
			warehouse[box_pos.row][box_pos.col] = ']';
			warehouse[box_pos.row][box_pos.col + 1] = '.';
			warehouse[box_pos.row][box_pos.col - 1] = '[';
		}else{
			warehouse[box_pos.row][box_pos.col] = '.';
			warehouse[box_pos.row][box_pos.col - 1] = ']';
			warehouse[box_pos.row][box_pos.col - 2] = '[';
		}
		break;
	case RIGHT:
		if(warehouse[box_pos.row][box_pos.col] == '['){
			warehouse[box_pos.row][box_pos.col] = '.';
			warehouse[box_pos.row][box_pos.col + 1] = '[';
			warehouse[box_pos.row][box_pos.col + 2] = ']';
		}else{
			warehouse[box_pos.row][box_pos.col] = '[';
			warehouse[box_pos.row][box_pos.col - 1] = '.';
			warehouse[box_pos.row][box_pos.col + 1] = ']';
		}
		break;
	}
}

/*
 * returns EMPTY if there is nothing blocking the box from moving
 * returns BOX if there is a box where it wants to move puts the cords of the box(s) in child_box_pos
 * returns WALL if there is a wall blocking the way
 * -1 is the sentinal value if there is only one child box
*/
int can_move(char warehouse[BUF_SIZE][BUF_SIZE], cord box_pos, int dir, cord child_box_pos[3])
{
	if(warehouse[box_pos.row][box_pos.col] != '[' && warehouse[box_pos.row][box_pos.col] != ']'){
		printf("error tried to check non box at %d %d\n", box_pos.row, box_pos.col);
		exit(1);
	}

	cord check_cords[2];

	switch(dir){
	case UP:
		if(warehouse[box_pos.row][box_pos.col] == '['){
			check_cords[0].row = box_pos.row - 1;
			check_cords[0].col = box_pos.col;
			check_cords[1].row = box_pos.row - 1;
			check_cords[1].col = box_pos.col + 1;
		}else{
			check_cords[0].row = box_pos.row - 1;
			check_cords[0].col = box_pos.col;
			check_cords[1].row = box_pos.row - 1;
			check_cords[1].col = box_pos.col - 1;
		}
		break;
	case DOWN:
		if(warehouse[box_pos.row][box_pos.col] == '['){
			check_cords[0].row = box_pos.row + 1;
			check_cords[0].col = box_pos.col;
			check_cords[1].row = box_pos.row + 1;
			check_cords[1].col = box_pos.col + 1;
		}else{
			check_cords[0].row = box_pos.row + 1;
			check_cords[0].col = box_pos.col;
			check_cords[1].row = box_pos.row + 1;
			check_cords[1].col = box_pos.col - 1;
		}
		break;
	case LEFT:
		if(warehouse[box_pos.row][box_pos.col] == '['){
			check_cords[0].row = box_pos.row;
			check_cords[0].col = box_pos.col - 1;
			check_cords[1].row = -1;
			check_cords[1].col = -1;
		}else{
			check_cords[0].row = box_pos.row;
			check_cords[0].col = box_pos.col - 2;
			check_cords[1].row = -1;
			check_cords[1].col = -1;
		}
		break;
	case RIGHT:
		if(warehouse[box_pos.row][box_pos.col] == '['){
			check_cords[0].row = box_pos.row;
			check_cords[0].col = box_pos.col + 2;
			check_cords[1].row = -1;
			check_cords[1].col = -1;
		}else{
			check_cords[0].row = box_pos.row;
			check_cords[0].col = box_pos.col + 1;
			check_cords[1].row = -1;
			check_cords[1].col = -1;
		}
		break;
	}

	int child_box_count = 0;
	int ret_val = EMPTY;

	for(int i = 0; i < 2 && check_cords[i].row != -1; ++i){
		switch(warehouse[check_cords[i].row][check_cords[i].col]){
		case '#':
			return WALL;
		case '[':
			/*
			 * keeps from double counting case
			 * 	[]
			 * 	[]
			 * because case
			 * 	[[
			 * 	[]
			 * can't happen the only way for the check to pass is with the above case
			*/
			if(i != 0 && check_cords[0].col > check_cords[i].col){
				break;
			}
			child_box_pos[child_box_count] = check_cords[i];
			++child_box_count;
			child_box_pos[child_box_count].row = -1;
			child_box_pos[child_box_count].col = -1;
			ret_val = BOX;
			break;
		case ']':
			/*
			 * keeps from double counting case
			 * 	[]
			 * 	[]
			 * because case
			 * 	]]
			 * 	[]
			 * can't happen the only way for the check to pass is with the above case
			*/
			if(i != 0 && check_cords[0].col < check_cords[i].col){
				break;
			}
			child_box_pos[child_box_count] = check_cords[i];
			++child_box_count;
			child_box_pos[child_box_count].row = -1;
			child_box_pos[child_box_count].col = -1;
			ret_val = BOX;
			break;
		case '.':
		default:
		}
	}

	return ret_val;
}

/*
 * simulates moving boxes and the robot
 * goes in dir marking everything as '.' keeping track of how many 'O' it comes accross till it hits '#'
 * then it will write that amount of 'O' than the robot and update the robot_pos
*/
void sim_move(char warehouse[BUF_SIZE][BUF_SIZE], cord *robot_pos, const int dir)
{
	int row_dif = 0;
	int col_dif = 0;

	switch(dir){
		case UP:
			row_dif = -1;
			break;
		case DOWN:
			row_dif = 1;
			break;
		case LEFT:
			col_dif = -1;
			break;
		case RIGHT:
			col_dif = 1;
			break;
	}

	cord cur_pos = *robot_pos;

	while(warehouse[cur_pos.row][cur_pos.col] != '.'){
		//no free space to move to
		if(warehouse[cur_pos.row][cur_pos.col] == '#'){
			return;
		}
		cur_pos.row += row_dif;
		cur_pos.col += col_dif;
	}

	warehouse[robot_pos->row][robot_pos->col] = '.';

	warehouse[cur_pos.row][cur_pos.col] = 'O';

	robot_pos->row += row_dif;
	robot_pos->col += col_dif;

	warehouse[robot_pos->row][robot_pos->col] = '@';
}

int main(void)
{
	enum{WAREHOUSE, MOVEMENTS};

	char *input_line = NULL;
	size_t lim = 0;

	char warehouse[BUF_SIZE][BUF_SIZE];
	int move_arr[70000];
	int move_arr_len = 0;
	int parse_part = WAREHOUSE;
	int warehouse_row_count = 0;
	int warehouse_col_count = 0;
	cord robot_pos;

	while(getline(&input_line, &lim, stdin) > 0){
		if(input_line[0] == '\n'){
			parse_part = MOVEMENTS;
			continue;
		}

		if(parse_part == WAREHOUSE){
			warehouse_col_count = 0;

			for(int i = 0; input_line[i] != '\n'; ++i){
				if(input_line[i] == '@'){
					robot_pos.row = warehouse_row_count;
					robot_pos.col = warehouse_col_count;
				}

				switch(input_line[i]){
				case '.':
					warehouse[warehouse_row_count][warehouse_col_count] = '.';
					warehouse[warehouse_row_count][warehouse_col_count + 1] = '.';

					warehouse_col_count += 2;
					break;
				case '#':
					warehouse[warehouse_row_count][warehouse_col_count] = '#';
					warehouse[warehouse_row_count][warehouse_col_count + 1] = '#';

					warehouse_col_count += 2;
					break;
				case '@':
					warehouse[warehouse_row_count][warehouse_col_count] = '@';
					warehouse[warehouse_row_count][warehouse_col_count + 1] = '.';

					warehouse_col_count += 2;
					break;
				case 'O':
					warehouse[warehouse_row_count][warehouse_col_count] = '[';
					warehouse[warehouse_row_count][warehouse_col_count + 1] = ']';

					warehouse_col_count += 2;
					break;
				}
			}

			++warehouse_row_count;
		}else{
			for(int i = 0; input_line[i] != '\n'; ++i){
				switch(input_line[i]){
				case '^':
					move_arr[move_arr_len] = UP;
					++move_arr_len;
					break;
				case 'v':
					move_arr[move_arr_len] = DOWN;
					++move_arr_len;
					break;
				case '<':
					move_arr[move_arr_len] = LEFT;
					++move_arr_len;
					break;
				case '>':
					move_arr[move_arr_len] = RIGHT;
					++move_arr_len;
					break;
				default:
					printf("unrecognized char %c %d", input_line[i], input_line[i]);
					break;
				}
			}
		}
	}

	free(input_line);
	return 0;
}
