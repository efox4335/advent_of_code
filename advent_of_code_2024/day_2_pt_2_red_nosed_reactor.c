/*
 * check if the first 2 numbers of the report are increasing or decreasing
 * then check if they are doing so at safe levels
 * then check if the rest of the report is doing so according to the rules
 * add flag that gets marked if an unsafe level change has happened and if it is the first time rerun that part with new input vals
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

	int level_dif = level_1 - level_2;
	int report_type = 0;

	switch(level_dif){
		case -3:
		case -2:
		case -1:
			report_type = INC;
			break;
		case 3:
		case 2:
		case 1:
			report_type = DEC;
			break;
		default://unsafe
			return 0;
	}

	if(report_type == DEC){
		while((level_ptr = strtok_r(NULL, delim, &save_ptr)) != NULL){
			level_1 = level_2;
			level_2 = atoi(level_ptr);
			level_dif = level_1 - level_2;

			if(level_dif <= 0 || level_dif >= 4){
				return 0;
			}
		}
	}else{
		while((level_ptr = strtok_r(NULL, delim, &save_ptr)) != NULL){
			level_1 = level_2;
			level_2 = atoi(level_ptr);
			level_dif = level_1 - level_2;

			if(level_dif >= 0 || level_dif <= -4){
				return 0;
			}
		}
	}

	return 1;
}

int main(void)
{
	char *str = NULL;
	size_t lim = 0;
	int safe_report_count = 0;

	while(getline(&str, &lim, stdin) > 1){
		safe_report_count += report_safe(str);
	}

	printf("%d\n", safe_report_count);

	free(str);
	return 0;
}
