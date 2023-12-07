/*
store each game in and array of struct with game num and possibility
first parce game numbers then parce cube count then cube color
store cube color names with max cubes then search with strncmp
store max cube numbers and compare if each game is possible
print sum of game names
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
	int game_num;
	int game_possible;
};

struct gamedata game_array[5000];
int game_data_index = 0;

/*
first find game string then get the game name and convert to int and store
find numbers then call strncmp to find which color it is
compare value with max color value
store is game is possiable
*/

int main(void)
{
	char game_line[1000];
	char temp_game_storage[1000];
	int game_name_total = 0;
	int c;
	int i = 0;
	int j = 0;

	int game_poss_check(char *);

	while((c = getchar()) != EOF){
		game_line[i++] = c;
		//printf("%c", c);
		if(c == '\n'){
			if(i == 1)
				continue;//skip blank lines

			game_line[i] = '\0';
			j = 0;
			i = 0;
			while(!isdigit(game_line[i]) && game_line[i] != '\0')
				++i;

			while(isdigit(game_line[i]) && game_line[i] != '\0'){
				temp_game_storage[j] = game_line[i];
				++i;
				++j;
			}

			temp_game_storage[j] = '\0';
			game_array[game_data_index].game_num = atoi(temp_game_storage);

			game_array[game_data_index].game_possible = game_poss_check(&(game_line[i]));
			printf("%d\n", game_array[game_data_index].game_possible);
			i = 0;
			++game_data_index;
		}
	}

	for(int temp_game_index = 0; temp_game_index != game_data_index; ++temp_game_index)
		if(game_array[temp_game_index].game_possible == 1)
			game_name_total += game_array[temp_game_index].game_num;

	printf("%d\n", game_name_total);

	return 0;
}

/*
recursively calls itself comparing the first number found until '\0' is reached
return 1 for possiable 0 for not
*/
int game_poss_check(char *game_line)
{
	int i, j;
	int temp_cube_val_storage = 0;
	char temp_game_storage[1000];
	int color;

	i = j = 0;

	while(!isdigit(game_line[i]) && game_line[i] != '\0')
		++i;

	if(game_line[i] == '\0'){
		return 1;
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

	if(atoi(temp_game_storage) > color_array[color].max_cubes)
		return 0;


	return game_poss_check(game_line + i);
}
