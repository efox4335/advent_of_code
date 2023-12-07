/*
note: i know i could save time by not caluclating the times in the middle of the arc im just being lazy
long int was needed because of the size of the numbers

get list of times and distances
combine times and distances into 1 big distance
put times into formula dist = boat_speed * button_time_held * (total_time - buttor_time_held)
compare dist to record (include times that match record?)
get the number of ways i could win record the product
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct game_data{
	long int game_time;
	long int game_record;
};

int main(void)
{
	long int prod_winning = 1;
	int boat_speed = 1;//speed of boat i'm given
	long int sum_winning = 0;
	long int dist = 0;
	int button_time_held = 0;
	struct game_data games[100];
	int game_index = 0;
	size_t lim = 0;
	char *line = NULL;
	char *number = NULL;
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
		for(button_time_held = 0; button_time_held <+ games[i].game_time; ++button_time_held){
			dist = boat_speed*button_time_held * (games[i].game_time - button_time_held);
			if(dist > games[i].game_record){
				++sum_winning;
			}
		}
		prod_winning *= sum_winning;
		printf("\n");
	}
	printf("%ld\n", prod_winning);
}
