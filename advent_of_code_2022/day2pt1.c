/*
get input and cound total
total = sum of res + play for all rounds
*/
#include <stdio.h>
#include <string.h>

enum round_points{
	LOSS = 0,
	DRAW = 3,
	WIN = 6
};//value of round outcomes

enum play_value{
	ROCK_PLAY = 1,
	PAPER_PLAY = 2,
	SCISSORS_PLAY = 3,
};//value of each play in points

enum play_type{
	ROCK,
	PAPER,
	SCISSORS,
	ERROR
};

//[round][player]
char plays[10000][2];
int round_count = 0;

int main(void)
{
	char *line = NULL;
	size_t lim = 0;

	int get_play(char);

	while(getline(&line, &lim, stdin) > 0){
		printf("%s", line);

		plays[round_count][0] = get_play(line[0]);
		plays[round_count][1] = get_play(line[2]);

		++round_count;
	}

	int total = 0;
	int find_winner(char, char);

	for(int i = 0; i < round_count; ++i){
		switch(plays[i][1]){
			case ROCK:
				total += ROCK_PLAY;
				break;
			case PAPER:
				total += PAPER_PLAY;
				break;
			case SCISSORS:
				total += SCISSORS_PLAY;
				break;
		}

		total += find_winner(plays[i][0], plays[i][1]);


		printf("%d %d %d\n", total, plays[i][0], plays[i][1]);
	}

	printf("%d\n", total);

	return 0;
}

//finds the winner of a round
int find_winner(char player_1, char player_2)
{
	switch(player_1){
	case ROCK:
		switch(player_2){
		case ROCK:
			return DRAW;
		case PAPER:
			return WIN;
		case SCISSORS:
			return LOSS;
		}
	case PAPER:
		switch(player_2){
		case ROCK:
			return LOSS;
		case PAPER:
			return DRAW;
		case SCISSORS:
			return WIN;
		}
	case SCISSORS:
		switch(player_2){
		case ROCK:
			return WIN;
		case PAPER:
			return LOSS;
		case SCISSORS:
			return DRAW;
		}
	}
}

//converts letter to play type
int get_play(char letter)
{
	switch(letter)
	{
		case 'A':
			return ROCK;
		case 'X':
			return ROCK;
		case 'B':
			return PAPER;
		case 'Y':
			return PAPER;
		case 'C':
			return SCISSORS;
		case 'Z':
			return SCISSORS;
		default:
			return ERROR;
	}

	return ERROR;
}
