/*
 * write an emulator to emulate the cpu
 * store opcodes and operands on a stack a parser will decode the op
 * each op will have it's own function
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	enum{REGA, REGB, REGC, INS_STACK};

	long reg_a = -1;
	long reg_b = -1;
	long reg_c = -1;

	int parse_part = REGA;

	int ins_stack[1000];
	int ins_ptr = 0;//stack starts at 0 and grows to larger numbers -1 represents end of ins stack

	char *input_line = NULL;
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 0){
		if(input_line[0] == '\n'){
			continue;
		}

		char *temp_ptr = NULL;
		switch(parse_part){
		case REGA:
			temp_ptr = strtok(input_line, "Registr A:\n");
			reg_a = atol(temp_ptr);
			parse_part = REGB;
			break;
		case REGB:
			temp_ptr = strtok(input_line, "Registr B:\n");
			reg_b = atol(temp_ptr);
			parse_part = REGC;
			break;
		case REGC:
			temp_ptr = strtok(input_line, "Registr C:\n");
			reg_c = atol(temp_ptr);
			parse_part = INS_STACK;
			break;
		case INS_STACK:
			temp_ptr = strtok(input_line, ",\nProgram: ");
			while(temp_ptr != NULL){
				ins_stack[ins_ptr] = atoi(temp_ptr);
				++ins_ptr;

				temp_ptr = strtok(NULL, ",\nProgram: ");
			}

			ins_stack[ins_ptr] = -1;
			ins_ptr = 0;

			break;
		}
	}

	free(input_line);
	return 0;
}
