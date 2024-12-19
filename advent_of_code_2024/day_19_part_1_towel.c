/*
 * since no native regex engine in c i'll just make an nfa to represent
 * or all towel configs and any amount of them if the input is consumed than we have a match
*/
#include <stdio.h>
#include <stdlib.h>

//represents consuming no char for a state
enum{START_STATE = '\0', OR_STATE = '\r'};

typedef struct{
	char match;
	int next_state;
	int or_next_state;//only used for or states
}state;

/*
 * returns new state_count
 * adds new state by
 * adding an or state at the end
 * than making or_next_state == next_state of start
 * making next_state of start the index of the newly added state
 * the next_state of the new or is set to the next index of state_arr
 * the string of chars is added on the final char next_state is set to start
*/
int add_string(state *state_arr, int state_count, int start_state, char *str)
{
	state_arr[state_count].match = OR_STATE;
	state_arr[state_count].or_next_state = state_arr[start_state].next_state;
	state_arr[state_count].next_state = state_count + 1;

	state_arr[start_state].next_state = state_count;

	++state_count;

	for(int i = 0; str[i] != '\0'; ++i){
		state_arr[state_count].match = str[i];
		state_arr[state_count].next_state = state_count + 1;

		++state_count;
	}

	state_arr[state_count - 1].next_state = start_state;

	return state_count;
}

int main(void)
{
	enum{MATCHES, INPUTS};
	enum{START = 0};

	int cur_part = MATCHES;
	char *input_line = NULL;
	size_t lim = 0;

	state state_arr[3000];
	int state_count = 0;

	state_arr[START].match = START_STATE;
	state_arr[START].next_state = START;
	++state_count;

	while(getline(&input_line, &lim, stdin) > 0){

	}

	free(input_line);
	return 0;
}
