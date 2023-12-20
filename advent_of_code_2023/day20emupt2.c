//first get all modules and outputs in strings
//add each module to array of modules
//add each output and input module to the modules
//store modules in tree structure with pointers to each connect module
//store instructions in array that will loop if INS_MAX is reached
//add module for rx when building tree if any modules output == rx set the output == rx
//find how wait till high calls to the function that call rx loop
//get lcm from it
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INS_MAX 10000
#define CONJ_MAX_INPUT 100
#define MAX_OUTPUT 100
#define MAX_MODULE_COUNT 1000
#define BUTTON_PUSH_AMMOUNT 10000000000000

enum module_type{
	FLIP,
	CONJ,
	BROAD
};

enum PULSE_TYPE{
	HIGH,
	LOW
};

struct module{
	char *mod_name;
	int module_type;
	void *module_pointer;
};

//if input pulse == HIGH output nothing
//if input pulse == LOW flip state and output state
//state starts LOW
struct flip_module{
	int curr_state;
	int num_of_output_modules;
	struct module *output_modules[MAX_OUTPUT];
};

//store most recent pulse from input modules tho this module
//when input pulse update state of connect module
//if all states are HIGH output LOW
//else output LOW
//default memory is all LOW
struct conj_module{
	struct module *input_modules[CONJ_MAX_INPUT];
	int input_state[CONJ_MAX_INPUT];
	int num_of_input_modules;
	int num_of_output_modules;
	struct module *output_modules[MAX_OUTPUT];
};

//sends input pulse to all output modules
struct broad_module{
	int num_of_output_modules;
	struct module *output_modules[MAX_OUTPUT];
};

struct instruction{
	struct module *module_in;
	int pulse;
	struct module *module_out;
};

struct module module_array[MAX_MODULE_COUNT];
struct instruction ins_que[INS_MAX];
struct module *broadcast_module;
struct module *rx;
int cur_ins = 0;//index of the next instruction to be done
int cur_ins_add = 1;//index of the next free spop for an instruction to be added
int module_count = 0;
long int button_press_count = 0;

long int total_high = 0;
long int total_low = 0;

int main(void)
{
	int find_module(char *);
	void add_module(char *);
	void get_module_data(struct module *, char *);
	int find_module(char *);
	void print_flip(struct module *, int);
	void print_conj(struct module *, int);
	void print_broad(struct module *, int);
	int push_button(void);

	char *line = NULL;
	size_t lim = 0;
	//temp storage for modules before they get put on module_array
	char module_strings[100][100];
	int rx_low = 0;

	rx = (struct module *) malloc(sizeof(struct module));
	rx->mod_name = strdup("rx");

	while(getline(&line, &lim, stdin) > 1){
		strcpy(module_strings[module_count], line);
		printf("%s", module_strings[module_count]);
		add_module(module_strings[module_count]);
		++module_count;
	}

	printf("\n");
	for(int i = 0; i < module_count; ++i){
		get_module_data(&module_array[i], module_strings[i]);
	}

	for(int i = 0; i < module_count; ++i){
		switch(module_array[i].module_type){
			case FLIP:
				print_flip(&module_array[i], 0);
				break;
			case CONJ:
				print_conj(&module_array[i], 1);
				break;
			case BROAD:
				print_broad(&module_array[i], 0);
				break;
		}
	}

	for(long int i = 0; i < BUTTON_PUSH_AMMOUNT; ++i){
		button_press_count = i + 1;
		rx_low = push_button();
		if(rx_low == 1){
			printf("low: %ld\n", i);
			return 0;
		}
	//	printf("hi\n\n\n\n\n");
	}

	printf("%ld\n", total_high * total_low);
	return 0;
}

//starts the chain of pulses
int push_button(void)
{
	void add_instruction(struct module *, struct module *, int);
	struct instruction *read_instrucions(void);
	void process_flip(struct instruction *);
	void process_conj(struct instruction *);
	void process_broad(struct instruction *);

	struct instruction *current_instruction;

	ins_que[0].module_in = NULL;
	ins_que[0].pulse = LOW;
	ins_que[0].module_out = broadcast_module;
	cur_ins = 0;
	cur_ins_add = 1;
	++total_low;

	while(cur_ins != cur_ins_add){
		current_instruction = read_instrucions();
		/*if(current_instruction->module_in != NULL){
			printf("ins: %s %s %d\n", current_instruction->module_in->mod_name,
			current_instruction->module_out->mod_name, current_instruction->pulse);
		}*/
		if(current_instruction->module_out == rx && current_instruction->pulse == LOW){
			printf("hi\n");
			return 1;
		}
		switch(current_instruction->module_out->module_type){
			case FLIP:
				process_flip(current_instruction);
				break;
			case CONJ:
				process_conj(current_instruction);
				break;
			case BROAD:
				process_broad(current_instruction);
				break;
		}
	}
	return 0;
}

//processes instructions for flip module
//adds output to ins_que
//counts the number of high and low signals
void process_flip(struct instruction *ci)
{
	void add_instruction(struct module *, struct module *, int);
	void inc_pulse(int);

	struct flip_module *fp = (struct flip_module *) ci->module_out->module_pointer;
	int pulse_type;

	if(ci->pulse == HIGH){
		return;
	}

	if(fp->curr_state == LOW){
		fp->curr_state = HIGH;
		pulse_type = HIGH;
	}
	else{
		fp->curr_state = LOW;
		pulse_type = LOW;
	}

	for(int i = 0; i < fp->num_of_output_modules; ++i){
		inc_pulse(pulse_type);

		if(fp->output_modules[i] == NULL)
			continue;

		add_instruction(ci->module_out, fp->output_modules[i], pulse_type);
	}
}

//processes instructions for conj module
//adds output to ins_que
//counts the number of high and low signals
void process_conj(struct instruction *ci)
{
	void add_instruction(struct module *, struct module *, int);
	void inc_pulse(int);
	void get_cycle(int);

	struct conj_module *cp = (struct conj_module *) ci->module_out->module_pointer;
	int pulse_type = LOW;

	for(int i = 0; i < cp->num_of_input_modules; ++i){
		if(ci->module_in == cp->input_modules[i])
			cp->input_state[i] = ci->pulse;
		if(cp->input_state[i] != HIGH)
			pulse_type = HIGH;
		if(cp->output_modules[0] == rx){
		//	printf("%d %ld ", cp->input_state[i], button_press_count);
			if(cp->input_state[i] == 0){
				get_cycle(i);
			//	printf("%i %ld\n", i, button_press_count);
			}
		}

	}

	if(cp->output_modules[0] == rx){
		//	printf("\n");
	}

	for(int i = 0; i < cp->num_of_output_modules; ++i){
		inc_pulse(pulse_type);

		if(cp->output_modules[i] == NULL)
			continue;

		add_instruction(ci->module_out, cp->output_modules[i], pulse_type);
	}
}

//processes instructions for broadcast module
//adds output to ins_que
//counts the number of high and low signals
void process_broad(struct instruction *ci)
{
	void add_instruction(struct module *, struct module *, int);
	void inc_pulse(int);

	struct broad_module *bp = (struct broad_module *) ci->module_out->module_pointer;
	int pulse_type = ci->pulse;

	for(int i = 0; i < bp->num_of_output_modules; ++i){
		inc_pulse(pulse_type);

		if(bp->output_modules[i] == NULL)
			continue;

		add_instruction(ci->module_out, bp->output_modules[i], pulse_type);
	}
}

//gets the inputs of the & module befor rx
void get_cycle(int i)
{
	enum val{
		ONE = 0,
		TWO = 1,
		THREE = 2,
		FOUR = 3
	};

	static long int button_cyc[4][1000000];
	static int num_of_occurrences[4] = {0, 0, 0, 0};
	static long int cyc_num[4] = {0, 0, 0, 0};
	long int lcmcalc(long int, long int);

	switch(i){
		case ONE:
			button_cyc[ONE][num_of_occurrences[ONE]] = button_press_count;
			if(num_of_occurrences[ONE] > 0){
				if(button_cyc[ONE][num_of_occurrences[ONE] - 1] == button_press_count)
					break;
				if(cyc_num[i] == 0){
					cyc_num[i] = button_cyc[i][num_of_occurrences[i]]
					 - button_cyc[i][num_of_occurrences[i] - 1];
				}
				else if(cyc_num[i] != button_cyc[i][num_of_occurrences[i]]
				- button_cyc[i][num_of_occurrences[i] - 1]){
						printf("oh no\n");
						exit(0);
				}
				printf("%d ", i);
				printf("%ld\n", cyc_num[i]);
			}
			else{
				printf("in: %d ", i);
				printf("%ld\n", button_cyc[ONE][num_of_occurrences[ONE]]);
			}
			++num_of_occurrences[ONE];
			break;
		case TWO:
			button_cyc[TWO][num_of_occurrences[TWO]] = button_press_count;
			if(num_of_occurrences[TWO] > 0){
				if(button_cyc[TWO][num_of_occurrences[TWO] - 1] == button_press_count)
					break;
				if(cyc_num[i] == 0){
					cyc_num[i] = button_cyc[i][num_of_occurrences[i]]
					 - button_cyc[i][num_of_occurrences[i] - 1];
				}
				else if(cyc_num[i] != button_cyc[i][num_of_occurrences[i]]
				- button_cyc[i][num_of_occurrences[i] - 1]){
						printf("oh no\n");
						exit(0);
				}
				printf("%d ", i);
				printf("%ld\n", cyc_num[i]);
			}
			else{
				printf("in: %d ", i);
				printf("%ld\n", button_cyc[TWO][num_of_occurrences[TWO]]);
			}
			++num_of_occurrences[TWO];
			break;
		case THREE:
			button_cyc[THREE][num_of_occurrences[THREE]] = button_press_count;
			if(num_of_occurrences[THREE] > 0){
				if(button_cyc[THREE][num_of_occurrences[THREE] - 1] == button_press_count)
					break;
				if(cyc_num[i] == 0){
					cyc_num[i] = button_cyc[i][num_of_occurrences[i]]
					 - button_cyc[i][num_of_occurrences[i] - 1];
				}
				else if(cyc_num[i] != button_cyc[i][num_of_occurrences[i]]
				- button_cyc[i][num_of_occurrences[i] - 1]){
						printf("oh no\n");
						exit(0);
				}
				printf("%d ", i);
				printf("%ld\n", cyc_num[i]);
			}
			else{
				printf("in: %d ", i);
				printf("%ld\n", button_cyc[THREE][num_of_occurrences[THREE]]);
			}
			++num_of_occurrences[THREE];
			break;
		case FOUR:
			if(button_cyc[FOUR][num_of_occurrences[FOUR] - 1] == button_press_count)
				break;
			button_cyc[FOUR][num_of_occurrences[FOUR]] = button_press_count;
			if(num_of_occurrences[FOUR] > 0){
				if(button_cyc[FOUR][num_of_occurrences[FOUR] - 1] == button_press_count)
					break;
				if(cyc_num[i] == 0){
					cyc_num[i] = button_cyc[i][num_of_occurrences[i]]
					 - button_cyc[i][num_of_occurrences[i] - 1];
				}
				else if(cyc_num[i] != button_cyc[i][num_of_occurrences[i]]
				- button_cyc[i][num_of_occurrences[i] - 1]){
						printf("oh no\n");
						exit(0);
				}
				printf("%d ", i);
				printf("%ld\n", cyc_num[i]);
			}
			else{
				printf("in: %d ", i);
				printf("%ld\n", button_cyc[FOUR][num_of_occurrences[FOUR]]);
			}
			++num_of_occurrences[FOUR];
			break;
	}
	long int temp_1 = button_cyc[0][0];
	long int temp_2 = button_cyc[1][0];
	long int temp_3 = button_cyc[2][0];
	long int temp_4 = button_cyc[3][0];
	long int *lowest;


	if(cyc_num[0] != 0 && cyc_num[1] != 0 && cyc_num[2] != 0 && cyc_num[3] != 0){
		printf("%ld\n", temp_1);
		printf("%ld\n\n", cyc_num[0]);
		printf("%ld\n", temp_2);
		printf("%ld\n\n", cyc_num[1]);
		printf("%ld\n", temp_3);
		printf("%ld\n\n", cyc_num[2]);
		printf("%ld\n", temp_4);
		printf("%ld\n", cyc_num[3]);

		temp_1 = lcmcalc(cyc_num[0], cyc_num[1]);
		temp_1 = lcmcalc(temp_1, cyc_num[2]);
		temp_1 = lcmcalc(temp_1, cyc_num[3]);

		printf("\n%ld\n", temp_1);
		exit(0);
	}

}

//increments total_high for pulse HIGH and total_low for pulse LOW
void inc_pulse(int pulse)
{
	if(pulse == HIGH)
		++total_high;
	else
		++total_low;
}

//read instruction from ins_que and ++cur_ins if cur_ins== INS_MAX cur_ins = 0
//returns pointer to instruction
struct instruction *read_instrucions(void)
{
	struct instruction *ip = &ins_que[cur_ins];
	++cur_ins;
	if(cur_ins == INS_MAX)
		cur_ins = 0;
	return ip;
}

//adds instruction from ins_que and ++cur_ins_add if cur_ins_add == INS_MAX cur_ins_add = 0
void add_instruction(struct module *in, struct module *out, int pulse)
{
	ins_que[cur_ins_add].module_in = in;
	ins_que[cur_ins_add].module_out = out;
	ins_que[cur_ins_add].pulse = pulse;
	++cur_ins_add;

	if(cur_ins_add == INS_MAX)
		cur_ins_add = 0;
}

//puts module name on module_array finds module type and calls either add_flip or add_conj puts type in module_type
void add_module(char *module_name)
{
	void add_flip(struct module *);
	void add_conj(struct module *);
	void add_brod(struct module *);

	char temp_mod_name[10];

	switch(module_name[0]){
		case '%':
			module_array[module_count].module_type = FLIP;
			temp_mod_name[0] = module_name[1];
			temp_mod_name[1] = module_name[2];
			temp_mod_name[2] = '\0';
			module_array[module_count].mod_name = strdup(temp_mod_name);
			add_flip(&module_array[module_count]);
			return;
		case '&':
			module_array[module_count].module_type = CONJ;
			temp_mod_name[0] = module_name[1];
			temp_mod_name[1] = module_name[2];
			temp_mod_name[2] = '\0';
			module_array[module_count].mod_name = strdup(temp_mod_name);
			add_conj(&module_array[module_count]);
			return;
		case 'b':
			broadcast_module = &module_array[module_count];
			module_array[module_count].module_type = BROAD;
			module_array[module_count].mod_name = strdup("broadcaster");
			add_brod(&module_array[module_count]);
			return;
	}
}

//adds flip module and starts it at LOW sets num_of_output_modules to 0
void add_flip(struct module *mp)
{
	struct flip_module *fp;
	fp = (struct flip_module *) malloc(sizeof(struct flip_module));
	fp->curr_state = LOW;
	fp->num_of_output_modules = 0;
	mp->module_pointer = fp;
}

//adds conj module sets num_of_input_modules to 0 and sets num_of_output_modules to 0 sets input_state 0 to CONJ_MAX_INPUT to LOW
void add_conj(struct module *mp)
{
	struct conj_module *fp;
	fp = (struct conj_module *) malloc(sizeof(struct conj_module));
	fp->num_of_input_modules = 0;
	fp->num_of_output_modules = 0;

	for(int i = 0; i < CONJ_MAX_INPUT; ++i)
		fp->input_state[i] = LOW;

	mp->module_pointer = fp;
}

//sets num_of_output_modules to 0
void add_brod(struct module *mp)
{
	struct broad_module *fp;
	fp = (struct broad_module *) malloc(sizeof(struct broad_module));
	fp->num_of_output_modules = 0;

	mp->module_pointer = fp;
}

//gets data for mp from module_data also adds any conj input modules to its input module_array
void get_module_data(struct module *mp, char *module_data)
{
	void add_inp_and_out(struct module *, char *);

	char *temp_data_store = strdup(module_data);
	char *module_outputs;

	module_outputs = strtok(temp_data_store, " ->,\n");

	while((module_outputs = strtok(NULL, " ->,\n")) != NULL){
		add_inp_and_out(mp, module_outputs);
	}
	free(temp_data_store);
}

//adds module to the output of mp and if mn is type conj adds mp to the input of mn
void add_inp_and_out(struct module *mp, char *mn)
{
	int find_module(char *);

	int add_module_index = find_module(mn);
	struct module *mp_add;

	if(add_module_index == -1)
		mp_add = rx;
	else
		mp_add = &module_array[add_module_index];


	switch(mp->module_type){
		case FLIP:
			struct flip_module *fmp = (struct flip_module *) mp->module_pointer;
			fmp->output_modules[fmp->num_of_output_modules] = mp_add;
			fmp->num_of_output_modules += 1;
			break;
		case CONJ:
			struct conj_module *cmp = (struct conj_module *) mp->module_pointer;
			cmp->output_modules[cmp->num_of_output_modules]
			= mp_add;
			cmp->num_of_output_modules += 1;
			break;
		case BROAD:
			struct broad_module *bmp = (struct broad_module *) mp->module_pointer;
			bmp->output_modules[bmp->num_of_output_modules] = mp_add;
			bmp->num_of_output_modules += 1;
			break;
	}

	if(mp_add != NULL && mp_add->module_type == CONJ){
		struct conj_module *cmp = (struct conj_module *) mp_add->module_pointer;
		cmp->input_modules[cmp->num_of_input_modules]= mp;
		cmp->num_of_input_modules += 1;
	}
}

//finds index of mod_name on modula_array
int find_module(char *mod_name)
{
	for(int i = 0; i < module_count; ++i){
		if(!strncmp(module_array[i].mod_name, mod_name, 2))
			return i;
	}

	return -1;
}

//mode 0 prints flip data like input
//mode 1 prints all flip data
void print_flip(struct module *mp, int mode)
{
	struct flip_module *fp = (struct flip_module *) mp->module_pointer;

	if(mode == 1){
		printf("curr state %d ", fp->curr_state);
	}

	printf("%%%s -> ", mp->mod_name);
	for(int i = 0; i < fp->num_of_output_modules; ++i){
		if(fp->output_modules[i] == NULL)
			continue;
		printf("%s, ", fp->output_modules[i]->mod_name);
	}
	printf("\n");
}

//mode 0 prints conj data like input
//mode 1 prints all conj data
void print_conj(struct module *mp, int mode)
{
	struct conj_module *cp = (struct conj_module *) mp->module_pointer;

	if(mode == 1){
		for(int i = 0; i < cp->num_of_input_modules; ++i){
			if(cp->input_modules[i] == NULL)
				continue;
			printf("%s, ", cp->input_modules[i]->mod_name);
		}
	}
	printf("&%s -> ", mp->mod_name);
	for(int i = 0; i < cp->num_of_output_modules; ++i){
		if(cp->output_modules[i] == NULL)
			continue;
		printf("%s, ", cp->output_modules[i]->mod_name);
	}

	printf("\n");
}

//mode 0 prints broad data like input
//mode 1 prints all broad data
void print_broad(struct module *mp, int mode)
{
	struct broad_module *bp = (struct broad_module *) mp->module_pointer;

	printf("%s -> ", mp->mod_name);
	for(int i = 0; i < bp->num_of_output_modules; ++i){
		if(bp->output_modules[i] == NULL)
			continue;
		printf("%s, ", bp->output_modules[i]->mod_name);
	}

	printf("\n");
}

//uses euclidean algorithm
long int get_gcd(long int a, long int b)
{
	while(1){
		if(a >= b){
			a -= b;
		}
		else if(a < b){
			b -= a;
		}
		if(a == 0)
			return b;
	}
}

/*
gets lcm form lcm(a, b) = (a*b)/gcd(a,b)
*/
long int lcmcalc(long int a, long int b)
{
	long int get_gcd(long int, long int);
	return (a * b)/ get_gcd(a,b);
}
