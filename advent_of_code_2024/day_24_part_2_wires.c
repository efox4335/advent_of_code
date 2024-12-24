/*
 * take in wires so that each is assigned a number
 * with that number the wires state can be looked up as well as the 2 wires and the op that is needed to set it
 * for each z wire recurse through wires need to set it till already set wires are found and set the z wires
 *
 * print circut digram
 * through testing no wires are compeletly cut out from main processing
 *
 * the addition works by for each bit xor the xor of the 2 input bits with weather previous bits generated a carry
 *
 * the rules for checking for carry bits are as follows
 * or
 * 	and (weather the 2 previous bits will generate a carry)
 * 		2 previous bits
 * 	and (weather the 2 previous bits + all bits before it would generate a carry)
 * 		or (weather the 2 bits before the previous bits generate a carry)
 * 		xor (if the 2 previous bits do not generate a carry)
 * 			2 previous bits
 *
 * this recursively goes till the bits before the 2 previous bits are the 0 bit then it takes the form
 * or
 * 	and
 * 		2 previous bits
 * 	and
 * 		xor
 * 			2 previous bits
 * 		and
 * 			2 bits before the previous bits
 *
 * if a wire leads to the wrong place then it will always break the pattern so find the eight wires that are always out of place
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/edsa.h"

enum{NOT_SET = -1, NO_WIRE = -2};
enum{MAX_WIRE_COUNT = 1000};
enum{XOR, OR, AND, NOP};

typedef struct{
	int in_1;
	int in_2;
	int op;
}gate;

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

int alpha_cmp(const void *a, const void *b)
{
	char *str_1 = ((char *) a);
	char *str_2 = ((char *) b);

	//convert from base ascii
	int num_1 = str_1[0] * 128 * 128 + str_1[1] * 128 + str_1[2];
	int num_2 = str_2[0] * 128 * 128 + str_2[1] * 128 + str_2[2];

	return num_1 > num_2;
}

/*
 * marks bad_wire for seen and when a circuit breaks the pattern
 * when a bad wire is found both it's inputs will be marked as bad
 * does not know what came before it in logic
*/
void find_bad(int wire_id, gate *gates, int bad_wire[MAX_WIRE_COUNT][2], char id_to_name[MAX_WIRE_COUNT][4], int level)
{
	if(gates[wire_id].op == NOP){
		return;
	}

	bad_wire[wire_id][0] += 1;

	const int in_1_id = gates[wire_id].in_1;
	const int in_2_id = gates[wire_id].in_2;

	const int in_1_op = gates[gates[wire_id].in_1].op;
	const int in_2_op = gates[gates[wire_id].in_2].op;

	int in_1_bit_num = (id_to_name[in_1_id][0] == 'x' || id_to_name[in_1_id][0] == 'y')? atoi(&id_to_name[in_1_id][1]) : -1;
	int in_2_bit_num = (id_to_name[in_2_id][0] == 'x' || id_to_name[in_2_id][0] == 'y')? atoi(&id_to_name[in_2_id][1]) : -1;

	switch(gates[wire_id].op){
	case XOR:
		//if one is nop and the other is not
		if(in_1_op != in_2_op && (in_1_op == NOP || in_2_op == NOP)){
			bad_wire[in_1_id][1] += 1;
			bad_wire[in_2_id][1] += 1;
			return;
		}

		//if both are nop
		if(in_1_op == NOP && in_2_op == NOP){
			//if it is at the wrong level
			if(in_1_bit_num != level){
				bad_wire[wire_id][1] += 1;
			}

			break;
		}

		//in case only one is broken
		if(in_1_op == XOR && in_2_op == XOR){
			if(gates[gates[in_1_id].in_1].op == NOP && gates[gates[in_2_id].in_1].op != NOP){
				bad_wire[in_2_id][1] += 1;
				break;
			}else if(gates[gates[in_1_id].in_1].op != NOP && gates[gates[in_2_id].in_1].op == NOP){
				bad_wire[in_1_id][1] += 1;
				break;
			}
		}

		//if it breaks the or xor input rule
		if(in_1_op == in_2_op){
			bad_wire[in_1_id][1] += 1;
			bad_wire[in_2_id][1] += 1;

			break;
		}

		//if the inputs are of the wrong gate type
		if(in_1_op == AND){
			bad_wire[in_1_id][1] += 1;
		}

		if(in_2_op == AND){
			bad_wire[in_2_id][1] += 1;
		}

		break;
	case OR:
		if(in_1_op != AND){
			bad_wire[in_1_id][1] += 1;
		}

		if(in_2_op != AND){
			bad_wire[in_2_id][1] += 1;
		}

		--level;

		break;
	case AND:
		if(in_1_op == NOP && in_2_op == NOP){
			if(in_1_bit_num != level && level > 1){
				bad_wire[wire_id][1] += 1;
				break;
			}else if(level == 1 && in_1_bit_num > 1){
				bad_wire[wire_id][1] += 1;
				break;
			}

			break;
		}

		if(in_1_op == in_2_op){
			bad_wire[in_1_id][1] += 1;
			bad_wire[in_2_id][1] += 1;

			break;
		}

		if(level == 1){
			if(in_1_op == OR){
				bad_wire[in_1_id][1] += 1;
			}

			if(in_2_op == OR){
				bad_wire[in_2_id][1] += 1;
			}
		}else{
			if(in_1_op == AND){
				bad_wire[in_1_id][1] += 1;
			}

			if(in_2_op == AND){
				bad_wire[in_2_id][1] += 1;
			}
		}

		break;
	}

	find_bad(in_1_id, gates, bad_wire, id_to_name, level);
	find_bad(in_2_id, gates, bad_wire, id_to_name, level);
}

int main(void)
{
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

	for(int i = 0; i < MAX_WIRE_COUNT; ++i){
		gate_arr[i].op = NOP;
	}

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

			++wire_count;
		}else{
			char *temp = strtok(input_line, gates_delim);
			int wire_1;

			if(edsa_htable_read(wire_ids, temp, &wire_1) == EDSA_HTABLE_READ_NO_ENTRY){
				edsa_htable_ins(wire_ids, temp, &wire_count);
				wire_1 = wire_count;

				strcpy(id_to_name[wire_count], temp);

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

				++wire_count;
			}

			temp = strtok(NULL, gates_delim);
			int wire_3;

			if(edsa_htable_read(wire_ids, temp, &wire_3) == EDSA_HTABLE_READ_NO_ENTRY){
				edsa_htable_ins(wire_ids, temp, &wire_count);
				wire_3 = wire_count;

				strcpy(id_to_name[wire_count], temp);

				++wire_count;
			}

			gate_arr[wire_3].in_1 = wire_1;
			gate_arr[wire_3].in_2 = wire_2;
			gate_arr[wire_3].op = op;
		}
	}

	//bad_wire[id][0] will be the amount of times the wire is seen and bad_wire[id][1] will be the amount of times it breaks the pattern
	int bad_wire[MAX_WIRE_COUNT][2];

	for(int i = 0; i < MAX_WIRE_COUNT; ++i){
		bad_wire[i][0] = 0;
		bad_wire[i][1] = 0;
	}

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


			if(gate_arr[wire_id].op != XOR && (i * 10) + j != 45){
				bad_wire[wire_id][1] += 1;
				if(gate_arr[wire_id].op == OR){
					find_bad(wire_id, gate_arr, bad_wire, id_to_name, (i * 10) + j + 1);
				}else{
					find_bad(wire_id, gate_arr, bad_wire, id_to_name, (i * 10) + j);
				}
			}else{
				find_bad(wire_id, gate_arr, bad_wire, id_to_name, (i * 10) + j);
			}//print_cir(wire_id, id_to_name, gate_arr, 0);
		}
	}
end_loop:

	char bad_gates[100];
	bad_gates[0] = '\0';

	for(int i = 0; i < MAX_WIRE_COUNT; ++i){
		if(bad_wire[i][0] != 0 && bad_wire[i][0] == bad_wire[i][1]){
			strcat(bad_gates, id_to_name[i]);
			strcat(bad_gates, ",");
		}
	}

	qsort(bad_gates, 8, 4, alpha_cmp);

	//erase last comma
	bad_gates[8 * 4 - 1] = '\0';

	printf("%s\n", bad_gates);

	edsa_htable_free(wire_ids);
	free(input_line);
	return 0;
}
