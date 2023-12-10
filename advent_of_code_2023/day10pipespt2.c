/*
store the maze on a multidimensional array
mark b for boundry
step 1 remove all pipes that aren't part of the loop
step 2 run through the main path putting markers on the left and right side and counting the number of left and right turns
if right turns > left turns then the markers on the right side are inside and vice versa
this works because the winding number of a point inside is never 0
the winding number for any point is if on the right (right_turns - smaller_num_of_turns)/4 and if its on the left (left_turns - smaller_num_of_turns)/4
once all the borders are marked run through and expand the inner one to fill the empty space
run through again to count all the marked tiles
diagram of all pipe shapes:
*       *      *                   *
|  *-*  L*    *J    *7     F*     *S*
*                    *     *       *

*/
#include <stdio.h>
#include <string.h>

char mazemap[1000][1000];//mazemap[row][column]
char main_path_map[1000][1000];//map with only the main map

struct node{
	int row;
	int column;
};
struct node start_loc;

struct pathnode{
	int curr_row;
	int curr_column;
	int past_row;
	int past_column;
};

struct pathnode trav_node;

int left_turns = 0;
int right_turns = 0;

int main(void)
{
	char *line = NULL;
	size_t lim = 0;
	int curr_row = 0;//for putting line on mazemap
	int step_num = 0;//number of steps taken in maze
	int inside_count = 0;

	void findpathstart(struct pathnode *);
	void pathstep(struct pathnode *);
	void markpathstep(struct pathnode *);
	void putsym(int, int, char);

	for(int i = 0; i < 1000; ++i)
		for(int j = 0; j < 1000; ++j)
			mazemap[j][i] = 'b';

	++curr_row;

	while(getline(&line, &lim, stdin) > 1){
		int i;

		mazemap[curr_row][0] = 'b';//put buffer of dots
		for(i = 0; *(line + i) != '\n'; ++i){
			mazemap[curr_row][i + 1] = *(line + i);
			if(*(line + i) == 'S'){
				start_loc.row = curr_row;
				start_loc.column = i + 1;
			}
		}
		mazemap[curr_row][i + 1] = 'b';

		++curr_row;
	}

	for(int i = 0; i < 1000; ++i)
		for(int j = 0; j < 1000; ++j)
			main_path_map[j][i] = 'b';

	findpathstart(&trav_node);//gets the start paths for trav_node

	main_path_map[trav_node.curr_row][ trav_node.curr_column] = mazemap[trav_node.curr_row][trav_node.curr_column];

//	printf("%c %d %d\n", mazemap[start_loc.row][start_loc.column], start_loc.row, start_loc.column);
//	printf("%c %d %d\n\n", main_path_map[trav_node.curr_row][trav_node.curr_column], trav_node.curr_row, trav_node.curr_column);

	while(trav_node.curr_row != start_loc.row || trav_node.curr_column != start_loc.column){
		pathstep(&trav_node);
		main_path_map[trav_node.curr_row][ trav_node.curr_column] = mazemap[trav_node.curr_row][trav_node.curr_column];
	//	printf("%c %d %d %d\n", mazemap[trav_node.curr_row][trav_node.curr_column], trav_node.curr_row, trav_node.curr_column, step_num);
		++step_num;
	}

//	for(int i = 0; i < 500; ++i){
//		for(int j = 0; j < 500; ++j){
//			printf("%c", main_path_map[i][j]);
		//	printf("%c)", mazemap[i][j]);
//		}
//		printf("\n");
//	}

	findpathstart(&trav_node);

	while(trav_node.curr_row != start_loc.row || trav_node.curr_column != start_loc.column){
		markpathstep(&trav_node);
	//	printf("%c %d %d %d\n", mazemap[trav_node.curr_row][trav_node.curr_column], trav_node.curr_row, trav_node.curr_column, step_num);
	}

//	for(int i = 0; i < 500; ++i){
//		for(int j = 0; j < 500; ++j){
//			printf("%c", main_path_map[i][j]);
//		}
//		printf("\n");
//	}

	if(right_turns > left_turns){
		for(int i = 0; i < 1000; ++i){
			for(int j = 0; j < 1000; ++j){
				if(main_path_map[i][j] == 'r')
					putsym(i + 1, j, 'r');
			}
		}
	}
	else{
		for(int i = 0; i < 1000; ++i){
			for(int j = 0; j < 1000; ++j){
				if(main_path_map[i][j] == 'l')
					putsym(i + 1, j, 'l');
			}
		}
	}

	if(right_turns > left_turns){
		for(int i = 0; i < 1000; ++i){
			for(int j = 0; j < 1000; ++j){
				if(main_path_map[i][j] == 'r')
					++inside_count;
			}
		}
	}
	else{
		for(int i = 0; i < 1000; ++i){
			for(int j = 0; j < 1000; ++j){
				if(main_path_map[i][j] == 'l')
					++inside_count;
			}
		}
	}

	for(int i = 0; i < 500; ++i){
		for(int j = 0; j < 500; ++j){
			printf("%c", main_path_map[i][j]);
		}
		printf("\n");
	}

	printf("%d %d\n", right_turns, left_turns);
	printf("%d\n", inside_count);


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

//travles path marking nodes next to the input node and recording left and right turns
void markpathstep(struct pathnode *node)
{
	void putsym(int, int, char);
	int temp_next_row;
	int temp_next_column;

	switch (mazemap[node->curr_row][node->curr_column]){
		case '|':
		//	r|l going down
			if(node->curr_row > node->past_row){
				putsym(node->curr_row, node->curr_column - 1, 'r');
				putsym(node->curr_row, node->curr_column + 1, 'l');

				temp_next_row = node->curr_row + 1;
				temp_next_column = node->curr_column;
				node->past_row = node->curr_row;
				node->past_column = node->curr_column;
				node->curr_row = temp_next_row;
				node->curr_column = temp_next_column;
				return;
			}
		//	r|l going up
			putsym(node->curr_row, node->curr_column - 1, 'l');
			putsym(node->curr_row, node->curr_column + 1, 'r');

			temp_next_row = node->curr_row - 1;
			temp_next_column = node->curr_column;
			node->past_row = node->curr_row;
			node->past_column = node->curr_column;
			node->curr_row = temp_next_row;
			node->curr_column = temp_next_column;
			return;
		case '-':
		//	l going right
		//	-
		//	r
			if(node->curr_column > node->past_column){
				putsym(node->curr_row - 1, node->curr_column, 'l');
				putsym(node->curr_row + 1, node->curr_column, 'r');

				temp_next_row = node->curr_row;
				temp_next_column = node->curr_column + 1;
				node->past_row = node->curr_row;
				node->past_column = node->curr_column;
				node->curr_row = temp_next_row;
				node->curr_column = temp_next_column;
				return;
			}
		//	r going left
		//	-
		//	l
			putsym(node->curr_row - 1, node->curr_column, 'r');
			putsym(node->curr_row + 1, node->curr_column, 'l');

			temp_next_row = node->curr_row;
			temp_next_column = node->curr_column - 1;
			node->past_row = node->curr_row;
			node->past_column = node->curr_column;
			node->curr_row = temp_next_row;
			node->curr_column = temp_next_column;
			return;
		case 'L':
		//	left
		//	rL
		//	rr
			if(node->curr_row > node->past_row){
				putsym(node->curr_row + 1, node->curr_column, 'r');
				putsym(node->curr_row + 1, node->curr_column - 1, 'r');
				putsym(node->curr_row, node->curr_column - 1, 'r');

				++left_turns;

				temp_next_row = node->curr_row;
				temp_next_column = node->curr_column + 1;
				node->past_row = node->curr_row;
				node->past_column = node->curr_column;
				node->curr_row = temp_next_row;
				node->curr_column = temp_next_column;
				return;
			}
		//	right
		//	lL
		//	ll
			putsym(node->curr_row + 1, node->curr_column, 'l');
			putsym(node->curr_row + 1, node->curr_column - 1, 'l');
			putsym(node->curr_row, node->curr_column - 1, 'l');

			++right_turns;

			temp_next_row = node->curr_row - 1;
			temp_next_column = node->curr_column;
			node->past_row = node->curr_row;
			node->past_column = node->curr_column;
			node->curr_row = temp_next_row;
			node->curr_column = temp_next_column;
			return;
		case 'J':
		//	right
		//	Jl
		//	ll
			if(node->curr_row > node->past_row){
				putsym(node->curr_row + 1, node->curr_column, 'l');
				putsym(node->curr_row + 1, node->curr_column + 1, 'l');
				putsym(node->curr_row, node->curr_column + 1, 'l');

				++right_turns;

				temp_next_row = node->curr_row;
				temp_next_column = node->curr_column - 1;
				node->past_row = node->curr_row;
				node->past_column = node->curr_column;
				node->curr_row = temp_next_row;
				node->curr_column = temp_next_column;
				return;
			}
		//	left
		//	Jr
		//	rr
			putsym(node->curr_row + 1, node->curr_column, 'r');
			putsym(node->curr_row + 1, node->curr_column + 1, 'r');
			putsym(node->curr_row, node->curr_column + 1, 'r');

			++left_turns;

			temp_next_row = node->curr_row - 1;
			temp_next_column = node->curr_column;
			node->past_row = node->curr_row;
			node->past_column = node->curr_column;
			node->curr_row = temp_next_row;
			node->curr_column = temp_next_column;
			return;
		case 'F':
		//	right
		//	ll
		//	lF
			if(node->curr_row < node->past_row){
				putsym(node->curr_row - 1, node->curr_column, 'l');
				putsym(node->curr_row - 1, node->curr_column - 1, 'l');
				putsym(node->curr_row, node->curr_column - 1, 'l');

				++right_turns;

				temp_next_row = node->curr_row;
				temp_next_column = node->curr_column + 1;
				node->past_row = node->curr_row;
				node->past_column = node->curr_column;
				node->curr_row = temp_next_row;
				node->curr_column = temp_next_column;
				return;
			}
		//	left
		//	rr
		//	rF
			putsym(node->curr_row - 1, node->curr_column, 'r');
			putsym(node->curr_row - 1, node->curr_column - 1, 'r');
			putsym(node->curr_row, node->curr_column - 1, 'r');

			++left_turns;

			temp_next_row = node->curr_row + 1;
			temp_next_column = node->curr_column;
			node->past_row = node->curr_row;
			node->past_column = node->curr_column;
			node->curr_row = temp_next_row;
			node->curr_column = temp_next_column;
			return;
		case '7':
		//	left
		//	rr
		//	7r
			if(node->curr_row < node->past_row){
				putsym(node->curr_row - 1, node->curr_column, 'r');
				putsym(node->curr_row - 1, node->curr_column + 1, 'r');
				putsym(node->curr_row, node->curr_column + 1, 'r');

				++left_turns;

				temp_next_row = node->curr_row;
				temp_next_column = node->curr_column - 1;
				node->past_row = node->curr_row;
				node->past_column = node->curr_column;
				node->curr_row = temp_next_row;
				node->curr_column = temp_next_column;
				return;
			}
		//	right
		//	ll
		//	7l
			putsym(node->curr_row - 1, node->curr_column, 'l');
			putsym(node->curr_row - 1, node->curr_column + 1, 'l');
			putsym(node->curr_row, node->curr_column + 1, 'l');

			++right_turns;

			temp_next_row = node->curr_row + 1;
			temp_next_column = node->curr_column;
			node->past_row = node->curr_row;
			node->past_column = node->curr_column;
			node->curr_row = temp_next_row;
			node->curr_column = temp_next_column;
			return;
	}
}

//puts symbol on main_path_map at point if point has b
void putsym(int row, int column, char c)
{
	if(main_path_map[row][column] == 'b')
		main_path_map[row][column] = c;
}


// sets node to a valid start path
void findpathstart(struct pathnode *node)
{
	enum{
		UP = 0,
		RIGHT = 1,
		DOWN = 2,
		LEFT = 4
	};
	int past_check = UP;//stores weather you have checked path for the start
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
