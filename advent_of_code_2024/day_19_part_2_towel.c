/*
 * since no native regex engine in c i'll just make an nfa to represent
 * or all towel configs and any amount of them if the input is consumed than we have a match
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//represents consuming no char for a state
enum{START_STATE = '\0', OR_STATE = '\r'};

typedef struct{
	char match;
	int next_state;
	int or_next_state;//only used for or states
}state;

/*
 * returns new state_count
 *
 * special case for the first string no or state is added
 *
 * adds new state by
 * adding an or state at the end
 * than making or_next_state == next_state of start
 * making next_state of start the index of the newly added state
 * the next_state of the new or is set to the next index of state_arr
 * the string of chars is added on the final char next_state is set to start
*/
int add_string(state *state_arr, int state_count, int start_state, char *str)
{
	if(state_count == 1){
		state_arr[start_state].next_state = state_count;
	}else{
		state_arr[state_count].match = OR_STATE;
		state_arr[state_count].or_next_state = state_arr[start_state].next_state;
		state_arr[state_count].next_state = state_count + 1;

		state_arr[start_state].next_state = state_count;

		++state_count;
	}

	for(int i = 0; str[i] != '\0'; ++i){
		state_arr[state_count].match = str[i];
		state_arr[state_count].next_state = state_count + 1;

		++state_count;
	}

	state_arr[state_count - 1].next_state = start_state;

	return state_count;
}

/*
 * returns the number of ways the end state could have been reached if one was added for next char
 * this is counted by adding the number each state could have been reached
 * for the start state this is equal to the sum of all ways each string ending this char could have been reached
 * for non start states this is equal to the way each state before it could be reached
*/
long match_char(char cur_char, state *state_arr, long cur_char_states[2][3000], long next_char_states[2][3000], int *next_char_state_count, int cur_char_state_count)
{
	long start_pos_reached = 0;
	int start_ind = 0;
	*next_char_state_count = 0;

	for(int i = 0; i < cur_char_state_count; ++i){
		if(state_arr[cur_char_states[0][i]].match == START_STATE){
			cur_char_states[0][cur_char_state_count] = state_arr[cur_char_states[0][i]].next_state;
			cur_char_states[1][cur_char_state_count] = cur_char_states[1][i];
			++cur_char_state_count;
		}else if(state_arr[cur_char_states[0][i]].match == OR_STATE){
			cur_char_states[0][cur_char_state_count] = state_arr[cur_char_states[0][i]].next_state;
			cur_char_states[1][cur_char_state_count] = cur_char_states[1][i];
			++cur_char_state_count;

			cur_char_states[0][cur_char_state_count] = state_arr[cur_char_states[0][i]].or_next_state;
			cur_char_states[1][cur_char_state_count] = cur_char_states[1][i];
			++cur_char_state_count;
		}else if(cur_char == state_arr[cur_char_states[0][i]].match){
			if(state_arr[cur_char_states[0][i]].next_state == 0){
				if(start_pos_reached == 0){
					start_pos_reached = cur_char_states[1][i];
					next_char_states[0][*next_char_state_count] = state_arr[cur_char_states[0][i]].next_state;
					next_char_states[1][*next_char_state_count] = start_pos_reached;
					start_ind = *next_char_state_count;
					*next_char_state_count += 1;
				}else{
					start_pos_reached += cur_char_states[1][i];
					next_char_states[1][start_ind] = start_pos_reached;
				}
			}else{
				next_char_states[0][*next_char_state_count] = state_arr[cur_char_states[0][i]].next_state;
				next_char_states[1][*next_char_state_count] = cur_char_states[1][i];
				*next_char_state_count += 1;
			}
		}
	}

	return start_pos_reached;
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

	char *temp_match = NULL;
	char delim[] = ", \n";

	long cur_char_states[2][3000];
	long next_char_states[2][3000];
	int next_char_state_count = 0;
	int cur_char_state_count = 0;

	long valid_pat_count = 0;

	while(getline(&input_line, &lim, stdin) > 0){
		switch(cur_part){
		case MATCHES:
			temp_match = strtok(input_line, delim);

			while(temp_match != NULL){
				state_count = add_string(state_arr, state_count, START, temp_match);

				temp_match = strtok(NULL, delim);
			}

			cur_part = INPUTS;
			break;
		case INPUTS:
			long valid_match = 0;
			void *arr_ptr = cur_char_states;
			void *arr_ptr_2 = next_char_states;
			void *temp_ptr = NULL;

			cur_char_state_count = 0;
			next_char_state_count = 0;

			cur_char_states[0][cur_char_state_count] = START;
			cur_char_states[1][cur_char_state_count] = 1;

			++cur_char_state_count;

			for(int i = 0; input_line[i] != '\n'; ++i){
				valid_match = match_char(input_line[i], state_arr, arr_ptr, arr_ptr_2, &next_char_state_count, cur_char_state_count);

				cur_char_state_count = next_char_state_count;

				temp_ptr = arr_ptr;
				arr_ptr = arr_ptr_2;
				arr_ptr_2 = temp_ptr;
			}

			valid_pat_count += valid_match;


			break;
		}
	}

	printf("%ld\n", valid_pat_count);

	free(input_line);
	return 0;
}
