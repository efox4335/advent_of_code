/*
 * just a simple cycle detection
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	long row;
	long col;
}cord;

typedef struct{
	cord st_point;
	cord vel;
}robot;

int main(void)
{
	char delim[] = "pv=,\n";
	char *int_ptr = NULL;

	char *input_line = NULL;
	size_t lim = 0;

	robot cur_rb;

	while(getline(&input_line, &lim, stdin) > 1){
		int_ptr = strtok(input_line, delim);
		cur_rb.st_point.col = atol(int_ptr);

		int_ptr = strtok(NULL, delim);
		cur_rb.st_point.row = atol(int_ptr);

		int_ptr = strtok(NULL, delim);
		cur_rb.vel.col = atol(int_ptr);

		int_ptr = strtok(NULL, delim);
		cur_rb.vel.row = atol(int_ptr);
	}

	free(input_line);
	return 0;
}
