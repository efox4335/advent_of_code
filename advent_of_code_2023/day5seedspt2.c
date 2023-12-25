/*
!!impotant use longs for this one
import the list of seeds to an array
import the tables sorted
bruteforce
took around 7mins
could be more optimized
*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

struct table_line{
	long int input_start;//start of the input range
	long int output_start;//start of the output range
	long int range;
	struct table_line *next_line;
};

struct seed_range{//seed range is inclusive
	long int lower_bound;
	long int upper_bound;
	struct seed_range *next_range;
};

int main(void)
{
	struct table_line *get_table(void);
	int get_seeds(struct seed_range **);
	long int read_table(long int input_val, struct table_line *table_ptr);
	long int temp_out;//holds output inbetween data conversions

	struct table_line *table_array[10];
	struct seed_range *seed_list[100];//pointers to seed ranges
	int seed_index = 0;//index of last seed
	long int lowest_loc = -1;//holds lowest location

	seed_index = get_seeds(seed_list);// gets seed numbers

	for(int i = 0; i < sizeof(table_array)/ sizeof(struct table_line *); ++i)
		table_array[i] = NULL;//sets table to null to prevent accessing invalid memory

	for(int i = 0; i < 8; ++i)// gets table
		table_array[i] = get_table();

	for(int i = 0; i < 7; ++i){
		for(struct table_line *ptr = table_array[i]; ptr != NULL; ptr = ptr->next_line){
			printf("%ld ", ptr->output_start);
			printf("%ld ", ptr->input_start);
			printf("%ld ", ptr->range);
			printf("\n");
		}
		printf("\n\n");
	}

	for(int i = 0; i <=seed_index; ++i){
		printf("%ld %ld\n", seed_list[i]->lower_bound, seed_list[i]->upper_bound);
	}


	//printf("%ld\n", table_array[0]->input_start);

		//loop is seeds then tables
	for(int i = 0; i <= seed_index; ++i){//gets all seeds
		temp_out = seed_list[i];
		long int p = 0;
		for(long int k = seed_list[i]->lower_bound; k <= seed_list[i]->upper_bound; ++k){
			temp_out = k;
			for(int j = 0; j < 7; ++j){//7 is the number of tables
				temp_out = read_table(temp_out, table_array[j]);
			}

		//	printf("\n%ld\n", temp_out);

			if((k - p) > 10000000){
				p = k;
				printf("%ld\n", k);
			}
			if(lowest_loc == -1)
				lowest_loc = temp_out;
			if(temp_out < lowest_loc)
				lowest_loc = temp_out;
		}
		printf("hi\n");
	}

	printf("%ld\n", lowest_loc);

	return 0;
}

//upper bound of seed range = lower bound + range - 1
//gets seed ranges from stdin returns array index of last seed range
int get_seeds(struct seed_range **seed_array)
{
	char temp_int_store[100];
	int seed_index = 0;
	long int seed_start_range;
	long int seed_range_length;

	int c;

	enum seed_parts{
		START,
		RANGE,
		END
	}seed_part = START;

	for(int i = 0; (c = getchar()) != EOF;){
		if(isdigit(c))
			temp_int_store[i++] = c;

		if(!isdigit(c) && i != 0){
			switch(seed_part){
			case(START):
				temp_int_store[i] = '\0';
				seed_start_range = atol(temp_int_store);
				seed_part = RANGE;
				i = 0;
				break;
			case(RANGE):
				temp_int_store[i] = '\0';
				seed_range_length = atol(temp_int_store);
				seed_part = END;
				i = 0;
				break;
			default:
				break;
			}
		}
		if(seed_part == END){
			seed_array[seed_index] = (struct seed_range *) malloc(sizeof(struct seed_range));
			seed_array[seed_index]->lower_bound = seed_start_range;
			seed_array[seed_index]->upper_bound = seed_start_range + seed_range_length - 1;
			seed_array[seed_index]->next_range = NULL;
			++seed_index;
			seed_part = START;
		}
		if(c == '\n')//so it gets the last number
			break;
	}
	return seed_index - 1;
}

//goes to first didgit in stdin gets table intil first letter or EOF is found
//returns pointer to first element of sorted table linked list
struct table_line *get_table(void)
{
	int number_of_lines = 1;
	int firstrun = 0;
	int c;
	while((c = getchar()) != ':' && c != EOF);//skips to the digits

	if(c == EOF)
		return NULL;
	enum parts{//the parts of each line on the table
		OUTPUT_ST,
		INPUT_ST,
		RANGE,
		END
	};
	enum parts part;
	part = OUTPUT_ST;
	char temp_num_store[100];
	struct table_line *firstptr;//stores first element for later storage
	struct table_line *table_ptr;

	long int temp_inp;
	long int temp_out;
	long int temp_range;

	firstptr = (struct table_line *) malloc(sizeof(struct table_line));//creats space for the first element

	table_ptr = firstptr;
	table_ptr->next_line = NULL;

	for(int i = 0; (c = getchar()) != EOF && !isalpha(c);){// store in temp values so as to only create structs for valid data
		if(isdigit(c))
			temp_num_store[i++] = c;

		if(!isdigit(c) && i != 0){
			switch(part){
			case (OUTPUT_ST):
				temp_num_store[i] = '\0';
				temp_out = atol(temp_num_store);
				part = INPUT_ST;
				break;

			case (INPUT_ST):
				temp_num_store[i] = '\0';
				temp_inp = atol(temp_num_store);
				part = RANGE;
				break;
			case (RANGE):
				temp_num_store[i] = '\0';
				temp_range = atol(temp_num_store);
				part = END;
				break;
			default:
				break;
			}
			i = 0;
		}
		if(part == END && c == '\n'){//check part to make sure not wasting space on empty lines
			if(firstrun){// only mallocs after the first run to not waste memory
				++number_of_lines;
				table_ptr->next_line = (struct table_line *) malloc(sizeof(struct table_line));//makes space for next element
				table_ptr = table_ptr->next_line;
				table_ptr->next_line = NULL;
			}
			firstrun = 1;
			part = OUTPUT_ST;
			table_ptr->output_start = temp_out;
			table_ptr->input_start = temp_inp;
			table_ptr->range = temp_range;
		}
	}

	struct table_line **temp_arr = (struct table_line **) malloc(number_of_lines * sizeof(struct table_line *));
	int table_sort_cmp(const void *, const void *);

	table_ptr = firstptr;

	for(int i = 0; i < number_of_lines; ++i, table_ptr = table_ptr->next_line)//puts linked list on array
		temp_arr[i] = table_ptr;

	qsort(temp_arr, number_of_lines, sizeof(struct table_line *), table_sort_cmp);//sorts array

	firstptr = temp_arr[0];

	for(int i = 0; i < number_of_lines - 1; ++i)
		temp_arr[i]->next_line = temp_arr[i + 1];

	temp_arr[number_of_lines - 1]->next_line = NULL;

	free(temp_arr);

	return firstptr;
}

/*
compare value of input_start in struct table_line for qsort
a and b cast at pointer to pointer to struct and dereferenced once with * to pointer to struct
element is accessed by ->
if else because qsort only accepts values 1, 0, -1 i've since learned this isn't true i'm not fixing it
*/
int table_sort_cmp(const void *a, const void *b)
{
	long int i;
	if((i = ((*((struct table_line **)b))->input_start - (*((struct table_line **)a))->input_start)) > 0)
		return -1;
	else if(i < 0)
		return 1;
	else
		return 0;
}

long int read_table(long int input_val, struct table_line *table_ptr)
{
	while(table_ptr != NULL && table_ptr->input_start - 1 < input_val){
		if(input_val >= table_ptr->input_start && input_val < table_ptr->input_start + table_ptr->range){
			return table_ptr->output_start + (input_val - table_ptr->input_start);
		}
		table_ptr = table_ptr->next_line;
	}
	return input_val;
}
