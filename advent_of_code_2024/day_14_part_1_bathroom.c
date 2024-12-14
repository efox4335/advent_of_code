/*
 * just a simple cycle detection
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

//euclids algorithm
size_t gcd(size_t a, size_t b)
{
	while(1){
		a %= b;

		if(a == 0){
			a = b;
			break;
		}

		b %= a;

		if(b == 0){
			break;
		}
	}

	return a;
}

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

/*
 * returns the robot position after SEC_COUNT seconds
 * to find when a cycle happens the row and col components can be broken up and solve separately
 * for any number > 1 and a < b (a*c) mod b certain c will cause (a*c) mod b == 0
 * and the lowest c is lcm(a, b)/a
 * so when a starting point is factored in ((a * c) + k) mod b == k if k < b
 * the c val can be found for both row and col c1, c2
 * the cycle length when factoring both in is lcm(c1, c2) == c3
 * so the final pos is sec_count % c3 == c4 and simulating c4 seconds
*/
cord get_robot_pos(const robot cur_rb)
{
	const long abs_row_vel = labs(cur_rb.vel.row);
	const long row_cyc_len = (((abs_row_vel * ROOM_ROW_COUNT) / gcd(abs_row_vel, ROOM_ROW_COUNT)) / abs_row_vel);

	const long abs_col_vel = labs(cur_rb.vel.col);
	const long col_cyc_len = (((abs_col_vel * ROOM_COL_COUNT) / gcd(abs_col_vel, ROOM_COL_COUNT)) / abs_col_vel);

	const long total_cyc_len = ((col_cyc_len * row_cyc_len) / gcd(col_cyc_len, row_cyc_len));

	const long sim_sec_count = SEC_COUNT % total_cyc_len;
	if(sim_sec_count == 0){
		return cur_rb.st_point;
	}

	cord ret_pos = cur_rb.st_point;

	for(long i = 0; i < sim_sec_count; ++i){
		ret_pos = sim_sec(ret_pos, cur_rb);
	}

	return ret_pos;
}

int main(void)
{
	char delim[] = "pv=,\n";
	char *int_ptr = NULL;

	char *input_line = NULL;
	size_t lim = 0;

	robot cur_rb;
	long ul_count = 0;
	long dl_count = 0;
	long dr_count = 0;
	long ur_count = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		int_ptr = strtok(input_line, delim);
		cur_rb.st_point.col = atol(int_ptr);

		int_ptr = strtok(NULL, delim);
		cur_rb.st_point.row = atol(int_ptr);

		int_ptr = strtok(NULL, delim);
		cur_rb.vel.col = atol(int_ptr);

		int_ptr = strtok(NULL, delim);
		cur_rb.vel.row = atol(int_ptr);

		cord final_pos = get_robot_pos(cur_rb);

		//upper
		if(final_pos.row < ROOM_ROW_COUNT >> 1){
			//left
			if(final_pos.col < ROOM_COL_COUNT >> 1){
				++ul_count;
			//right
			}else if(final_pos.col > ROOM_COL_COUNT >> 1){
				++ur_count;
			}
		//down
		}else if(final_pos.row > ROOM_ROW_COUNT >> 1){
			//left
			if(final_pos.col < ROOM_COL_COUNT >> 1){
				++dl_count;
			//right
			}else if(final_pos.col > ROOM_COL_COUNT >> 1){
				++dr_count;
			}
		}
	}

	printf("%ld\n", ul_count * ur_count * dl_count * dr_count);

	free(input_line);
	return 0;
}
