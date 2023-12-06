/*
note: i know i could save time by not caluclating the times in the middle of the arc im just being lazy

get list of times and distances
put times into formula dist = boat_speed * button_time_held * (total_time - buttor_time_held)
compare dist to record (include times that match record?)
get the number of ways i could win record the product
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct game_data{
	int game_time;
	int game_record;
};

int main(void)
{
	int prod_winning = 1;
	int boat_speed = 1;//speed of boat i'm given
	int sum_winning = 0;
	int dist = 0;
	int button_time_held = 0;
	struct game_data games[100];
	int game_index = 0;
	size_t lim = 0;
	char *line;
	char *number = (char *) malloc(sizeof(char) * 100);

	for(int i = 0; getline(&line, &lim, stdin) > 1; ++i){
		game_index = 0;
		number = strtok(line, " "); // splits input to tokens

		while((number = strtok(NULL, " ")) != NULL){// fits iteration skips the not numbers
			if(i == 0)//games and times are on different lines
				games[game_index++].game_time = atoi(number);
			else
				games[game_index++].game_record = atoi(number);
		}
		free(line);
	}

	for(int i = 0; i < game_index; ++i, sum_winning = 0){
		for(button_time_held = 0; button_time_held <+ games[i].game_time; ++button_time_held){
			dist = boat_speed*button_time_held * (games[i].game_time - button_time_held);
			if(dist > games[i].game_record){
				printf("%d %d\n", dist, prod_winning);
				++sum_winning;
			}
		}
		prod_winning *= sum_winning;
		printf("\n");
	}
	printf("%d\n", prod_winning);
}
