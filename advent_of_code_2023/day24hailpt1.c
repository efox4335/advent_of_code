//get input each line is an equation with a starting point and a velocity for each axis
//convert to equation
//y_cord = (x_cord/x_vol) * y_vol + (y_in_cord - ((x_in_cord/x_vol) * y_vol))
//y_cord = a*x_cord + b
//intersection of these points is x = (b(2) - b(1))/(a(1) - a(2)) y = a(1) * ((b(2) - b(1))/(a(1) - a(2))) + b(1)
//if x or y is within the min max range count it
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
	double x_vol;
	double y_vol;

	double slope;

	double x_start;
	double y_start;

	double offset;
}equation;

equation equation_array[1000];
int equation_num = 0;

double MAX = 400000000000000.0;
double MIN = 200000000000000.0;

int main(void)
{
	int check_past(double, double, equation *);

	char *line = NULL;
	size_t lim = 0;
	char *token = NULL;
	long inter_count = 0;

	while(getline(&line, &lim, stdin) > 1){
		token = strtok(line, " @,\n");
		equation_array[equation_num].x_start = atof(token);

		token = strtok(NULL, " @,\n");
		equation_array[equation_num].y_start = atof(token);

		token = strtok(NULL, " @,\n");
		token = strtok(NULL, " @,\n");
		equation_array[equation_num].x_vol = atof(token);

		token = strtok(NULL, " @,\n");
		equation_array[equation_num].y_vol = atof(token);

		printf("%lf %lf %lf %lf\n", equation_array[equation_num].x_start,
		equation_array[equation_num].y_start,
		equation_array[equation_num].x_vol,
		equation_array[equation_num].y_vol);
		++equation_num;
	}

	for(int i = 0; i < equation_num; ++i){
		printf("%lf\n", (double) equation_array[i].y_vol / (double) equation_array[i].x_vol);
		equation_array[i].slope = equation_array[i].y_vol / equation_array[i].x_vol;
		equation_array[i].offset = equation_array[i].y_start - (equation_array[i].slope * equation_array[i].x_start);
	}

	for(int i = 0; i < equation_num; ++i){
		//printf("%d ", i);
		for(int j = i + 1; j < equation_num; ++j){
		//	printf("%d ", j);
			if(fabs(equation_array[i].slope - equation_array[j].slope) < 0.0000000001){
				if(fabs(equation_array[i].offset - equation_array[j].offset) < 0.001){
					++inter_count;
				}
			//	printf("what\n");
			//	exit(0);
				continue;
			}

			double temp_x = (equation_array[i].offset - equation_array[j].offset) /
			(equation_array[j].slope - equation_array[i].slope);

			double temp_y = (equation_array[i].slope * temp_x) + equation_array[i].offset;

			//printf("y = %lfx + %lf %d\n", equation_array[i].slope, equation_array[i].offset, i);
			//printf("y = %lfx + %lf %d\n", equation_array[j].slope, equation_array[j].offset, j);
			//printf("x:%lf y:%lf\n\n", temp_x, temp_y);

			if(MIN <= temp_x && MAX >= temp_x && MIN <= temp_y && MAX >= temp_y && check_past(temp_x, temp_y, &equation_array[i]) &&
			check_past(temp_x, temp_y, &equation_array[j])){
			//	printf("through x:%lf y:%lf %lf\n\n", temp_x, temp_y, MIN);
				++inter_count;
			}
		}
		//printf("\n");
	}

	printf("%ld\n", inter_count);

	return 0;
}

//checks if point is in the past for cords
int check_past(double x, double y, equation *eqp)
{
	if(x > eqp->x_start && eqp->x_vol < 0)
		return 0;
	if(x < eqp->x_start && eqp->x_vol > 0)
		return 0;

	if(y > eqp->y_start && eqp->y_vol < 0)
		return 0;
	if(y < eqp->y_start && eqp->y_vol > 0)
		return 0;
	return 1;
}
