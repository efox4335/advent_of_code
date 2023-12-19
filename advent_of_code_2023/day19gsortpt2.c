//get input of filters as strings
//put filters in tree structure with the conditions and pointers to each filter that could be next
//have 2 nodes for accepting or rejecting gears
//store what value it's compared against and what operation as 2 chars
//	for example a<2006 would be type[0] = a, type[1] = < and cond = 2006
//	for no test type[0] = n
//	for accept node first_type[0] = a
//	for reject node first_type[0] = r
//get product of the the ranges of each gear value that could get an a
//get the sum of these values
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

struct gear_ranges{
	long int x_max;
	long int x_min;
	long int m_max;
	long int m_min;
	long int a_max;
	long int a_min;
	long int s_max;
	long int s_min;
	long int prod;
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
int filter_num = 0;
struct gear_ranges accepted_gear_arr[1000];
int accepted_gear_num = 0;

int main(void)
{
	int find_index(char *);
	struct filter *add_filter(char *);
	void print_tree(struct filter *, int);
	void get_acc_gear_ranges(struct filter *, struct gear_ranges);
	void get_gear_prod(struct gear_ranges *);

	char *line = NULL;
	size_t lim = 0;
	struct gear_ranges st_range;
	long int total_gear_ranges = 0;

	st_range.x_min = 1;
	st_range.x_max = 4000;
	st_range.m_min = 1;
	st_range.m_max = 4000;
	st_range.a_min = 1;
	st_range.a_max = 4000;
	st_range.s_min = 1;
	st_range.s_max = 4000;

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

	get_acc_gear_ranges(in_filter, st_range);

	for(int i = 0; i < accepted_gear_num; ++i){
		printf("x %ld %ld m %ld %ld a %ld %ld s %ld %ld\n", accepted_gear_arr[i].x_min, accepted_gear_arr[i].x_max,
		accepted_gear_arr[i].m_min, accepted_gear_arr[i].m_max, accepted_gear_arr[i].a_min, accepted_gear_arr[i].a_max,
		accepted_gear_arr[i].s_min, accepted_gear_arr[i].s_max);
		get_gear_prod(&accepted_gear_arr[i]);
	}

	for(int i = 0; i < accepted_gear_num; ++i)
		total_gear_ranges += accepted_gear_arr[i].prod;

	printf("%d\n", accepted_gear_num);
	printf("%ld\n", total_gear_ranges);

	return 0;
}

//gets the products of all gear ranges
void get_gear_prod(struct gear_ranges *gp)
{
	long int x_range;
	long int m_range;
	long int a_range;
	long int s_range;

	x_range = gp->x_max - gp->x_min + 1;
	m_range = gp->m_max - gp->m_min + 1;
	a_range = gp->a_max - gp->a_min + 1;
	s_range = gp->s_max - gp->s_min + 1;

	if(x_range < 0 || m_range < 0 || a_range < 0 || s_range < 0)
		return;

	gp->prod = x_range * m_range * a_range * s_range;
}

//gets the ranges of gears that end in node accept from input_gear_range and puts them on accepted_gear_arr
void get_acc_gear_ranges(struct filter *input_filter, struct gear_ranges input_gear_range)
{
	void copy_ranges(struct gear_ranges *, struct gear_ranges *);
	int get_range(struct filter *fp, struct gear_ranges *gp, int arg_count);

	struct gear_ranges output_1;
	struct gear_ranges output_2;
	struct gear_ranges output_3;
	struct gear_ranges output_4;

	if(input_filter == reject)
		return;

	if(input_filter == accept){
		copy_ranges(&accepted_gear_arr[accepted_gear_num], &input_gear_range);
		++accepted_gear_num;
		return;
	}

	copy_ranges(&output_1, &input_gear_range);
	copy_ranges(&output_2, &input_gear_range);
	copy_ranges(&output_3, &input_gear_range);
	copy_ranges(&output_4, &input_gear_range);

	if(get_range(input_filter, &output_1, 1) == 1){
		get_acc_gear_ranges(input_filter->first_pointer, output_1);
	}

	if(get_range(input_filter, &output_2, 2) == 2){
		get_acc_gear_ranges(input_filter->second_pointer, output_2);
	}

	if(get_range(input_filter, &output_3, 3) == 3){
		get_acc_gear_ranges(input_filter->third_pointer, output_3);
	}

	if(get_range(input_filter, &output_4, 4) == 4){
		get_acc_gear_ranges(input_filter->fourth_pointer, output_4);
	}
}

//modifies gp to be the ranges that would go to the argument arg_count
//ex. for arg_count 1 in{x>20:a,r} would modify x_min = 21 and arg_count 2 would make x_max = 20
//returns number of args prossessed
int get_range(struct filter *fp, struct gear_ranges *gp, int arg_count)
{
	int cond;
	char *type;
	if(fp == NULL)
		return 0;

	for(int i = 0; i < arg_count; ++i){
		switch(i){
			case 0:
				cond = fp->first_cond;
				type = fp->first_type;
				break;
			case 1:
				cond = fp->second_cond;
				type = fp->second_type;
				break;
			case 2:
				cond = fp->third_cond;
				type = fp->third_type;
				break;
			case 3:
				cond = fp->fourth_cond;
				type = fp->fourth_type;
				break;
		}
		switch(type[0]){
			case 'n':
				return i + 1;
			case 'x':
				if(type[1] == '>'){
					if(i + 1 == arg_count){
						gp->x_min = cond + 1;
					}
					else{
						gp->x_max = cond;
					}
				}
				else{
					if(i + 1 == arg_count){
						gp->x_max = cond - 1;
					}
					else{
						gp->x_min = cond;
					}
				}
				break;
			case 'm':
				if(type[1] == '>'){
					if(i + 1 == arg_count){
						gp->m_min = cond + 1;
					}
					else{
						gp->m_max = cond;
					}
				}
				else{
					if(i + 1 == arg_count){
						gp->m_max = cond - 1;
					}
					else{
						gp->m_min = cond;
					}
				}
				break;
			case 'a':
				if(type[1] == '>'){
					if(i + 1 == arg_count){
						gp->a_min = cond + 1;
					}
					else{
						gp->a_max = cond;
					}
				}
				else{
					if(i + 1 == arg_count){
						gp->a_max = cond - 1;
					}
					else{
						gp->a_min = cond;
					}
				}
				break;
			case 's':
				if(type[1] == '>'){
					if(i + 1 == arg_count){
						gp->s_min = cond + 1;
					}
					else{
						gp->s_max = cond;
					}
				}
				else{
					if(i + 1 == arg_count){
						gp->s_max = cond - 1;
					}
					else{
						gp->s_min = cond;
					}
				}
				break;
		}
	}

	return arg_count;
}

//copys gear range 2 to gear range 1
void copy_ranges(struct gear_ranges *gear_range_1, struct gear_ranges *gear_range_2)
{
	gear_range_1->a_max = gear_range_2->a_max;
	gear_range_1->a_min = gear_range_2->a_min;

	gear_range_1->m_max = gear_range_2->m_max;
	gear_range_1->m_min = gear_range_2->m_min;

	gear_range_1->s_max = gear_range_2->s_max;
	gear_range_1->s_min = gear_range_2->s_min;

	gear_range_1->x_max = gear_range_2->x_max;
	gear_range_1->x_min = gear_range_2->x_min;

	gear_range_1->prod = gear_range_2->prod;
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
