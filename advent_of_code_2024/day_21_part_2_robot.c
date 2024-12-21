/*
 * there are 3 robots
 * 	1 that presses the keypad
 * 	2 that press other robots keypads
 * robots panic if there is no button underneath there finger
 * all robots initally aim at the a key
 * keypad layout:
 * 	+---+---+---+
 *	| 7 | 8 | 9 |
 * 	+---+---+---+
 * 	| 4 | 5 | 6 |
 * 	+---+---+---+
 * 	| 1 | 2 | 3 |
 * 	+---+---+---+
 * 	    | 0 | A |
 * 	    +---+---+
 *
 * robot control layout:
 * 	    +---+---+
 * 	    | ^ | A |
 * 	+---+---+---+
 * 	| < | v | > |
 * 	+---+---+---+
 *
 * because of the multible layers of robots paths which only change directon once are faster
 * because of the no empty button rule for the keypad go right then down and up than left
 * for control robots go down than left and right than up
 *
 * backwards does not always yeld the shortest input
 * eg the visit order provided by << ^^ may be shorter for a later robot than ^^ <<
 * every time 2 directions appear in order like this both options are tried
 * each gap between a presses can be processed separately it's length does not affect later gaps
 * this is because returning to a resets the state of all above robots because to press a all previous robots must press a
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/edsa.h"

typedef struct{
	int row;
	int col;
}cord;

typedef struct{
	char dir;
	int mag;
}vector;

typedef struct{
	vector dir_1;
	vector dir_2;
}directions;

/*
 * if row is > 0 dir is down otherwise up
 * if col is > 0 dir is right otherwise left
 * sets dit to how to get from button_1 to button_2
 * returns weather dir can be inverted
*/
int get_keypad_dir(char button_1, char button_2, directions *dir)
{
	const cord seven = {0, 0};
	const cord eight = {0, 1};
	const cord nine = {0, 2};
	const cord four = {1, 0};
	const cord five = {1, 1};
	const cord six = {1, 2};
	const cord one = {2, 0};
	const cord two = {2, 1};
	const cord three = {2, 2};
	const cord zero = {3, 1};
	const cord A = {3, 2};
	const cord EMPTY = {3, 0};

	cord button_1_loc;

	switch(button_1){
	case '0':
		button_1_loc = zero;
		break;
	case '1':
		button_1_loc = one;
		break;
	case '2':
		button_1_loc = two;
		break;
	case '3':
		button_1_loc = three;
		break;
	case '4':
		button_1_loc = four;
		break;
	case '5':
		button_1_loc = five;
		break;
	case '6':
		button_1_loc = six;
		break;
	case '7':
		button_1_loc = seven;
		break;
	case '8':
		button_1_loc = eight;
		break;
	case '9':
		button_1_loc = nine;
		break;
	case 'A':
		button_1_loc = A;
		break;
	default:
		printf("get_keypad_dist error button_1 bad key %c\n", button_1);
		exit(1);
	}

	cord button_2_loc;

	switch(button_2){
	case '0':
		button_2_loc = zero;
		break;
	case '1':
		button_2_loc = one;
		break;
	case '2':
		button_2_loc = two;
		break;
	case '3':
		button_2_loc = three;
		break;
	case '4':
		button_2_loc = four;
		break;
	case '5':
		button_2_loc = five;
		break;
	case '6':
		button_2_loc = six;
		break;
	case '7':
		button_2_loc = seven;
		break;
	case '8':
		button_2_loc = eight;
		break;
	case '9':
		button_2_loc = nine;
		break;
	case 'A':
		button_2_loc = A;
		break;
	default:
		printf("get_keypad_dist error button_2 bad key %c\n", button_2);
		exit(1);
	}

	//default set to be vertical
	vector temp_dir_1 = {(button_2_loc.row > button_1_loc.row)? 'v' : '^', button_2_loc.row - button_1_loc.row};

	//defaule set to be horizontal
	vector temp_dir_2 = {(button_2_loc.col > button_1_loc.col)? '>' : '<', button_2_loc.col - button_1_loc.col};

	int can_invert = 1;

	if(button_1_loc.row + temp_dir_1.mag == EMPTY.row && button_1_loc.col == EMPTY.col){
		can_invert = 0;
		vector swap_temp = temp_dir_1;
		temp_dir_1 = temp_dir_2;
		temp_dir_2 = swap_temp;
	}else if(button_1_loc.row == EMPTY.row && button_1_loc.col + temp_dir_2.mag == EMPTY.col){
		can_invert = 0;
	}

	temp_dir_1.mag = abs(temp_dir_1.mag);
	temp_dir_2.mag = abs(temp_dir_2.mag);

	dir->dir_1 = temp_dir_1;
	dir->dir_2 = temp_dir_2;

	return can_invert;
}

//null terminates output
void encode_dir(vector enc, char *output)
{
	char val = enc.dir;

	for(int i = 0; i < enc.mag; ++i){
		output[i] = val;
	}

	output[enc.mag] = '\0';
}

/*
 * if row is > 0 dir is down otherwise up
 * if col is > 0 dir is right otherwise left
 * sets dit to how to get from button_1 to button_2
 * returns weather dir can be inverted
*/
int get_robot_control_dir(char button_1, char button_2, directions *dir)
{
	const cord UP = {0, 1};
	const cord A = {0, 2};
	const cord LEFT = {1, 0};
	const cord DOWN = {1, 1};
	const cord RIGHT = {1, 2};
	const cord EMPTY = {0, 0};

	cord button_1_loc;

	switch(button_1){
	case '^':
		button_1_loc = UP;
		break;
	case 'A':
		button_1_loc = A;
		break;
	case '<':
		button_1_loc = LEFT;
		break;
	case 'v':
		button_1_loc = DOWN;
		break;
	case '>':
		button_1_loc = RIGHT;
		break;
	default:
		printf("get_robot_control_dir button 1 invalid value %c\n", button_1);
		exit(1);
	}

	cord button_2_loc;

	switch(button_2){
	case '^':
		button_2_loc = UP;
		break;
	case 'A':
		button_2_loc = A;
		break;
	case '<':
		button_2_loc = LEFT;
		break;
	case 'v':
		button_2_loc = DOWN;
		break;
	case '>':
		button_2_loc = RIGHT;
		break;
	default:
		printf("get_robot_control_dir button 2 invalid value %c\n", button_2);
		exit(1);
	}

	//default set to be vertical
	vector temp_dir_1 = {(button_2_loc.row > button_1_loc.row)? 'v' : '^', button_2_loc.row - button_1_loc.row};

	//defaule set to be horizontal
	vector temp_dir_2 = {(button_2_loc.col > button_1_loc.col)? '>' : '<', button_2_loc.col - button_1_loc.col};

	int can_invert = 1;

	if(button_1_loc.row + temp_dir_1.mag == EMPTY.row && button_1_loc.col == EMPTY.col){
		can_invert = 0;
		vector swap_temp = temp_dir_1;
		temp_dir_1 = temp_dir_2;
		temp_dir_2 = swap_temp;
	}else if(button_1_loc.row == EMPTY.row && button_1_loc.col + temp_dir_2.mag == EMPTY.col){
		can_invert = 0;
	}

	temp_dir_1.mag = abs(temp_dir_1.mag);
	temp_dir_2.mag = abs(temp_dir_2.mag);

	dir->dir_1 = temp_dir_1;
	dir->dir_2 = temp_dir_2;

	return can_invert;
}

typedef struct{
	char output[1000];
	int num;
}call_parem;

/*
 * sets starting point to 'A'
 * returns the length of the shortest possable input that could have produced output
*/
long get_shortest_input_recur(char *output, int num, edsa_htable *shortest_input_cashe)
{
	call_parem args;
	memset(args.output, 0, 1000);
	strcpy(args.output, output);
	args.num = num;

	long ret_val;

	if(edsa_htable_read(shortest_input_cashe, &args, &ret_val) == EDSA_SUCCESS){
		return ret_val;
	}

	if(num == 0){
		return strlen(output);
	}

	char last_pos = 'A';
	long input_len = 0;

	for(int i = 0; output[i] != '\0'; ++i){
		directions temp;
		int can_inv = get_robot_control_dir(last_pos, output[i], &temp);
		last_pos = output[i];

		char temp_output[1000];
		int temp_output_len = 0;



		encode_dir(temp.dir_1, &temp_output[temp_output_len]);
		temp_output_len += temp.dir_1.mag;
		encode_dir(temp.dir_2, &temp_output[temp_output_len]);
		temp_output_len += temp.dir_2.mag;
		temp_output[temp_output_len] = 'A';
		temp_output[temp_output_len + 1] = '\0';

		long or_1_len = get_shortest_input_recur(temp_output, num - 1, shortest_input_cashe);

		if(can_inv){
			temp_output_len = 0;
			encode_dir(temp.dir_2, &temp_output[temp_output_len]);
			temp_output_len += temp.dir_2.mag;
			encode_dir(temp.dir_1, &temp_output[temp_output_len]);
			temp_output_len += temp.dir_1.mag;
			temp_output[temp_output_len] = 'A';
			temp_output[temp_output_len + 1] = '\0';

			long or_2_len = get_shortest_input_recur(temp_output, num - 1, shortest_input_cashe);

			if(or_2_len < or_1_len){
				input_len += or_2_len;
			}else{
				input_len += or_1_len;
			}
		}else{
			input_len += or_1_len;
		}
	}

	edsa_htable_ins(shortest_input_cashe, &args, &input_len);

	return input_len;
}

/*
 * returns the length of the shortest possable input that could have produced output
*/
long get_shortest_input(char *output)
{
	char last_pos = 'A';
	long input_len = 0;

	edsa_htable *shortest_input_cashe = NULL;
	edsa_htable_init(&shortest_input_cashe, sizeof(call_parem), sizeof(long), 10000);

	for(int i = 0; output[i] != '\n'; ++i){
		directions temp;
		int can_inv = get_keypad_dir(last_pos, output[i], &temp);
		last_pos = output[i];

		char temp_output[1000];
		int temp_output_len = 0;

		encode_dir(temp.dir_1, &temp_output[temp_output_len]);
		temp_output_len += temp.dir_1.mag;
		encode_dir(temp.dir_2, &temp_output[temp_output_len]);
		temp_output_len += temp.dir_2.mag;
		temp_output[temp_output_len] = 'A';
		temp_output[temp_output_len + 1] = '\0';

		long or_1_len = get_shortest_input_recur(temp_output, 25, shortest_input_cashe);

		if(can_inv){
			temp_output_len = 0;
			encode_dir(temp.dir_2, &temp_output[temp_output_len]);
			temp_output_len += temp.dir_2.mag;
			encode_dir(temp.dir_1, &temp_output[temp_output_len]);
			temp_output_len += temp.dir_1.mag;
			temp_output[temp_output_len] = 'A';
			temp_output[temp_output_len + 1] = '\0';

			long or_2_len = get_shortest_input_recur(temp_output, 25, shortest_input_cashe);

			if(or_2_len < or_1_len){
				input_len += or_2_len;
			}else{
				input_len += or_1_len;
			}
		}else{
			input_len += or_1_len;
		}
	}

	edsa_htable_free(shortest_input_cashe);

	return input_len;
}

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	long complex = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		long len = get_shortest_input(input_line);

		char *temp_num = strtok(input_line, "A\n");

		complex += (atoi(temp_num) * len);
	}

	printf("%ld\n", complex);

	free(input_line);
	return 0;
}
