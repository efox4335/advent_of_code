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
 * working backwards seems to be good here
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int row;
	int col;
}cord;

/*
 * if row is > 0 dir is down otherwise up
 * if col is > 0 dir is right otherwise left
 * returns how to get from button_1 to button_2
*/
cord get_keypad_dist(char button_1, char button_2)
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

	cord dir = {button_2_loc.row - button_1_loc.row, button_2_loc.col - button_1_loc.col};

	return dir;
}

//null terminates output
void encode_dir(int dir, int count, char *output)
{
	char val = dir;

	for(int i = 0; i < count; ++i){
		output[i] = val;
	}

	output[count] = '\0';
}

//sets robot_control to the button presses reqired to input code
void get_key_pad_robot_control(char *code, char *robot_control)
{
	int control_index = 0;

	char cur_place = 'A';

	for(int i = 0; code[i] != '\n'; ++i){
		cord dir = get_keypad_dist(cur_place, code[i]);
		cur_place = code[i];

		//to avoid going over empty space
		if(dir.row > 0){
			if(dir.col > 0){
				encode_dir('>', dir.col, &robot_control[control_index]);
				control_index += dir.col;

				encode_dir('v', dir.row, &robot_control[control_index]);
				control_index += dir.row;
			}else{
				encode_dir('<', abs(dir.col), &robot_control[control_index]);
				control_index += abs(dir.col);

				encode_dir('v', dir.row, &robot_control[control_index]);
				control_index += dir.row;
			}

		}else{
			if(dir.col > 0){
				encode_dir('^', abs(dir.row), &robot_control[control_index]);
				control_index += abs(dir.row);

				encode_dir('>', dir.col, &robot_control[control_index]);
				control_index += dir.col;
			}else{
				encode_dir('^', abs(dir.row), &robot_control[control_index]);
				control_index += abs(dir.row);

				encode_dir('<', abs(dir.col), &robot_control[control_index]);
				control_index += abs(dir.col);
			}

		}

		robot_control[control_index] = 'A';
		++control_index;
		robot_control[control_index] = '\0';
	}
}

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	char keypad_inputs[1000];

	while(getline(&input_line, &lim, stdin) > 1){
		get_key_pad_robot_control(input_line, keypad_inputs);
	}

	free(input_line);
	return 0;
}
