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

void calc_antinode_pos(cords antenna_1, cords antenna_2, cords antinodes[2])
{
	int row_dif = antenna_1.row - antenna_2.row;
	int col_dif = antenna_1.col - antenna_2.col;

	antinodes[0].row = antenna_1.row + row_dif;
	antinodes[0].col = antenna_1.col + col_dif;

	antinodes[1].row = antenna_2.row - row_dif;
	antinodes[1].col = antenna_2.col - col_dif;
}

//draws antinode if it is within the range
//line_count only is needed because input is square
void draw_antinode(char dest_arr[200][200], cords antinode, int line_count)
{
	if(antinode.row >= 0 && antinode.row < line_count && antinode.col >= 0 && antinode.col < line_count){
		dest_arr[antinode.row][antinode.col] = '#';
	}
}

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

	char antinode_pos[200][200];
	for(int i = 0; i < 200; ++i){
		for(int j = 0; j < 200; ++j){
			antinode_pos[i][j] = '.';
		}
	}

	for(int i = 0; i < 256; ++i){
		if(antenna_type_count[i] == 0){
			continue;
		}

		for(int k = 0; k < antenna_type_count[i]; ++k){
			for(int j = k + 1; j < antenna_type_count[i]; ++j){
				cords temp_antinodes[2];

				calc_antinode_pos(antenna_pos_arr[i][k], antenna_pos_arr[i][j], temp_antinodes);

				draw_antinode(antinode_pos, temp_antinodes[0], line_count);
				draw_antinode(antinode_pos, temp_antinodes[1], line_count);
			}
		}
	}

	int antinode_count = 0;
	for(int i = 0; i < 200; ++i){
		for(int j = 0; j < 200; ++j){
			if(antinode_pos[i][j] == '#'){
				++antinode_count;
			}
		}
	}

	printf("%d\n", antinode_count);

	free(input_line);
	return 0;
}
