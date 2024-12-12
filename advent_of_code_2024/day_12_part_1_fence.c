/*
 * for a polygon made of squares where you have points in the center of each edge
 * the area is equal to the shoelace theorm + 1/2 perimeter - 1
 * which is picks theorm
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 1){
	}

	free(input_line);
	return 0;
}
