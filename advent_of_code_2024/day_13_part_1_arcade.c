/*
 * button press count is the intersection of
 * prizex = a_count * adx + b_count * bdx
 * prizey = a_count * ady + b_count * bdy
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//double prescision will represent the number of digits correctly
typedef struct{
	double adx;
	double ady;
	double bdx;
	double bdy;
	double prizex;
	double prizey;
}equation;

typedef struct{
	double a_count;
	double b_count;
}press_count;

//returns button press count
press_count gaussian(const equation cur_equ)
{
	const double row_dif = (cur_equ.ady / cur_equ.adx);
	press_count cur_count;
	cur_count.b_count = (cur_equ.prizey - (cur_equ.prizex * row_dif)) / (cur_equ.bdy - (row_dif * cur_equ.bdx));
	cur_count.a_count = (cur_equ.prizex - (cur_count.b_count * cur_equ.bdx)) / cur_equ.adx;

	return cur_count;
}

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	equation cur_equ;

	char delim[] = "ButonPrizeA: XY+=\n,";
	char *num_ptr = NULL;

	enum{ENTER, EQU_1, EQU_2, PRIZE};
	int part = ENTER;

	long token_count = 0;

	while(getline(&input_line, &lim, stdin) > 0){
		if(input_line[0] == '\n'){
			continue;
		}

		switch(part){
		case ENTER:
		case EQU_1:
			num_ptr = strtok(input_line, delim);
			cur_equ.adx = atof(num_ptr);

			num_ptr = strtok(NULL, delim);
			cur_equ.ady = atof(num_ptr);

			part = EQU_2;
			break;
		case EQU_2:
			num_ptr = strtok(input_line, delim);
			cur_equ.bdx = atof(num_ptr);

			num_ptr = strtok(NULL, delim);
			cur_equ.bdy = atof(num_ptr);

			part = PRIZE;
			break;
		case PRIZE:
			num_ptr = strtok(input_line, delim);
			cur_equ.prizex = atof(num_ptr);

			num_ptr = strtok(NULL, delim);
			cur_equ.prizey = atof(num_ptr);

			part = EQU_1;
			break;
		}

		if(part == EQU_1){
			press_count cur_button = gaussian(cur_equ);

			if(cur_button.a_count > 0 && cur_button.b_count > 0 && fabs(cur_button.a_count - round(cur_button.a_count)) < 0.01 && fabs(cur_button.b_count - round(cur_button.b_count)) < 0.01){
				token_count += 3 * cur_button.a_count + cur_button.b_count;
			}
		}
	}

	printf("%ld\n", token_count);

	free(input_line);
	return 0;
}
