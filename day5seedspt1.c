/*
!!impotant use longs for this one
import the list of seeds to an array
import the tables
create structs with pointers to structs that contain the table

create func to convert any input to it's output on the table
save the closest seed
*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

struct table_line{
	long int input_start;
	long int output_start;
	long int range;
	struct table_line *next_line;
};

enum table_types{
	SEED_SOIL = 0,
	SOIL_FERT,
	FERT_WATER,
	WATER_LIGHT,
	LIGHT_TEMP,
	TEMP_HUMID,
	HUMID_LOC
};

int main(void)
{
	long int read_table(long int, struct table_line *);
	struct table_line *get_table(void);
	int get_seeds(long int *);

	struct table_line *table_array[10];
	long int seed_list[100];
	int seed_index = 0;//index of last seed
	long int temp_out;//holds output inbetween data conversions
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

	//loop is seeds then tables
	for(int i = 0; i <= seed_index; ++i){//gets all seeds
		temp_out = seed_list[i];
		for(int j = 0; j < 7; ++j){//7 is the number of tables
			temp_out = read_table(temp_out, table_array[j]);
		}
		printf("\n%ld\n", temp_out);
		if(lowest_loc == -1)
			lowest_loc = temp_out;
		if(temp_out < lowest_loc)
			lowest_loc = temp_out;
	}

	printf("%ld\n", lowest_loc);

	return 0;
}

//gets seed names from stdin returns array index of last seed
int get_seeds(long int seeds[])
{
	char temp_int_store[100];
	int seed_index = 0;

	int c;

	for(int i = 0; (c = getchar()) != EOF;){
		if(isdigit(c))
			temp_int_store[i++] = c;

		if(!isdigit(c) && i != 0){
			temp_int_store[i] = '\0';
			seeds[seed_index++] = (long int) atol(temp_int_store);
			i = 0;
		}
		if(c == '\n')//so it gets the last number
			break;
	}
	return seed_index - 1;
}

//goes to first didgit in stdin gets table intil first letter or EOF is found
//returns pointer to first element of table linked list
struct table_line *get_table(void)
{
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
	return firstptr;
}

/*
walk linked list finding if inputs are in rang if not return input
if in range output = output_start + input - input_start
returns the input value
*/
long int read_table(long int input_val, struct table_line *table_ptr)
{
	while(table_ptr != NULL){
		if(input_val >= table_ptr->input_start && input_val < table_ptr->input_start + table_ptr->range){
			return table_ptr->output_start + (input_val - table_ptr->input_start);
		}
		table_ptr = table_ptr->next_line;
	}
	return input_val;
}
