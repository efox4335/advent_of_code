/*
 * write an emulator to emulate the cpu
 * store opcodes and operands on a stack a parser will decode the op
 * each op will have it's own function
 * operands will be resolved by the ins parser
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum{
	ADV = 0,
	BXL = 1,
	BST = 2,
	JNZ = 3,
	BXC = 4,
	OUT = 5,
	BDV = 6,
	CDV = 7
};

typedef struct{
	long reg_a;
	long reg_b;
	long reg_c;
}reg_file;

//op code0 combo operand
//returns reg_a/(2^cop_1) stores in reg_a
//equal to reg_a >>= cop_1
long adv(long reg_a, long cop_1)
{
	return reg_a >> cop_1;
}

//opcode 1 literal operand
//returns reg_b ^ lop_1 stores in reg_b
long bxl(long reg_b, long lop_1)
{
	return reg_b ^ lop_1;
}

//opcode 2 combo operand
//returns cop_1 % 8 stores in reg_b
long bst(long cop_1)
{
	return cop_1 % 8;
}

//opcode 3 literal operand
//moves ins_ptr lop_1 if reg_a != 0 if reg_a != 0 ins_ptr is not incremented
long jnz(long reg_a, long lop_1)
{
}

//opcode 4 takes operand but ignores
//returns reg_b ^ reg_c stores in reg_b
long bxc(long reg_b, long reg_c)
{
}

//opcode 5 combo operand
//returns cop_1 % 8 prints output (if there are multiple values seperates by coma)
long out(long cop_1)
{
}

//opcode 6 combo operand
//returns reg_a/(2^cop_1) stores in reg_b
//equal to reg_b = reg_a >> cop_1
long bdv(long reg_a, long cop_1)
{
}

//opcode 7 combo operand
//returns reg_a/(2^cop_1) stores in reg_c
//equal to reg_c = reg_a >> cop_1
long cdv(long reg_a, long cop_1)
{
}

//returns the correct operand for a combo operand
long parse_combo(int cop, const reg_file *register_file)
{
	long val = -1;

	switch(cop){
	case 0:
		val = 0;
		break;
	case 1:
		val = 1;
		break;
	case 2:
		val = 2;
		break;
	case 3:
		val = 3;
		break;
	case 4:
		val = register_file->reg_a;
		break;
	case 5:
		val = register_file->reg_b;
		break;
	case 6:
		val = register_file->reg_c;
		break;
	default:
		printf("error invalid combo operand %d\n", cop);
		exit(1);
	}

	return val;
}

int main(void)
{
	enum{REGA, REGB, REGC, INS_STACK};

	reg_file register_file;
	register_file.reg_a = -1;
	register_file.reg_b = -1;
	register_file.reg_c = -1;

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
			register_file.reg_a = atol(temp_ptr);
			parse_part = REGB;
			break;
		case REGB:
			temp_ptr = strtok(input_line, "Registr B:\n");
			register_file.reg_b = atol(temp_ptr);
			parse_part = REGC;
			break;
		case REGC:
			temp_ptr = strtok(input_line, "Registr C:\n");
			register_file.reg_c = atol(temp_ptr);
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

	char out_buf[1000];
	int buf_pos = 0;

	while(ins_stack[ins_ptr] != -1){
		int op = ins_stack[ins_ptr + 1];

		switch(ins_stack[ins_ptr]){
		case ADV:
			register_file.reg_a = adv(register_file.reg_a, parse_combo(op, &register_file));
			break;
		case BXL:
			register_file.reg_b = bxl(register_file.reg_b, op);
			break;
		case BST:
			register_file.reg_b = bst(parse_combo(op, &register_file));
			break;
		case JNZ:
			//-2 to account for increase of stack pointer
			ins_ptr = jnz(register_file.reg_a, op) - 2;
			break;
		case BXC:
			register_file.reg_b = bxc(register_file.reg_b, register_file.reg_c);
			break;
		case OUT:
			long temp = out(parse_combo(op, &register_file));
			if(buf_pos != 0){
				out_buf[buf_pos] = ',';
				++buf_pos;
			}

			//output will always be one char
			out_buf[buf_pos] = temp + '0';
			++buf_pos;

			out_buf[buf_pos] = '\n';
			out_buf[buf_pos + 1] = '\0';
			break;
		case BDV:
			register_file.reg_b = bdv(register_file.reg_a, parse_combo(op, &register_file));
			break;
		case CDV:
			register_file.reg_c = cdv(register_file.reg_a, parse_combo(op, &register_file));
			break;
		}

		ins_ptr += 2;
	}

	free(input_line);
	return 0;
}
