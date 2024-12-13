/*
 * only exists because i thought this method was to error prone so switched to another for the real solve
 *
 * for 2 non parallel lines there can be only one intersection point
 * set one line at the origin and one at the claw pos then do a gaussian elimination to see where they intersect
 * equations will be in form k = dy * x - dx * y
 * k can be calculated by filling in x and y for the point they intersect
 * the gaussian elimination will need to deal with non integar numbers but rounding to the nearist whole number should take care of errors
 * the number of times each button was pressed for k = 0 is intersect x / dx
 * and for k != 0 is (claw_end_point - intersect x) / dx
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//double prescision will represent the number of digits correctly
typedef struct{
	double dx1;
	double dy1;
	double dx2;
	double dy2;
	double k1;
	double k2;
}equation;

typedef struct{
	double x;
	double y;
}cord;

//returns intersection point of these 2 equations
cord gaussian(const equation cur_equ)
{
	const double row_dif = (cur_equ.dy2 / cur_equ.dy1);
	cord intersect_point;
	intersect_point.y = cur_equ.k2 / ((-1 * cur_equ.dx2) - ((-1 * cur_equ.dx1) * row_dif));
	intersect_point.x = (cur_equ.dx1 * intersect_point.y) / cur_equ.dy1;//(dx1intersect_point.y)/dy1 = x_intersect

	return intersect_point;
}

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	equation cur_equ;
	cord prize_pos;

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
			cur_equ.dx1 = atof(num_ptr);

			num_ptr = strtok(NULL, delim);
			cur_equ.dy1 = atof(num_ptr);

			part = EQU_2;
			break;
		case EQU_2:
			num_ptr = strtok(input_line, delim);
			cur_equ.dx2 = atof(num_ptr);

			num_ptr = strtok(NULL, delim);
			cur_equ.dy2 = atof(num_ptr);

			part = PRIZE;
			break;
		case PRIZE:
			num_ptr = strtok(input_line, delim);
			prize_pos.x = atof(num_ptr);

			num_ptr = strtok(NULL, delim);
			prize_pos.y = atof(num_ptr);

			part = EQU_1;
			break;
		}

		if(part == EQU_1){
			cur_equ.k1 = 0;
			cur_equ.k2 = (prize_pos.x * cur_equ.dy2) - (prize_pos.y * cur_equ.dx2);

			cord inter_point = gaussian(cur_equ);

			long a_press_count = round(inter_point.x) / round(cur_equ.dx1);
			long b_press_count = (round(prize_pos.x) - round(inter_point.x)) / round(cur_equ.dx2);


			//interscetion point makes it impossaible to get the prize
			if(inter_point.x > prize_pos.x || inter_point.x < 0 || inter_point.y > prize_pos.y || inter_point.y < 0){
				if((a_press_count * (long) cur_equ.dx1) + (b_press_count * (long) cur_equ.dx2) == (long) prize_pos.x &&
				(a_press_count * (long) cur_equ.dy1) + (b_press_count * (long) cur_equ.dy2) == (long) prize_pos.y){
				token_count += a_press_count * 3 + b_press_count;
				}
				continue;
			//intersection point is not a whole number meaning it cannot be reached with button presses
			}/*else if(inter_point.x != floor(inter_point.x) || inter_point.y != floor(inter_point.y)){
				continue;
			}*/



			if((a_press_count * (long) cur_equ.dx1) + (b_press_count * (long) cur_equ.dx2) == (long) prize_pos.x &&
			(a_press_count * (long) cur_equ.dy1) + (b_press_count * (long) cur_equ.dy2) == (long) prize_pos.y){
				token_count += a_press_count * 3 + b_press_count;
			}
		}
	}

	printf("%ld\n", token_count);

	free(input_line);
	return 0;
}
