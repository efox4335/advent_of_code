//get input of filters as strings
//put filters in tree structure with the conditions and pointers to each filter that could be next
//have 2 nodes for accepting or rejecting gears
//store what value it's compared against and what operation as 2 chars
//	for example a<2006 would be type[0] = a, type[1] = < and cond = 2006
//	for no test type[0] = n
//	for accept node first_type[0] = a
//	for reject node first_type[0] = r
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct filter{
	char *node_name;

	char first_type[2];
	int first_cond;
	struct filter *first_pointer;

	char second_type[2];
	int second_cond;
	struct filter *second_pointer;

	char third_type[2];
	int third_cond;
	struct filter *third_pointer;

	char fourth_type[2];
	int fourth_cond;
	struct filter *fourth_pointer;
};

struct gear{
	int x_num;
	int m_num;
	int a_num;
	int s_num;
};

//stores pointers to avoid creating the same structure twice
struct temp_store{
	char *filter_string;
	struct filter *filter_pointer;
};

struct filter *accept;
struct filter *reject;
struct filter *in_filter;
struct temp_store **filter_string_array;
struct gear gear_array[1000];
int filter_num = 0;
int gear_num = 0;
struct gear *accepted_gear_arr[1000];
int accepted_gear_num = 0;
long int acc_gear_type_total = 0;

int main(void)
{
	int find_index(char *);
	struct filter *add_filter(char *);
	void print_tree(struct filter *, int);
	void find_acc_gears(struct gear *, struct filter *);

	char *line = NULL;
	size_t lim = 0;
	char *filter_gear_nums;

	accept = (struct filter *) malloc(sizeof(struct filter));
	reject = (struct filter *) malloc(sizeof(struct filter));

	accept->first_type[0] = 'a';
	reject->first_type[0] = 'r';
	accept->node_name = strdup("A");
	reject->node_name = strdup("R");

//	stores the filter strings before they are put on the tree
	filter_string_array = (struct temp_store **) malloc(sizeof(struct temp_store) * 1000);

//	gets all filters
	while(getline(&line, &lim, stdin) > 1){
		*(filter_string_array + filter_num) = (struct temp_store *) malloc(sizeof(struct temp_store));
		(*(filter_string_array + filter_num))->filter_string = strdup(line);
		(*(filter_string_array + filter_num))->filter_pointer = NULL;
		++filter_num;
	}

	for(int i = 0; i < filter_num; ++i){
		printf("%p %s", (*(filter_string_array + i))->filter_pointer, (*(filter_string_array + i))->filter_string);
	}

	in_filter = add_filter("in");
	printf("\n\ntree:\n");
	print_tree(in_filter, 0);

	for(int i = 0; i < filter_num; ++i){
		free(*(filter_string_array + i));
	}

	free(filter_string_array);

//	gets gears
	while(getline(&line, &lim, stdin) > 1){
		filter_gear_nums = strtok(line, "{xmas,=}");
		gear_array[gear_num].x_num = atoi(filter_gear_nums);

		filter_gear_nums = strtok(NULL, "{xmas,=}");
		gear_array[gear_num].m_num = atoi(filter_gear_nums);

		filter_gear_nums = strtok(NULL, "{xmas,=}");
		gear_array[gear_num].a_num = atoi(filter_gear_nums);

		filter_gear_nums = strtok(NULL, "{xmas,=}");
		gear_array[gear_num].s_num = atoi(filter_gear_nums);

		++gear_num;
	}

	for(int i = 0; i < gear_num; ++i){
		printf("{x=%d,x=%d,a=%d,s=%d}\n", gear_array[i].x_num, gear_array[i].m_num, gear_array[i].a_num, gear_array[i].s_num);
		find_acc_gears(&gear_array[i], in_filter);
	}

	printf("\n");

	for(int i = 0; i < accepted_gear_num; ++i){
		printf("{x=%d,x=%d,a=%d,s=%d}\n", accepted_gear_arr[i]->x_num, accepted_gear_arr[i]->m_num,
		accepted_gear_arr[i]->a_num, accepted_gear_arr[i]->s_num);
		acc_gear_type_total += accepted_gear_arr[i]->x_num;
		acc_gear_type_total += accepted_gear_arr[i]->m_num;
		acc_gear_type_total += accepted_gear_arr[i]->a_num;
		acc_gear_type_total += accepted_gear_arr[i]->s_num;
	}

	printf("\n%ld\n", acc_gear_type_total);

	return 0;
}

//adds index of accepted gears to accepted_gear_arr
//recursively navs filter tree till either finds node accept or node reject
void find_acc_gears(struct gear *curr_gear, struct filter *curr_filter)
{
	if(curr_filter == accept){
		accepted_gear_arr[accepted_gear_num] = curr_gear;
		++accepted_gear_num;
		return;
	}

	if(curr_filter == reject){
		return;
	}
	int test_filter(struct gear *, char *, int);

	if(test_filter(curr_gear, curr_filter->first_type, curr_filter->first_cond)){
		find_acc_gears(curr_gear, curr_filter->first_pointer);
		return;
	}

	if(test_filter(curr_gear, curr_filter->second_type, curr_filter->second_cond)){
		find_acc_gears(curr_gear, curr_filter->second_pointer);
		return;
	}

	if(test_filter(curr_gear, curr_filter->third_type, curr_filter->third_cond)){
		find_acc_gears(curr_gear, curr_filter->third_pointer);
		return;
	}

	if(test_filter(curr_gear, curr_filter->fourth_type, curr_filter->fourth_cond)){
		find_acc_gears(curr_gear, curr_filter->fourth_pointer);
		return;
	}
}

//test if gear passes a filter cond
//returns 1 for pass 0 for fail
int test_filter(struct gear *curr_gear, char *type, int cond)
{
	switch(type[0]){
		case 'n':
			return 1;
		case 'x':
			if(type[1] == '>'){
				if(curr_gear->x_num > cond)
					return 1;
				return 0;
			}
			if(type[1] == '<'){
				if(curr_gear->x_num < cond)
					return 1;
				return 0;
			}
		case 'm':
			if(type[1] == '>'){
				if(curr_gear->m_num > cond)
					return 1;
				return 0;
			}
			if(type[1] == '<'){
				if(curr_gear->m_num < cond)
					return 1;
				return 0;
			}
		case 'a':
			if(type[1] == '>'){
				if(curr_gear->a_num > cond)
					return 1;
				return 0;
			}
			if(type[1] == '<'){
				if(curr_gear->a_num < cond)
					return 1;
				return 0;
			}
		case 's':
			if(type[1] == '>'){
				if(curr_gear->s_num > cond)
					return 1;
				return 0;
			}
			if(type[1] == '<'){
				if(curr_gear->s_num < cond)
					return 1;
				return 0;
			}
	}
	return -1;
}

//returns pointer to filter with name
//if no filter is found also creates it and calls it's self on each condition pointer
struct filter *add_filter(char *name)
{
	if(!strcmp(name, "A"))
		return accept;
	if(!strcmp(name, "R"))
		return reject;
	int find_index(char *);

	int ind_name = find_index(name);
	char *filter_line = NULL;
	char *pointer_loc = NULL;
	struct filter *fp = NULL;
	char temp_num_store[10];
	int arg_num = 0;
	int find_col;
	char *temp_int;
	char *temp_fit;

	if((*(filter_string_array + ind_name))->filter_pointer != NULL){
		return (*(filter_string_array + ind_name))->filter_pointer;
	}

	fp = (struct filter *) malloc(sizeof(struct filter));
	(*(filter_string_array + ind_name))->filter_pointer = fp;

	temp_int = strdup((*(filter_string_array + ind_name))->filter_string);

	filter_line = strtok_r(temp_int, " \n{},", &pointer_loc);

	fp->node_name = strdup(filter_line);

	while((filter_line = strtok_r(NULL, " \n{},", &pointer_loc)) != NULL){
		if(filter_line[1] != '<' && filter_line[1] != '>'){
			switch(arg_num){
				case 0:
					temp_fit = strdup(filter_line);
					fp->first_pointer = add_filter(temp_fit);
					free(temp_fit);
					fp->first_type[0] = 'n';
					fp->first_type[1] = 'n';
					fp->first_cond = 0;
					break;
				case 1:
					temp_fit = strdup(filter_line);
					fp->second_pointer = add_filter(temp_fit);
					free(temp_fit);
					fp->second_type[0] = 'n';
					fp->second_type[1] = 'n';
					fp->second_cond = 0;
					break;
				case 2:
					temp_fit = strdup(filter_line);
					fp->third_pointer = add_filter(temp_fit);
					free(temp_fit);
					fp->third_type[0] = 'n';
					fp->third_type[1] = 'n';
					fp->third_cond = 0;
					break;
				case 3:
					temp_fit = strdup(filter_line);
					fp->fourth_pointer = add_filter(temp_fit);
					free(temp_fit);
					fp->fourth_type[0] = 'n';
					fp->fourth_type[1] = 'n';
					fp->fourth_cond = 0;
					break;
			}
		}
		else{
			switch(arg_num){
				case 0:
					fp->first_type[0] = filter_line[0];
					fp->first_type[1] = filter_line[1];
					break;
				case 1:
					fp->second_type[0] = filter_line[0];
					fp->second_type[1] = filter_line[1];
					break;
				case 2:
					fp->third_type[0] = filter_line[0];
					fp->third_type[1] = filter_line[1];
					break;
				case 3:
					fp->fourth_type[0] = filter_line[0];
					fp->fourth_type[1] = filter_line[1];
					break;
			}

			for(int i = 2; filter_line[i] != ':'; ++i){
				temp_num_store[i - 2] = filter_line[i];
				temp_num_store[i - 1] = '\0';
			}

			switch(arg_num){
				case 0:
					fp->first_cond = atoi(temp_num_store);
					break;
				case 1:
					fp->second_cond = atoi(temp_num_store);
					break;
				case 2:
					fp->third_cond = atoi(temp_num_store);
					break;
				case 3:
					fp->fourth_cond = atoi(temp_num_store);
					break;
			}
			for(find_col = 0; filter_line[find_col] != ':'; ++find_col);
			switch(arg_num){
				case 0:
					fp->first_pointer = add_filter(filter_line + find_col + 1);
					break;
				case 1:
					fp->second_pointer = add_filter(filter_line + find_col + 1);
					break;
				case 2:
					fp->third_pointer = add_filter(filter_line + find_col + 1);
					break;
				case 3:
					fp->fourth_pointer = add_filter(filter_line + find_col + 1);
					break;
			}
		}
		++arg_num;
	}

	free(temp_int);

	return fp;
}

//prints tree node at the number of tabs and prints all nodes linked to this one
void print_tree(struct filter *input, int tab_num)
{

	void tab_print(int);

	if(input == accept){
		//tab_print(tab_num);
		printf("a\n");
		return;
	}
	if(input == reject){
		//tab_print(tab_num);
		printf("r\n");
		return;
	}
	if(input == NULL){
		return;
	}

	printf("%s{", input->node_name);

	if(input->first_pointer != NULL){
		if(input->first_type[0] == 'n'){
			printf("%s", input->first_pointer->node_name);
		}
		else{
			printf("%c%c%d:", input->first_type[0], input->first_type[1], input->first_cond);
			printf("%s", input->first_pointer->node_name);
		}
	}
	if(input->second_pointer != NULL){
		printf(",");
		if(input->second_type[0] == 'n'){
			printf("%s", input->second_pointer->node_name);
		}
		else{
			printf("%c%c%d:", input->second_type[0], input->second_type[1], input->second_cond);
			printf("%s", input->second_pointer->node_name);
		}
	}
	if(input->third_pointer != NULL){
		printf(",");
		if(input->third_type[0] == 'n'){
			printf("%s", input->third_pointer->node_name);
		}
		else{
			printf("%c%c%d:", input->third_type[0], input->third_type[1], input->third_cond);
			printf("%s", input->third_pointer->node_name);
		}
	}
	if(input->fourth_pointer != NULL){
		printf(",");
		if(input->fourth_type[0] == 'n'){
			printf("%s", input->fourth_pointer->node_name);
		}
		else{
			printf("%c%c%d:", input->fourth_type[0], input->fourth_type[1], input->fourth_cond);
			printf("%s", input->fourth_pointer->node_name);
		}
	}


	printf("}\n");

	if(input->first_pointer != NULL){
		tab_print(tab_num);
		printf("1 ");
		print_tree(input->first_pointer, tab_num + 1);
	}
	if(input->second_pointer != NULL){
		tab_print(tab_num);
		printf("2 ");
		print_tree(input->second_pointer, tab_num + 1);
	}
	if(input->third_pointer != NULL){
		tab_print(tab_num);
		printf("3 ");
		print_tree(input->third_pointer, tab_num + 1);
	}
	if(input->fourth_pointer != NULL){
		tab_print(tab_num);
		printf("4 ");
		print_tree(input->fourth_pointer, tab_num + 1);
	}
}

//prints the specified number of tabs
void tab_print(int tab_num)
{
	for(int i = 0; i < tab_num; ++i)
		printf("\t");
}

//finds index of string starting with name in filter_string_array
int find_index(char *name)
{
	int name_index;

	for(name_index = 0; name_index < filter_num; ++name_index){
		for(int i = 0; (*(filter_string_array + name_index))->filter_string[i] == name[i]; ++i){
			if(name[i + 1] == '\0' && (*(filter_string_array + name_index))->filter_string[i + 1] == '{'){
				return name_index;
			}
		}
	}
	return name_index;
}
