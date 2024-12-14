/*
 * flood fill all robot positions till a high number of robots are in an area
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

//returns the number of robots connected to st_rb
int flood_count(const cord st_rb, const int rb_pos_arr[200][200])
{
	int seen_arr[200][200];

	for(int i = 0; i < 200; ++i){
		for(int j = 0; j < 200; ++j){
			seen_arr[i][j] = 0;
		}
	}

	cord visit_stack[1000];
	int st_count = 0;
	int rb_count = 0;

	seen_arr[st_rb.row][st_rb.col] = 1;
	visit_stack[st_count] = st_rb;
	++st_count;

	while(st_count > 0){
		cord cur_rb = visit_stack[st_count - 1];

		//upper
		if(cur_rb.row > 0 && rb_pos_arr[cur_rb.row - 1][cur_rb.col] > 0 && seen_arr[cur_rb.row - 1][cur_rb.col] == 0){
			visit_stack[st_count].row = cur_rb.row - 1;
			visit_stack[st_count].col = cur_rb.col;
			seen_arr[cur_rb.row - 1][cur_rb.col] = 1;
			++st_count;
		}
		//down
		if(cur_rb.row < ROOM_ROW_COUNT && rb_pos_arr[cur_rb.row + 1][cur_rb.col] > 0 && seen_arr[cur_rb.row + 1][cur_rb.col] == 0){
			visit_stack[st_count].row = cur_rb.row + 1;
			visit_stack[st_count].col = cur_rb.col;
			seen_arr[cur_rb.row + 1][cur_rb.col] = 1;
			++st_count;
		}
		//left
		if(cur_rb.col > 0 && rb_pos_arr[cur_rb.row][cur_rb.col - 1] > 0 && seen_arr[cur_rb.row][cur_rb.col - 1] == 0){
			visit_stack[st_count].row = cur_rb.row;
			visit_stack[st_count].col = cur_rb.col - 1;
			seen_arr[cur_rb.row][cur_rb.col - 1] = 1;
			++st_count;
		}
		//right
		if(cur_rb.col < ROOM_COL_COUNT && rb_pos_arr[cur_rb.row][cur_rb.col + 1] > 0 && seen_arr[cur_rb.row][cur_rb.col + 1] == 0){
			visit_stack[st_count].row = cur_rb.row;
			visit_stack[st_count].col = cur_rb.col + 1;
			seen_arr[cur_rb.row][cur_rb.col + 1] = 1;
			++st_count;
		}

		--st_count;
		++rb_count;
	}

	return rb_count;
}

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
	int max_seen = 0;

	while(1){
		for(int j = 0; j < rb_count; ++j){
			const cord next_pos = sim_sec(rb_arr[j]);

			room_arr[rb_arr[j].cur_pos.row][rb_arr[j].cur_pos.col] -= 1;
			room_arr[next_pos.row][next_pos.col] += 1;

			rb_arr[j].cur_pos = next_pos;
		}

		max_seen = 0;

		for(int j = 0; j < rb_count; ++j){
			int cur_seen = flood_count(rb_arr[j].cur_pos, room_arr);
			max_seen = (cur_seen > max_seen)? cur_seen : max_seen;
		}

		++sec_count;

		if(max_seen >= 20){
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
