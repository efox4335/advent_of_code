/*
get input
go line by line moving each circle rock as far north as it can go
count up all circle rocks times the row bonuses
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char mirror_array[1000][1000];

int max_rows = 0;

int main(void)
{
	char *line = NULL;
	size_t lim = 0;
	long int running_total = 0;

	int find_dist_to_next(int, int);
	void swap_tile(int, int, int, int);



//	gets input
	while(getline(&line, &lim, stdin) > 1)
		strcpy(mirror_array[max_rows++], line);

	free(line);

	for(int i = 1; i < max_rows; ++i){
		printf("%s", mirror_array[i]);
		for(int j = 0; mirror_array[i][j] != '\n'; ++j){
			if(mirror_array[i][j] == 'O'){
				swap_tile(i, j, find_dist_to_next(i,j), j);
			}
		}
	}

	printf("\n");

	for(int i = 0; i < max_rows; ++i){
		printf("%s", mirror_array[i]);
		for(int j = 0; mirror_array[i][j] != '\n'; ++j){
			if(mirror_array[i][j] == 'O'){
				running_total += max_rows - i;
			}
		}
	}

	printf("%ld\n", running_total);
	return 0;
}

//finds the distance from the input tile to the next above non '.' tile
//returns row + 1 of the tile found
int find_dist_to_next(int row, int col)
{
	for(int i = row - 1; i >= 0; --i)
		if(mirror_array[i][col] != '.')
			return i + 1;

	return 0;
}

//swaps tiles at row_1 col_1 with tiles at row_2 col_2
void swap_tile(int row_1, int col_1, int row_2, int col_2)
{
	char buff = mirror_array[row_2][col_2];
	mirror_array[row_2][col_2] = mirror_array[row_1][col_1];
	mirror_array[row_1][col_1] = buff;
}
