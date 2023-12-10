/*
store the maze on a multidimensional array
the only diverging path is the start so just follow all starting paths till we hit the same tile and print the number of steps
the starting path only has 2 ways out so we can just store the location of 2 paths as we travel them and end when they are on the same tile
diagram of all pipe shapes:
*       *      *                   *
|  *-*  L*    *J    *7     F*     *S*
*                    *     *       *

*/
#include <stdio.h>
#include <string.h>

char mazemap[1000][1000];//mazemap[row][column]

struct{
	int row;
	int column;
}start_loc;

struct pathnode{
	int curr_row;
	int curr_column;
	int past_row;
	int past_column;
};

struct pathnode path_1;
struct pathnode path_2;

int main(void)
{
	char *line = NULL;
	size_t lim = 0;
	int curr_row = 0;//for putting line on mazemap
	int step_num = 1;//number of steps taken in maze

	void findpathstart(struct pathnode *);
	void pathstep(struct pathnode *);

	for(int i = 0; i < 1000; ++i)
		for(int j = 0; j < 1000; ++j)
			mazemap[j][i] = '.';

	++curr_row;

	while(getline(&line, &lim, stdin) > 1){
		int i;

		mazemap[curr_row][0] = '.';//put buffer of dots
		for(i = 0; *(line + i) != '\n'; ++i){
			mazemap[curr_row][i + 1] = *(line + i);
			if(*(line + i) == 'S'){
				start_loc.row = curr_row;
				start_loc.column = i + 1;
			}
		}

		++curr_row;
	}

	findpathstart(&path_1);//gets the start paths for path 1 and 2
	findpathstart(&path_2);

	printf("%c %d %d\n", mazemap[start_loc.row][start_loc.column], start_loc.row, start_loc.column);
	printf("%c %d %d\n", mazemap[path_1.curr_row][path_1.curr_column], path_1.curr_row, path_1.curr_column);
	printf("%c %d %d\n\n", mazemap[path_2.curr_row][path_2.curr_column], path_2.curr_row, path_2.curr_column);

	while(path_1.curr_row != path_2.curr_row || path_1.curr_column != path_2.curr_column){
		++step_num;
		pathstep(&path_1);
		pathstep(&path_2);
		printf("%c %d %d\n", mazemap[path_1.curr_row][path_1.curr_column], path_1.curr_row, path_1.curr_column);
		printf("%c %d %d\n", mazemap[path_2.curr_row][path_2.curr_column], path_2.curr_row, path_2.curr_column);
	}

	printf("%d\n", step_num);


	return 0;
}

//takes the value at curr_row/column and uses the value at past_row/column to find the next node and puts the node there
void pathstep(struct pathnode *node)
{
	int temp_next_row;
	int temp_next_column;

	switch (mazemap[node->curr_row][node->curr_column]){
		case '|':
			if(node->curr_row > node->past_row){
				temp_next_row = node->curr_row + 1;
				temp_next_column = node->curr_column;
				node->past_row = node->curr_row;
				node->past_column = node->curr_column;
				node->curr_row = temp_next_row;
				node->curr_column = temp_next_column;
				return;
			}
			temp_next_row = node->curr_row - 1;
			temp_next_column = node->curr_column;
			node->past_row = node->curr_row;
			node->past_column = node->curr_column;
			node->curr_row = temp_next_row;
			node->curr_column = temp_next_column;
			return;
		case '-':
			if(node->curr_column > node->past_column){
				temp_next_row = node->curr_row;
				temp_next_column = node->curr_column + 1;
				node->past_row = node->curr_row;
				node->past_column = node->curr_column;
				node->curr_row = temp_next_row;
				node->curr_column = temp_next_column;
				return;
			}
			temp_next_row = node->curr_row;
			temp_next_column = node->curr_column - 1;
			node->past_row = node->curr_row;
			node->past_column = node->curr_column;
			node->curr_row = temp_next_row;
			node->curr_column = temp_next_column;
			return;
		case 'L':
			if(node->curr_row > node->past_row){
				temp_next_row = node->curr_row;
				temp_next_column = node->curr_column + 1;
				node->past_row = node->curr_row;
				node->past_column = node->curr_column;
				node->curr_row = temp_next_row;
				node->curr_column = temp_next_column;
				return;
			}
			temp_next_row = node->curr_row - 1;
			temp_next_column = node->curr_column;
			node->past_row = node->curr_row;
			node->past_column = node->curr_column;
			node->curr_row = temp_next_row;
			node->curr_column = temp_next_column;
			return;
		case 'J':
			if(node->curr_row > node->past_row){
				temp_next_row = node->curr_row;
				temp_next_column = node->curr_column - 1;
				node->past_row = node->curr_row;
				node->past_column = node->curr_column;
				node->curr_row = temp_next_row;
				node->curr_column = temp_next_column;
				return;
			}
			temp_next_row = node->curr_row - 1;
			temp_next_column = node->curr_column;
			node->past_row = node->curr_row;
			node->past_column = node->curr_column;
			node->curr_row = temp_next_row;
			node->curr_column = temp_next_column;
			return;
		case 'F':
			if(node->curr_row < node->past_row){
				temp_next_row = node->curr_row;
				temp_next_column = node->curr_column + 1;
				node->past_row = node->curr_row;
				node->past_column = node->curr_column;
				node->curr_row = temp_next_row;
				node->curr_column = temp_next_column;
				return;
			}
			temp_next_row = node->curr_row + 1;
			temp_next_column = node->curr_column;
			node->past_row = node->curr_row;
			node->past_column = node->curr_column;
			node->curr_row = temp_next_row;
			node->curr_column = temp_next_column;
			return;
		case '7':
			if(node->curr_row < node->past_row){
				temp_next_row = node->curr_row;
				temp_next_column = node->curr_column - 1;
				node->past_row = node->curr_row;
				node->past_column = node->curr_column;
				node->curr_row = temp_next_row;
				node->curr_column = temp_next_column;
				return;
			}
			temp_next_row = node->curr_row + 1;
			temp_next_column = node->curr_column;
			node->past_row = node->curr_row;
			node->past_column = node->curr_column;
			node->curr_row = temp_next_row;
			node->curr_column = temp_next_column;
			return;
	}
}

// checks for paths out of start keeps track of checked paths with static variable
void findpathstart(struct pathnode *node)
{
	enum{
		UP = 0,
		RIGHT = 1,
		DOWN = 2,
		LEFT = 4
	};
	static int past_check = UP;//stores weather you have checked path for the start
	char c;//stores the location of a path

	while(past_check < 5){
		switch(past_check){//scans nodes clockwise for nodes that connect to the start
			case UP:
				if((c = mazemap[start_loc.row - 1][start_loc.column]) == '|' || c == '7' || c == 'F'){
					node->curr_row = start_loc.row - 1;
					node->curr_column = start_loc.column;
					node->past_row = start_loc.row;
					node->past_column = start_loc.column;
					past_check = RIGHT;
					return;
				}
				past_check = RIGHT;
				break;
			case RIGHT:
				if((c = mazemap[start_loc.row][start_loc.column + 1]) == '-' || c == 'J' || c == '7'){
					node->curr_row = start_loc.row;
					node->curr_column = start_loc.column + 1;
					node->past_row = start_loc.row;
					node->past_column = start_loc.column;
					past_check = DOWN;
					return;
				}
				past_check = DOWN;
				break;
			case DOWN:
				if((c = mazemap[start_loc.row + 1][start_loc.column]) == '|' || c == 'L' || c == 'J'){
					node->curr_row = start_loc.row + 1;
					node->curr_column = start_loc.column;
					node->past_row = start_loc.row;
					node->past_column = start_loc.column;
					past_check = LEFT;
					return;
				}
				past_check = LEFT;
				break;
			case LEFT:
				if((c = mazemap[start_loc.row][start_loc.column - 1]) == '-' || c == 'F' || c == 'L'){
					node->curr_row = start_loc.row;
					node->curr_column = start_loc.column - 1;
					node->past_row = start_loc.row;
					node->past_column = start_loc.column;
					past_check = 5;
					return;
				}
				past_check = 5;
				printf("error: no path found\n");
				break;
		}
	}
}
