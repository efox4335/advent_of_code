/*
 * put report in arr of ints
 * check if the first 2 numbers of the report are increasing or decreasing
 * then check if they are doing so at safe levels
 * then check if the rest of the report is doing so according to the rules
 * if unsafe data is found check if the report would be safe if either of the unsafe data pair would be removed
 * check also if removing levle at index 1 or 0 fixes it as they set weather the data increases or decreases
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//returns index of unsafe level on fail report_len on success
//will skip skip_index in pocessing
int report_safe(int *report, int report_len, int skip_index)
{
	enum{INC, DEC};

	int level_1 = 0;
	int level_2 = 0;
	int report_index = 0;

	if(report_index == skip_index){
		++report_index;
	}

	level_1 = report[report_index];
	++report_index;
	if(report_index == skip_index){
		++report_index;
	}

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

	if(report_index == skip_index){
			++report_index;
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

			if(report_index == skip_index){
				++report_index;
			}
		}
	}else{
		while(report_index < report_len){
			level_1 = level_2;
			level_2 = report[report_index];
			++report_index;
			level_dif = level_1 - level_2;

			if(level_dif >= 0 || level_dif <= -4){
				return report_index - 1;
			}

			if(report_index == skip_index){
				++report_index;
			}
		}
	}

	return report_len;
}

int main(void)
{
	char *str = NULL;
	size_t lim = 0;
	int safe_report_count = 0;

	while(getline(&str, &lim, stdin) > 1){
		int report_arr[100];
		int level_count = 0;
		int unsafe_level = 0;

		char *level_ptr = NULL;
		char *save_ptr = NULL;
		char delim[] = " \n";

		level_ptr = strtok_r(str, delim, &save_ptr);

		do{
			report_arr[level_count++] = atoi(level_ptr);

			level_ptr = strtok_r(NULL, delim, &save_ptr);
		}while(level_ptr != NULL);

		if((unsafe_level = report_safe(report_arr, level_count, level_count)) != level_count){//check if report is safe for both removed levels
			if(report_safe(report_arr, level_count, unsafe_level) == level_count){
				++safe_report_count;
			}else if(report_safe(report_arr, level_count, unsafe_level - 1) == level_count){
				++safe_report_count;
			}else if(report_safe(report_arr, level_count, 0) == level_count){
				++safe_report_count;
			}else if(report_safe(report_arr, level_count, 1) == level_count){
				++safe_report_count;
			}
		}else{
			++safe_report_count;
		}
	}

	printf("%d\n", safe_report_count);

	free(str);
	return 0;
}
