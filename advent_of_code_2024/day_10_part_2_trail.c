/*
 * a recursive memoized function will walk the path and return the number of trail ends connected to a path
*/
#include <stdio.h>
#include <stdlib.h>

enum{BUFF_SIZE = 100};
int memo[BUFF_SIZE][BUFF_SIZE];

int get_trail_end_num(int paths[BUFF_SIZE][BUFF_SIZE], int row, int col, int line_count)
{
	if(memo[row][col] != -1){
		return memo[row][col];
	}

	if(paths[row][col] == 9){
		return 1;
	}

	int trail_end_sum = 0;

	if(row > 0 && paths[row - 1][col] - 1 == paths[row][col]){
		trail_end_sum += get_trail_end_num(paths, row - 1, col, line_count);
	}

	if(row < line_count && paths[row + 1][col] - 1 == paths[row][col]){
		trail_end_sum += get_trail_end_num(paths, row + 1, col, line_count);
	}

	if(col > 0 && paths[row][col - 1] - 1 == paths[row][col]){
		trail_end_sum += get_trail_end_num(paths, row, col - 1, line_count);
	}

	if(col < line_count && paths[row][col + 1] - 1 == paths[row][col]){
		trail_end_sum += get_trail_end_num(paths, row, col + 1, line_count);
	}

	memo[row][col] = trail_end_sum;

	return trail_end_sum;
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
			memo[i][j] = -1;
		}
	}

	int score_sum = 0;

	for(int i = 0; i < line_count; ++i){
		for(int j = 0; j < line_count; ++j){
			if(paths[i][j] == 0){
				score_sum += get_trail_end_num(paths, i, j, line_count);
			}
		}
	}

	printf("%d\n", score_sum);

	free(input_line);
	return 0;
}
