/*
 * put report in arr of ints
 * check if the first 2 numbers of the report are increasing or decreasing
 * then check if they are doing so at safe levels
 * then check if the rest of the report is doing so according to the rules
 * if unsafe data is found check if the report would be safe if either of the unsafe data pair would be removed
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char *str = NULL;
	size_t lim = 0;

	while(getline(&str, &lim, stdin) > 1){
		int report_arr[100];
		int level_count = 0;

		char *level_ptr = NULL;
		char *save_ptr = NULL;
		char delim[] = " \n";

		level_ptr = strtok_r(str, delim, &save_ptr);

		do{
			report_arr[level_count++] = atoi(level_ptr);

			level_ptr = strtok_r(NULL, delim, &save_ptr);
		}while(level_ptr != NULL);
	}

	free(str);
	return 0;
}
