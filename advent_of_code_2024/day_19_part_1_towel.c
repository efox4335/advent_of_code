/*
 * since no native regex engine in c i'll just make an nfa to represent
 * or all towel configs and any amount of them if the input is consumed than we have a match
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
