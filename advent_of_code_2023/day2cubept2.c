/*
store each game with the min number to cubes required for eacch game
get the power of each game and add them together
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

struct colors{
	char *color;
	int max_cubes;
	int word_length;
};

struct colors color_array[] = {
	{"red", 12, sizeof("red")/ sizeof(char) - 1},
	{"green", 13, sizeof("green")/ sizeof(char) - 1},
	{"blue", 14, sizeof("blue")/ sizeof(char) - 1}
};

struct gamedata{
	int red_max;
	int green_max;
	int blue_max;
	int game_power;
};

struct gamedata game_array[5000];
int game_data_index = 0;

/*
find first number to filter out game name
then call min_cube_req to put the min cubes required in game_array
them calculate power and add them up
*/

int main(void)
{
	char game_line[1000];
	int game_power_total = 0;
	int c;
	int i = 0;

	void min_cube_req(char *);

	while((c = getchar()) != EOF){
		game_line[i++] = c;
		//printf("%c", c);
		if(c == '\n'){
			game_line[i] = '\0';
			if(i == 1)
				continue;//skip blank lines
			i = 0;

			while(!isdigit(game_line[i]) && game_line[i] != '\0')
				++i;

			while(isdigit(game_line[i]) && game_line[i] != '\0')// skips game number
				++i;

			game_array[game_data_index].red_max = 0;
			game_array[game_data_index].green_max = 0;
			game_array[game_data_index].blue_max = 0;
			min_cube_req(game_line + i);

			i = 0;
			++game_data_index;
		}
	}

	for(int temp_game_index = 0; temp_game_index != game_data_index; ++temp_game_index){
		printf("%d\n", game_array[temp_game_index].red_max);
		printf("%d\n", game_array[temp_game_index].green_max);
		printf("%d\n\n", game_array[temp_game_index].blue_max);
		game_array[temp_game_index].game_power = game_array[temp_game_index].red_max * game_array[temp_game_index].green_max * game_array[temp_game_index].blue_max;
	}

	for(int temp_game_index = 0; temp_game_index != game_data_index; ++temp_game_index){
		printf("%d\n\n", game_array[temp_game_index].game_power);
		game_power_total += game_array[temp_game_index].game_power;
	}

	printf("%d\n", game_power_total);

	return 0;
}

/*
recursively calls itself comparing the first number found until '\0' is reached
find the max color value for each game
*/
void min_cube_req(char *game_line)
{
	int i, j;
	char temp_game_storage[1000];
	int color;

	i = j = 0;

	while(!isdigit(game_line[i]) && game_line[i] != '\0')
		++i;

	if(game_line[i] == '\0'){
		return;
	}

	while(isdigit(game_line[i]) && game_line[i] != '\0'){
		temp_game_storage[j] = game_line[i];
		++i;
		++j;
	}
	temp_game_storage[j] = '\0';

	while(!isalpha(game_line[i]))
		++i;

	for(color = 0; color < sizeof(color_array)/ sizeof(struct colors); ++color)
		if(!strncmp(color_array[color].color, game_line + i, color_array[color].word_length))
			break;

	switch(color){
		case (0):
			if(atoi(temp_game_storage) > game_array[game_data_index].red_max)
				game_array[game_data_index].red_max = atoi(temp_game_storage);
			break;
		case (1):
			if(atoi(temp_game_storage) > game_array[game_data_index].green_max)
				game_array[game_data_index].green_max = atoi(temp_game_storage);
			break;
		case (2):
			if(atoi(temp_game_storage) > game_array[game_data_index].blue_max)
				game_array[game_data_index].blue_max = atoi(temp_game_storage);
			break;
	}

	min_cube_req(game_line + i);
}
