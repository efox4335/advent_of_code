//get input
//brute force all paths and pick the longest one
#include <stdio.h>
#include <string.h>

enum direction{
	NORTH,
	SOUTH,
	EAST,
	WEST
};

typedef struct{
	int visited;
	char path;
}path;

typedef struct{
	int row;
	int col;
}cord;

path path_arr[200][200];
cord start;
cord end;
int row_length = 0;
int col_length = 0;
int longest_path = 0;

int main(void)
{
	void find_longest_path(int, int, int, int);

	char *line = NULL;
	size_t lim = 0;

	//gets input
	while(getline(&line, &lim, stdin) > 1){
		for(int i = 0; line[i] != '\n'; ++i){
			if(line[i] == '#'){
				path_arr[row_length][i].path = line[i];
				path_arr[row_length][i].visited = 1;
			}
			else{
				path_arr[row_length][i].path = '.';
				path_arr[row_length][i].visited = 0;
			}

			col_length = i + 1;
		}
		++row_length;
	}


	//gets the start and end nodes
	for(int i = 0; i < row_length; ++i){
		for(int j = 0; j < col_length; ++j){
			if(i == 0){
				if(path_arr[i][j].path == '.'){
					start.row = i;
					start.col = j;
				}
			}
			if(i == row_length - 1){
				if(path_arr[i][j].path == '.'){
					end.row = i;
					end.col = j;
				}
			}
			printf("%c", path_arr[i][j].path);
		}
		printf("\n");
	}

	printf("start: %d %d %c\n", start.row, start.col, path_arr[start.row][start.col].path);
	printf("end: %d %d %c\n", end.row, end.col, path_arr[end.row][end.col].path);
	path_arr[start.row][start.col].visited = 1;

	find_longest_path(start.row + 1, start.col, SOUTH, 1);

	printf("%d\n", longest_path);

	return 0;
}

//marks paths as traveled then checks if all four directions are valid calls itself for each valid path
//if path is end compares to longest path if longer sets new longest path
//unmarks path as traveled when returning
void find_longest_path(int row, int col, int direction, int visited)
{
	int is_valid(int, int, int);
	int count_visited(void);

	path_arr[row][col].visited = 1;

	if(row == end.row && col == end.col){
		if(visited > longest_path)
			longest_path = visited;
		path_arr[row][col].visited = 0;
		return;
	}

	if(is_valid(row - 1, col, NORTH)){
		find_longest_path(row - 1, col, NORTH, visited + 1);
	}

	if(is_valid(row + 1, col, SOUTH)){
		find_longest_path(row + 1, col, SOUTH, visited + 1);
	}

	if(is_valid(row, col + 1, EAST)){
		find_longest_path(row, col + 1, EAST, visited + 1);
	}

	if(is_valid(row, col - 1, WEST)){
		find_longest_path(row, col - 1, WEST, visited + 1);
	}

	path_arr[row][col].visited = 0;
}

//checks if path from direction is valid
//1 for valid 0 for invalid
int is_valid(int row, int col, int direction)
{
	if(path_arr[row][col].visited == 1)
		return 0;
	switch(path_arr[row][col].path){
		case '>':
			if(direction == EAST)
				return 1;
			return 0;
		case '<':
			if(direction == WEST)
				return 1;
			return 0;
		case '^':
			if(direction == NORTH)
				return 1;
			return 0;
		case 'v':
			if(direction == SOUTH)
				return 1;
			return 0;
		case '.':
			return 1;
	}
	return -1;
}
