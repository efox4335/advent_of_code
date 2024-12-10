/*
 * a recursive memoized function will walk the path and return the number of trail ends connected to a path
 * the memo will contain a list of all the trail ends connected to the path to avoid double counting
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int row;
	int col;
}cords;

//returns 1 if cords are equal 0 otherwise
int cords_equal(cords a, cords b)
{
	if(a.row != b.row){
		return 0;
	}

	if(a.col != b.col){
		return 0;
	}

	return 1;
}

//merges arr_2 into arr_1 without duplicates
void merge_cords_arr(cords *arr_1, cords *arr_2)
{
	for(int i = 0; arr_2[i].row != -1; ++i){
		int list_end = 0;
		for(list_end = 0; arr_1[list_end].row != -1; ++list_end){
			if(cords_equal(arr_1[list_end], arr_2[i])){
				break;
			}
		}

		if(arr_1[list_end].row == -1){
			arr_1[list_end].row = arr_2[i].row;
			arr_1[list_end].col = arr_2[i].col;
		}
	}
}

enum{BUFF_SIZE = 100};
cords memo[BUFF_SIZE][BUFF_SIZE][BUFF_SIZE];

cords *get_trail_end_num(int paths[BUFF_SIZE][BUFF_SIZE], int row, int col, int line_count)
{
	if(memo[row][col][0].row != -1){
		return memo[row][col];
	}

	if(paths[row][col] == 9){
		memo[row][col][0].row = row;
		memo[row][col][0].col = col;
		return memo[row][col];
	}

	if(row > 0 && paths[row - 1][col] - 1 == paths[row][col]){
		merge_cords_arr(memo[row][col], get_trail_end_num(paths, row - 1, col, line_count));
	}

	if(row < line_count && paths[row + 1][col] - 1 == paths[row][col]){
		merge_cords_arr(memo[row][col], get_trail_end_num(paths, row + 1, col, line_count));
	}

	if(col > 0 && paths[row][col - 1] - 1 == paths[row][col]){
		merge_cords_arr(memo[row][col], get_trail_end_num(paths, row, col - 1, line_count));
	}

	if(col < line_count && paths[row][col + 1] - 1 == paths[row][col]){
		merge_cords_arr(memo[row][col], get_trail_end_num(paths, row, col + 1, line_count));
	}

	return memo[row][col];
}

int main(void)
{
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

	for(int i = 0; i < BUFF_SIZE; ++i){
		for(int j = 0; j < BUFF_SIZE; ++j){
			for(int k = 0; k < BUFF_SIZE; ++k){
				memo[k][i][j].row = -1;
				memo[k][i][j].col = -1;
			}
		}
	}

	int score_sum = 0;

	for(int i = 0; i < line_count; ++i){
		for(int j = 0; j < line_count; ++j){
			if(paths[i][j] == 0){
				cords *arr = get_trail_end_num(paths, i, j, line_count);
				for(int i = 0; arr[i].row != -1; ++i){
					++score_sum;
				}
			}
		}
	}

	printf("%d\n", score_sum);

	free(input_line);
	return 0;
}
