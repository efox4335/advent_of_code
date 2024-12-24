/*
 * take in wires so that each is assigned a number
 * with that number the wires state can be looked up as well as the 2 wires and the op that is needed to set it
 * for each z wire recurse through wires need to set it till already set wires are found and set the z wires
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
