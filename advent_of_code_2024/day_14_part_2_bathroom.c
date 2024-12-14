/*
 * look for robots in a row
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

enum{ROOM_COL_COUNT = 101L, ROOM_ROW_COUNT = 103L, SEC_COUNT = 100L};

typedef struct{
	long row;
	long col;
}cord;

typedef struct{
	cord st_point;
	cord vel;
}robot;

//returns the robots pos after one second
cord sim_sec(const cord cur_pos, const robot cur_rb)
{
	cord ret_pos;
	ret_pos.row = cur_rb.vel.row + cur_pos.row;

	if(ret_pos.row >= ROOM_ROW_COUNT){
		ret_pos.row %= ROOM_ROW_COUNT;
	}else if(ret_pos.row < 0){
		ret_pos.row += ROOM_ROW_COUNT;
	}

	ret_pos.col = cur_rb.vel.col + cur_pos.col;

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

	while(getline(&input_line, &lim, stdin) > 1){
		int_ptr = strtok(input_line, delim);
		rb_arr[rb_count].st_point.col = atol(int_ptr);

		int_ptr = strtok(NULL, delim);
		rb_arr[rb_count].st_point.row = atol(int_ptr);

		int_ptr = strtok(NULL, delim);
		rb_arr[rb_count].vel.col = atol(int_ptr);

		int_ptr = strtok(NULL, delim);
		rb_arr[rb_count].vel.row = atol(int_ptr);

		++rb_count;
	}

	free(input_line);
	return 0;
}
