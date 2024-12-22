/*
 * sn = ((((s << 6) ^ s) % (1 << 24))
 * sf = (((sn >> 5) ^ sn) % (1 << 24))
 * sk = (((sf << 11) ^ sf) % (1 << 24))
 * s + 1 = sk
 * just a simple calculation to calculate the 2000th secret number
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
