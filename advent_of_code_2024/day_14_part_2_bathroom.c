/*
 * check if each robot has a right neighbor when this number is very high there is likely not a random distribution of robots
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum{ROOM_COL_COUNT = 101L, ROOM_ROW_COUNT = 103L};

typedef struct{
	long row;
	long col;
}cord;

typedef struct{
	cord cur_pos;
	cord vel;
}robot;

//returns the robots pos after one second
cord sim_sec(const robot cur_rb)
{
	cord ret_pos;
	ret_pos.row = cur_rb.vel.row + cur_rb.cur_pos.row;

	if(ret_pos.row >= ROOM_ROW_COUNT){
		ret_pos.row %= ROOM_ROW_COUNT;
	}else if(ret_pos.row < 0){
		ret_pos.row += ROOM_ROW_COUNT;
	}

	ret_pos.col = cur_rb.vel.col + cur_rb.cur_pos.col;

	if(ret_pos.col >= ROOM_COL_COUNT){
		ret_pos.col %= ROOM_COL_COUNT;
	}else if(ret_pos.col < 0){
		ret_pos.col += ROOM_COL_COUNT;
	}

	return ret_pos;
}

int main(void)
{
	char delim[] = "pv=,\n";
	char *int_ptr = NULL;

	char *input_line = NULL;
	size_t lim = 0;

	robot rb_arr[1000];
	int rb_count = 0;

	int room_arr[200][200];

	for(int i = 0; i < 200; ++i){
		for(int j = 0; j < 200; ++j){
			room_arr[i][j] = 0;
		}
	}

	while(getline(&input_line, &lim, stdin) > 1){
		int_ptr = strtok(input_line, delim);
		rb_arr[rb_count].cur_pos.col = atol(int_ptr);

		int_ptr = strtok(NULL, delim);
		rb_arr[rb_count].cur_pos.row = atol(int_ptr);

		int_ptr = strtok(NULL, delim);
		rb_arr[rb_count].vel.col = atol(int_ptr);

		int_ptr = strtok(NULL, delim);
		rb_arr[rb_count].vel.row = atol(int_ptr);

		room_arr[rb_arr[rb_count].cur_pos.row][rb_arr[rb_count].cur_pos.col] += 1;

		++rb_count;
	}

	int sec_count = 0;

	while(1){
		int right_nab_count = 0;

		for(int j = 0; j < rb_count; ++j){
			const cord next_pos = sim_sec(rb_arr[j]);

			room_arr[rb_arr[j].cur_pos.row][rb_arr[j].cur_pos.col] -= 1;
			room_arr[next_pos.row][next_pos.col] += 1;

			rb_arr[j].cur_pos = next_pos;

			if(room_arr[next_pos.row][next_pos.col + 1] > 0){
				++right_nab_count;
			}
		}

		++sec_count;

		if(right_nab_count > 90){
			break;
		}
	}

	for(int i = 0; i < ROOM_ROW_COUNT; ++i){
		for(int j = 0; j < ROOM_COL_COUNT; ++j){
			printf("%d", room_arr[i][j]);
		}

		printf("\n");
	}

	printf("%d\n", sec_count);

	free(input_line);
	return 0;
}
