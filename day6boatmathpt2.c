/*
get list of times and distances
put times into quadratic equation calculate times based on output rounded up
upper_bound = (-time - 2root(time^2 - 4*-1*-record))/-2
lower_bound = (-time + 2root(time^2 - 4*-1*-record))/-2

sum_winning = ceil(upper_bound) - floor(lower_bound) + 1

get the number of ways i could win record the product
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct game_data{
	long int game_time;
	long int game_record;
};

int main(void)
{
	long int prod_winning = 1;
	int boat_speed = 1;//speed of boat i'm given
	long int sum_winning = 0;
	double lower_bound;
	double upper_bound;
	long int dist = 0;
	int button_time_held = 0;
	struct game_data games[100];
	int game_index = 0;
	size_t lim = 0;
	char *line = NULL;
	char *number = (char *) malloc(sizeof(char) * 100);
	char real_number[1000] = "\0";

	for(int i = 0; getline(&line, &lim, stdin) > 1; ++i){
		game_index = 0;
		number = strtok(line, " "); // splits input to tokens

		while((number = strtok(NULL, " ")) != NULL){// fits iteration skips the not numbers
			strcat(real_number, number);
		}
		printf("hi2 %s\n", real_number);


		if(i == 0)//games and times are on different lines
			games[game_index++].game_time = atol(real_number);
		else
			games[game_index++].game_record = atol(real_number);
		free(line);
		real_number[0] = '\0';
		line = NULL;
	}

	printf("%ld\n", games[0].game_record);
	printf("%ld\n", games[0].game_time);

	for(int i = 0; i < game_index; ++i, sum_winning = 0){
		double time = (double) games[i].game_time;
		double record = (double) games[i].game_record;

		upper_bound = (-time - sqrt(pow(time, 2) + (-4.0) * (-1.0) * (-record)))/-2.0;
		lower_bound = (-time + sqrt(pow(time, 2) + (-4.0) * (-1.0) * (-record)))/-2.0;

		sum_winning = (int) ((ceil(upper_bound)) - (floor(lower_bound) + 1));
		printf("%lf %lf %d\n", ceil(upper_bound), floor(lower_bound) + 1, sum_winning);

		prod_winning *= sum_winning;
		printf("%d\n", prod_winning);
	}
	printf("%ld\n", prod_winning);
}
