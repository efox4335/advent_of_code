/*
 * check if the first 2 numbers of the report are increasing or decreasing
 * then check if they are doing so at safe levels
 * then check if the rest of the report is doing so according to the rules
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//returns 1 if report is safe returns 0 if not
int report_safe(char *report)
{
	enum{INC, DEC};

	int level_1 = 0;
	int level_2 = 0;

	char *level_ptr = NULL;
	char *save_ptr = NULL;
	char delim[] = " \n";

	level_ptr = strtok_r(report, delim, &save_ptr);
	level_1 = atoi(level_ptr);
	level_ptr = strtok_r(NULL, delim, &save_ptr);
	level_2 = atoi(level_ptr);

}

int main(void)
{
	char *str = NULL;
	size_t lim = 0;

	while(getline(&str, &lim, stdin) > 1){
		report_safe(str);
	}

	free(str);
	return 0;
}
