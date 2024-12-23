/*
 * this is just a graph theory problem
 * ill store the graph in an edge adjacency matrix
 * to keep track of vertices ill have a 26 by 26 array where each leter in the vertex name is set to a number
 * if there is no entry for that vertex is is assigned a nuber
 * edges will only be added to the adjacency matrix with [lower number vertex][higher number vertex] to avoid double counting
 * to find 3 cycles take all pairs of vertices that are connected to a vertex starting with 't' if there is a connection then count that cycle
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
