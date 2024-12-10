/*
 * a recursive memoized function will walk the path and return the number of trail ends connected to a path
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	enum{BUFF_SIZE = 100};

	int paths[BUFF_SIZE][BUFF_SIZE];
	int line_count = 0;

	char *input_line = NULL;
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		for(int i = 0; input_line[i] != '\n'; ++i){
			paths[line_count][i] = input_line[i] - '0';
		}
		++line_count;
	}

	free(input_line);
	return 0;
}
