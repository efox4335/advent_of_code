/*
 * get the input line by line and transform it so each way a match and happen is all in one line
 * eg. diagonal matchs transformed this way make it look like a rhombus
 * 	ab
 * 	cd
 * becomes
 * 	a
 * 	bc
 * 	 d
 * and
 * 	 b
 * 	ad
 * 	c
 * and
 * 	bd
 * 	ac
 * have a fowards and backwards match so no mirror is required
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	enum{BUFFER_LEN = 1000};

	int line_count = 0;

	char *input_line = NULL;
	char input[BUFFER_LEN][BUFFER_LEN];
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		strcpy(input[line_count], input_line);
		++line_count;
	}

	free(input_line);
	return 0;
}
