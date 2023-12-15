/*
until blank line add up the number of calories
compare the number of calories to the largest 3 numbers if bigger store it and print the sum of the 3 biggest
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	char *line = NULL;
	size_t lim = 0;
	long int biggest_amount[3] = {0, 0, 0};
	long int temp_stote = 0;

	for(getline(&line, &lim, stdin); !feof(stdin); getline(&line, &lim, stdin)){
		if(line[0] == '\n'){
			if(temp_stote > biggest_amount[0]){
				biggest_amount[2] = biggest_amount[1];
				biggest_amount[1] = biggest_amount[0];
				biggest_amount[0] = temp_stote;
			}
			else if(temp_stote > biggest_amount[1]){
				biggest_amount[2] = biggest_amount[1];
				biggest_amount[1] = temp_stote;
			}
			else if(temp_stote > biggest_amount[2]){
				biggest_amount[2] = temp_stote;
			}
			printf("%ld %ld %ld %ld\n", temp_stote, biggest_amount[0], biggest_amount[1], biggest_amount[2]);
			temp_stote = 0;
		}
		temp_stote += atoi(line);
	}

	printf("%ld\n", biggest_amount[0] + biggest_amount[1] + biggest_amount[2]);
	return 0;
}
