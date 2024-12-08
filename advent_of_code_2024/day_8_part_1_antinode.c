/*
 * antinode position calculation is simple just get the slope between each pair of the same type antennas
 * i think if 2 antinodes overlap than it counts as 1 antinode
 * store antinodes in 2d array with index being there position to not double count antinodes
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int row;
	int col;
}cords;

int main(void)
{
	//index by ascii num
	int antenna_type_count[256];

	for(int i = 0; i < 256; ++i){
		antenna_type_count[i] = 0;
	}

	cords antenna_pos_arr[256][100];
	int line_count = 0;

	char *input_line = NULL;
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		for(int i = 0; input_line[i] != '\0'; ++i){
			if(input_line[i] == '.' || input_line[i] == '\n'){
				continue;
			}
			antenna_pos_arr[(int) input_line[i]][antenna_type_count[(int) input_line[i]]].row = line_count;
			antenna_pos_arr[(int) input_line[i]][antenna_type_count[(int) input_line[i]]].col = i;

			antenna_type_count[(int) input_line[i]] += 1;
		}
		++line_count;
	}

	free(input_line);
	return 0;
}
