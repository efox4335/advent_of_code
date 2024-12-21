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

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 1){
	}

	free(input_line);
	return 0;
}
