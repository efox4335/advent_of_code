/*
 * a simple dijkstra's algorithm
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum{BUF_SIZE = 200};


int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	char input[BUF_SIZE][BUF_SIZE];
	int line_count = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		strcpy(input[line_count], input_line);
		++line_count;
	}

	free(input_line);
	return 0;
}
