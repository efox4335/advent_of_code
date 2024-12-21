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
 * backwards does not always yeld the shortest input because this is a limited version of the traveling salesmen problem
 * eg the visit order provided by << ^^ may be shorter for a later robot than ^^ <<
 * every time 2 directions appear in order like this both options are tried
 * each gap between a presses can be processed separately it's length does not affect later gaps
 * this is because returning to a resets the state of all above robots because to press a all previous robots must press a
*/
#include <stdio.h>
#include <stdlib.h>

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
int get_keypad_dist(char button_1, char button_2, directions *dir)
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

//sets robot_control to the button presses reqired to input code
void get_key_pad_robot_control(char *code, char *robot_control)
{
	int control_index = 0;

	char cur_place = 'A';

	for(int i = 0; code[i] != '\n'; ++i){
		directions dir;

		get_keypad_dist(cur_place, code[i], &dir);
		cur_place = code[i];

		encode_dir(dir.dir_1, &robot_control[control_index]);
		control_index += dir.dir_1.mag;
		encode_dir(dir.dir_2, &robot_control[control_index]);
		control_index += dir.dir_2.mag;

		robot_control[control_index] = 'A';
		++control_index;
		robot_control[control_index] = '\0';
	}
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

//input is set to produse output on another robot
void get_control_robot_input(char *output, char *input)
{
	int control_index = 0;

	char cur_place = 'A';

	for(int i = 0; output[i] != '\0'; ++i){
		directions dir;

		get_robot_control_dir(cur_place, output[i], &dir);
		cur_place = output[i];

		encode_dir(dir.dir_1, &input[control_index]);
		control_index += dir.dir_1.mag;
		encode_dir(dir.dir_2, &input[control_index]);
		control_index += dir.dir_2.mag;

		input[control_index] = 'A';
		++control_index;
		input[control_index] = '\0';
	}
}

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	char keypad_inputs[1000];
	char robot_input_1[1000];
	char robot_input_2[1000];

	while(getline(&input_line, &lim, stdin) > 1){
		get_key_pad_robot_control(input_line, keypad_inputs);
		printf("%s\n", keypad_inputs);
		get_control_robot_input(keypad_inputs, robot_input_1);
		printf("%s\n", robot_input_1);
		get_control_robot_input(robot_input_1, robot_input_2);

		printf("%s\n\n", robot_input_2);
	}

	free(input_line);
	return 0;
}
