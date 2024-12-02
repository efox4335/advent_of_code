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

//returns index of unsafe level on fail report_len on success
int report_safe(int *report, int report_len)
{
	enum{INC, DEC};

	int level_1 = 0;
	int level_2 = 0;
	int report_index = 0;

	level_1 = report[report_index];
	++report_index;
	level_2 = report[report_index];
	++report_index;

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
			return report_index - 1;
	}

	if(report_type == DEC){
		while(report_index < report_len){
			level_1 = level_2;
			level_2 = report[report_index];
			++report_index;
			level_dif = level_1 - level_2;

			if(level_dif <= 0 || level_dif >= 4){
				return report_index - 1;
			}
		}
	}else{
		while(report_index < report_len){
			level_1 = level_2;
			level_2 = report[report_index];
			++report_index;
			level_dif = level_1 - level_2;

			if(level_dif >= 0 || level_dif <= -4){
				return report_index - 1;;
			}
		}
	}

	return report_len;
}

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
