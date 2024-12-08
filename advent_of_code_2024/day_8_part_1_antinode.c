/*
 * antinode position calculation is simple just get the slope between each pair of the same type antennas
 * i think if 2 antinodes overlap than it counts as 1 antinode
 * store antinodes in 2d array with index being there position to not double count antinodes
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		printf("%s", input_line);
	}

	free(input_line);
	return 0;
}
