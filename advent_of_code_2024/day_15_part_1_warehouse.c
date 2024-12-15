/*
 * just a simple simulation
*/

#include <stdio.h>
#include <stdlib.h>

enum{BUF_SIZE = 200};
enum{UP, DOWN, LEFT, RIGHT};

int main(void)
{
	enum{WAREHOUSE, MOVEMENTS};

	char *input_line = NULL;
	size_t lim = 0;

	char warehouse[BUF_SIZE][BUF_SIZE];
	int move_arr[70000];
	int move_arr_len = 0;
	int parse_part = WAREHOUSE;
	int warehouse_size = 0;

	while(getline(&input_line, &lim, stdin) > 0){
		if(input_line[0] == '\n'){
			parse_part = MOVEMENTS;
			continue;
		}

		if(parse_part == WAREHOUSE){
			for(int i = 0; input_line[i] != '\n'; ++i){
				warehouse[warehouse_size][i] = input_line[i];
			}

			++warehouse_size;
		}else{
			for(int i = 0; input_line[i] != '\n'; ++i){
				switch(input_line[i]){
				case '^':
					move_arr[move_arr_len] = UP;
					++move_arr_len;
					break;
				case 'v':
					move_arr[move_arr_len] = DOWN;
					++move_arr_len;
					break;
				case '<':
					move_arr[move_arr_len] = LEFT;
					++move_arr_len;
					break;
				case '>':
					move_arr[move_arr_len] = RIGHT;
					++move_arr_len;
					break;
				default:
					printf("unrecognized char %c %d", input_line[i], input_line[i]);
					break;
				}
			}
		}
	}

	free(input_line);
	return 0;
}
