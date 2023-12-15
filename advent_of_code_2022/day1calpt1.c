/*
until blank line add up the number of calories
compare the number of calories to the largest number if bigger store it and print
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	char *line = NULL;
	size_t lim = 0;
	long int biggest_amount = 0;
	long int temp_stote = 0;

	for(getline(&line, &lim, stdin); !feof(stdin); getline(&line, &lim, stdin)){
		if(line[0] == '\n'){
			if(temp_stote > biggest_amount)
				biggest_amount = temp_stote;
			temp_stote = 0;
		}
		temp_stote += atoi(line);
	}

	printf("%ld\n", biggest_amount);
	return 0;
}
