/*
 * for a polygon made of squares where you have points in the center of each edge
 * the area is equal to the shoelace theorm + 1/2 perimeter - 1
 * which is picks theorm
 * when processing each regon each plant within it will be marked to avoid processing it twice
*/
#include <stdio.h>
#include <stdlib.h>

enum{VISITED, NOT_VISITED};
enum{BUF_SIZE = 200};

typedef struct{
	char plant;
	int visited;
}plant;

int main(void)
{
	int line_count = 0;
	plant input[BUF_SIZE][BUF_SIZE];

	char *input_line = NULL;
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		for(int i = 0; input_line[i] != '\n'; ++i){
			input[line_count][i].plant = input_line[i];
			input[line_count][i].visited = NOT_VISITED;
		}

		++line_count;
	}

	free(input_line);
	return 0;
}
