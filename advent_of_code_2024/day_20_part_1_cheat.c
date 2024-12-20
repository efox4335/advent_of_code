/*
 * mark each path with it's distance from the start
 * the time a shortcut saves is equal to the differences of dist from start - cut len
 * a cut is only valid if it is adjcent to the normal path
 * i'm going on the assumpiton that .##. would have a valid cut
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum{BUF_SIZE = 200};

int main(void)
{
	char input[BUF_SIZE][BUF_SIZE];
	int line_count = 0;

	char *input_line = NULL;
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		strcpy(input[line_count], input_line);
		++line_count;
	}

	free(input_line);
	return 0;
}
