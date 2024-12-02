/*
 * check if the first 2 numbers of the report are increasing or decreasing
 * then check if they are doing so at safe levels
 * then check if the rest of the report is doing so according to the rules
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *str = NULL;
	size_t lim = 0;

	while(getline(&str, &lim, stdin) > 1){
	}

	free(str);
	return 0;
}
