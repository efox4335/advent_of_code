/*
 * for 2 non parallel lines there can be only one intersection point
 * set one line at the origin and one at the claw pos then do a gaussian elimination to see where they intersect
 * equations will be in form k = dx * x - dy * y
 * k can be calculated by filling in x and y for the point they intersect
 * no need to deal with non integar numbers as there is no division and you can't half press a button
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 0){
	}

	free(input_line);
	return 0;
}
