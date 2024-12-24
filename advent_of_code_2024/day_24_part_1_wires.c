/*
 * take in wires so that each is assigned a number
 * with that number the wires state can be looked up as well as the 2 wires and the op that is needed to set it
 * for each z wire recurse through wires need to set it till already set wires are found and set the z wires
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/edsa.h"

enum{NOT_SET = -1, NO_WIRE = -2};
enum{MAX_WIRE_COUNT = 1000};

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

	while(getline(&input_line, &lim, stdin) > 0){
		if(input_line[0] == '\n'){
			part = GATES;
			continue;
		}

		if(part == INI_STATES){
			char *wire_name = strtok(input_line, ini_delim);
			edsa_htable_ins(wire_ids, wire_name, &wire_count);

			char *wire_state = strtok(NULL, ini_delim);

			wire_states[wire_count] = atoi(wire_state);

			++wire_count;
		}else{

		}
	}

	edsa_htable_free(wire_ids);
	free(input_line);
	return 0;
}
