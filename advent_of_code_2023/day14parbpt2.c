/*
goes througth cycles till a same cycle is found
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MOV_NUM 1000000000

char mirror_array[200][200];
char seen_bef[1000][200][200];
long int k_val[1000];

int max_rows = 0;
int cashe_ind = 0;
int max_len = 0;

enum direction{
	NORTH,
	WEST,
	SOUTH,
	EAST
};

int main(void)
{
	char *line = NULL;
	size_t lim = 0;
	long int running_total = 0;

	int find_dist_to_next(int, int, int);
	void swap_tile(int, int, int, int);
	void advance_cyc(void);



//	gets input
	while(getline(&line, &lim, stdin) > 1)
		strcpy(mirror_array[max_rows++], line);

	free(line);

	for(int i = 0; mirror_array[0][i] != '\n'; ++i)
		max_len = i;

	//return 0;
	long int new_k = 0;
	for(long int k = 0; k < MOV_NUM; ++k){//gets iterations
		advance_cyc();
		if(k > 0 && new_k == 0){
			new_k = 0;
			int is_same = 2;
			for(int i = 0; i < max_rows; ++i){//puts iteration on cashe
				strcpy(seen_bef[cashe_ind][i], mirror_array[i]);
			}
			k_val[cashe_ind] = k;

			if(cashe_ind == 999)//loops cashe for long times spent out of loop
				cashe_ind = 0;
			else
				++cashe_ind;


			for(int i = 0; i < cashe_ind - 1; ++i){//checks cashe for match
				is_same = 0;
				for(int j = 0; j < max_rows; ++j){
					if(strcmp(seen_bef[i][j], mirror_array[j]) != 0){
						is_same = 1;
						break;
					}
				}
				if(is_same == 0){
					long int temp = MOV_NUM - k_val[i];
					new_k = MOV_NUM - (temp % (k - k_val[i]));
					break;
				}
			}
			if(is_same == 0){//if found advance k to near the end of loop
				k = new_k;
			}
		}
	}

	running_total = 0;

	for(int i = 0; i < max_rows; ++i){
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
//returns row + 1 of the tile found for north
//returns row - 1 of the tile found for south
//returns col - 1 of the tile found for west
//returns col + 1 of the tile found for east
int find_dist_to_next(int row, int col, int direction)
{
	switch(direction){
	case NORTH:
		for(int i = row - 1; i >= 0; --i)
			if(mirror_array[i][col] != '.')
				return i + 1;

		return 0;
	case SOUTH:
		for(int i = row + 1; i < max_rows; ++i)
			if(mirror_array[i][col] != '.')
				return i - 1;

		return max_rows - 1;
	case WEST:
		for(int i = col - 1; i >= 0; --i)
			if(mirror_array[row][i] != '.')
				return i + 1;
		return 0;
	case EAST:
		for(int i = col + 1; mirror_array[row][i] != '\n'; ++i){
			if(mirror_array[row][i] != '.')
				return i - 1;
		}
		return max_len;
	}
	return -1;
}

//swaps tiles at row_1 col_1 with tiles at row_2 col_2
void swap_tile(int row_1, int col_1, int row_2, int col_2)
{
	char buff = mirror_array[row_2][col_2];
	mirror_array[row_2][col_2] = mirror_array[row_1][col_1];
	mirror_array[row_1][col_1] = buff;
}


//moves the input 1 cycle
void advance_cyc(void)
{

	int direction = EAST;
	for(long int k = 0; k < 4; ++k){
		switch(direction){
		case NORTH:
		direction = WEST;
		for(int i = 0; i < max_rows; ++i){
			for(int j = 0; j <= max_len; ++j){
				if(mirror_array[i][j] == 'O'){
					swap_tile(i, j, i, find_dist_to_next(i,j, WEST));
				}
			}
		}
		break;
		case WEST:
		direction = SOUTH;
		for(int i = max_rows - 1; i >= 0; --i){
			for(int j = 0; mirror_array[i][j] != '\n'; ++j){
				if(mirror_array[i][j] == 'O'){
					swap_tile(i, j, find_dist_to_next(i,j, SOUTH), j);
				}
			}
		}
		break;
		case SOUTH:
		direction = EAST;
		for(int i = 0; i < max_rows; ++i){
			for(int j = max_len; j >= 0; --j){
				if(mirror_array[i][j] == 'O'){
					swap_tile(i, j, i, find_dist_to_next(i,j, EAST));
				}
			}
		}
		break;
		case EAST:
		direction = NORTH;
		for(int i = 0; i < max_rows; ++i){
			for(int j = 0; mirror_array[i][j] != '\n'; ++j){
				if(mirror_array[i][j] == 'O'){
					swap_tile(i, j, find_dist_to_next(i,j, NORTH), j);
				}
			}
		}
		break;
		}
	}

}
