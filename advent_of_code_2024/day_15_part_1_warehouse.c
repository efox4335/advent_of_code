/*
 * just a simple simulation
*/

#include <stdio.h>
#include <stdlib.h>

enum{BUF_SIZE = 200};
enum{UP, DOWN, LEFT, RIGHT};

typedef struct{
	int row;
	int col;
}cord;

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
	int box_count = 0;

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
	int warehouse_size = 0;
	cord robot_pos;

	while(getline(&input_line, &lim, stdin) > 0){
		if(input_line[0] == '\n'){
			parse_part = MOVEMENTS;
			continue;
		}

		if(parse_part == WAREHOUSE){
			for(int i = 0; input_line[i] != '\n'; ++i){
				if(input_line[i] == '@'){
					robot_pos.row = warehouse_size;
					robot_pos.col = i;
				}
				warehouse[warehouse_size][i] = input_line[i];
			}

			++warehouse_size;
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
