/*
 * just simulate the guard till it goes off screen while counting the positions
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum{INPUT_BUFF_SIZE = 200};

typedef struct{
	int row;
	int col;
}cords;

//returns the column the guard is found in
int find_guard(char *line)
{
	for(int i = 0; line[i] != '\n'; ++i){
		if(line[i] == '^'){
			return i;
		}
	}

	return -1;
}

int main(void)
{
	enum{FOUND, NOT_FOUND};

	char *input_line = NULL;
	size_t lim = 0;

	char input[INPUT_BUFF_SIZE][INPUT_BUFF_SIZE];
	int line_count = 0;
	cords guard_pos;
	int is_guard_found = NOT_FOUND;

	while(getline(&input_line, &lim, stdin) > 1){
		strcpy(input[line_count], input_line);
		if(is_guard_found == NOT_FOUND){
			guard_pos.col = find_guard(input_line);
			if(guard_pos.col != -1){
				is_guard_found = FOUND;
				guard_pos.row = line_count;
			}
		}
		++line_count;
	}

	free(input_line);
	return 0;
}
