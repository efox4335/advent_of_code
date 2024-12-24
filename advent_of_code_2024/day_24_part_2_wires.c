/*
 * take in wires so that each is assigned a number
 * with that number the wires state can be looked up as well as the 2 wires and the op that is needed to set it
 * for each z wire recurse through wires need to set it till already set wires are found and set the z wires
 *
 * print circut digram
 * through testing no wires are compeletly cut out from main processing
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/edsa.h"

enum{NOT_SET = -1, NO_WIRE = -2};
enum{MAX_WIRE_COUNT = 1000};
enum{XOR, OR, AND};

typedef struct{
	int in_1;
	int in_2;
	int op;
}gate;

int get_state(int wire, gate *gates, int *wire_states){
	if(wire_states[wire] == NO_WIRE){
		printf("error wire dependency failed\n");
		exit(1);
	}

	if(wire_states[wire] != NOT_SET){
		return wire_states[wire];
	}

	int state_1 = get_state(gates[wire].in_1, gates, wire_states);
	int state_2 = get_state(gates[wire].in_2, gates, wire_states);

	int cur_state = -1;
	switch(gates[wire].op){
	case XOR:
		cur_state = state_1 ^ state_2;
		break;
	case OR:
		cur_state = state_1 | state_2;
		break;
	case AND:
		cur_state = state_1 & state_2;
		break;
	default:
		printf("error bad state %d for wire %d\n", gates[wire].op, wire);
		exit(1);
	}

	wire_states[wire] = cur_state;
	return cur_state;
}

//prints all wires and ops wire_id depends on indents based on how many levels deep it is
void print_cir(int wire_id, char wire_names[MAX_WIRE_COUNT][4], gate *gates, int level)
{
	for(int i = 0; i < level; ++i){
		printf(" ");
	}

	char *name = wire_names[wire_id];

	printf("%s ", name);

	if(name[0] == 'x' || name[0] == 'y'){
		printf("\n");
		return;
	}

	switch(gates[wire_id].op){
	case XOR:
		printf("XOR\n");
		break;
	case OR:
		printf("OR\n");
		break;
	case AND:
		printf("AND\n");
		break;
	default:
		printf("no op for wire %d\n", wire_id);
		exit(1);
	}

	print_cir(gates[wire_id].in_1, wire_names, gates, level + 1);
	print_cir(gates[wire_id].in_2, wire_names, gates, level + 1);
}

int main(void)
{
	int wire_states[MAX_WIRE_COUNT];

	for(int i = 0; i < MAX_WIRE_COUNT; ++i){
		wire_states[i] = NO_WIRE;
	}

	edsa_htable *wire_ids = NULL;

	edsa_htable_init(&wire_ids, sizeof(char) * 3, sizeof(int), 1000);

	char *input_line = NULL;
	size_t lim = 0;

	enum{INI_STATES, GATES};
	int part = INI_STATES;

	int wire_count = 0;

	char ini_delim[] = " :\n";
	char gates_delim[] = " ->\n";

	//indexed by output wire id
	gate gate_arr[MAX_WIRE_COUNT];

	char id_to_name[MAX_WIRE_COUNT][4];

	for(int i = 0; i < MAX_WIRE_COUNT; ++i){
		id_to_name[i][0] = '\0';
		id_to_name[i][1] = '\0';
		id_to_name[i][2] = '\0';
		id_to_name[i][3] = '\0';
	}

	while(getline(&input_line, &lim, stdin) > 0){
		if(input_line[0] == '\n'){
			part = GATES;
			continue;
		}

		if(part == INI_STATES){
			char *wire_name = strtok(input_line, ini_delim);
			edsa_htable_ins(wire_ids, wire_name, &wire_count);

			strcpy(id_to_name[wire_count], wire_name);

			char *wire_state = strtok(NULL, ini_delim);

			wire_states[wire_count] = atoi(wire_state);

			++wire_count;
		}else{
			char *temp = strtok(input_line, gates_delim);
			int wire_1;

			if(edsa_htable_read(wire_ids, temp, &wire_1) == EDSA_HTABLE_READ_NO_ENTRY){
				edsa_htable_ins(wire_ids, temp, &wire_count);
				wire_1 = wire_count;

				strcpy(id_to_name[wire_count], temp);

				wire_states[wire_1] = NOT_SET;

				++wire_count;
			}

			temp = strtok(NULL, gates_delim);
			int op;

			switch(temp[0]){
			case 'X':
				op = XOR;
				break;
			case 'O':
				op = OR;
				break;
			case 'A':
				op = AND;
				break;
			default:
				printf("error bad op %s\n", temp);
				return 1;
			}

			temp = strtok(NULL, gates_delim);
			int wire_2;

			if(edsa_htable_read(wire_ids, temp, &wire_2) == EDSA_HTABLE_READ_NO_ENTRY){
				edsa_htable_ins(wire_ids, temp, &wire_count);
				wire_2 = wire_count;

				strcpy(id_to_name[wire_count], temp);

				wire_states[wire_2] = NOT_SET;

				++wire_count;
			}

			temp = strtok(NULL, gates_delim);
			int wire_3;

			if(edsa_htable_read(wire_ids, temp, &wire_3) == EDSA_HTABLE_READ_NO_ENTRY){
				edsa_htable_ins(wire_ids, temp, &wire_count);
				wire_3 = wire_count;

				strcpy(id_to_name[wire_count], temp);

				wire_states[wire_3] = NOT_SET;

				++wire_count;
			}

			gate_arr[wire_3].in_1 = wire_1;
			gate_arr[wire_3].in_2 = wire_2;
			gate_arr[wire_3].op = op;
		}
	}

	unsigned long output = 0;

	for(int i = 0; i < 10; ++i){
		for(int j = 0; j < 10; ++j){
			char wire[3];
			wire[0] = 'z';
			wire[1] = i + '0';
			wire[2] = j + '0';

			int wire_id = 0;

			if(edsa_htable_read(wire_ids, wire, &wire_id) == EDSA_HTABLE_READ_NO_ENTRY){
				goto end_loop;
			}

			print_cir(wire_id, id_to_name, gate_arr, 0);

			output += ((long) get_state(wire_id, gate_arr, wire_states)) << ((i * 10) + j);
		}
	}
end_loop:

	printf("%lu\n", output);

	for(int i = 0; i < wire_count; ++i){
		if(wire_states[i] == NOT_SET){
			print_cir(i, id_to_name, gate_arr, 0);
		}
	}

	edsa_htable_free(wire_ids);
	free(input_line);
	return 0;
}
