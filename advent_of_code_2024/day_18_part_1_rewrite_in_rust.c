/*
 * dijkstra's algorithm
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum{READ_AMOUNT = 1024};
enum{ROW_AMOUNT = 71, COL_AMOUNT = 71};

typedef struct{
	long row;
	long col;
}cord;

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	char mem_space[ROW_AMOUNT][COL_AMOUNT];

	for(int i = 0; i < ROW_AMOUNT; ++i){
		for(int j = 0; j < COL_AMOUNT; ++j){
			mem_space[i][j] = '.';
		}
	}

	char delim[] = ",\n";
	int count = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		if(count >= READ_AMOUNT){
			break;
		}

		cord temp_cord;

		char *temp_ptr = strtok(input_line, delim);
		temp_cord.col = atol(temp_ptr);

		if(temp_cord.col >= COL_AMOUNT){
			++count;
			continue;
		}

		temp_ptr = strtok(NULL, delim);
		temp_cord.row = atol(temp_ptr);

		if(temp_cord.row >= ROW_AMOUNT){
			++count;
			continue;
		}

		mem_space[temp_cord.row][temp_cord.col] = '#';

		++count;
	}

	free(input_line);
	return 0;
}
