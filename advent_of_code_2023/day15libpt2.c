/*
take input
apply the box finding algorithm to find box
copy chars before - or = to a label name string
if - is found in string go to box number and search for string matching label name
	set lens_size to -1
	if no label is found do nothing
if = is found search for label name in box
	if no label is found place the label and and number after = in box after the last non -1 lense size slot
		if last slot if full apply an algorithm to squish array
	is label is found replace lens size in slot where label name is found with

once input is exhausted add all focusing power of lenses together

focasing power of lens = (box_num + 1) * (slot_of_lens - number_of_slots_with_-1_lens_before_it + 1) * (lens strength)
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct slot{
	char *label;
	int lens_size;
};//-1 lens_size means slot is emtpy

struct box{
	struct slot slot_array[1000];
	int slot_index;
};

struct box box_array[256];

int main(void)
{
	char *line = NULL;
	char *index_string;
	char label_name[100];
	size_t lim = 0;
	long int lens_strength = 0;
	long int total_focal_strength = 0;
	int type_loc;

	int find_box_num(char *);
	void add_label(int, char *, int);
	void remove_label(int, char *);

	for(int i = 0; i < 256; ++i)//sets the slot_index in box to be 0
		box_array[i].slot_index = 0;

	getline(&line, &lim, stdin);

	index_string = strtok(line, ",\n");

	do{
		for(int i = 0; isalpha(index_string[i]); ++i)
			type_loc = i + 1;

		strncpy(label_name, index_string, type_loc);
		label_name[type_loc] = '\0';

		if(index_string[type_loc] == '-')
			remove_label(find_box_num(label_name), label_name);
		else
			add_label(find_box_num(label_name), label_name, atoi(index_string + type_loc + 1));


	}while((index_string = strtok(NULL, ",\n")) != NULL);

	for(int i = 0; i < 256; ++i){
		int has_print = 0;
		int empty_slot = 0;
		for(int k = 0; k < box_array[i].slot_index; ++k){
			if(box_array[i].slot_array[k].lens_size != -1){
				printf("%s %d ", box_array[i].slot_array[k].label, box_array[i].slot_array[k].lens_size);
				has_print = 1;
				lens_strength = (i + 1) * (k + 1 - empty_slot) * box_array[i].slot_array[k].lens_size;
				total_focal_strength += lens_strength;
			}
			else
				++empty_slot;
		}
		if(has_print == 1)
			printf("\n");
	}


	printf("%ld\n", total_focal_strength);
	return 0;
}

//marks lens in box_num slot with label as lens_size
//if no slot is found add new slot at slot_index and ++slot_index
//if slot_index == 1000 squish slot array
void add_label(int box_num, char *label, int lens_size)
{
	int find_label(int, char *);

	int slot_index = find_label(box_num, label);

	if(slot_index != -1)
		box_array[box_num].slot_array[slot_index].lens_size = lens_size;
	else{
		box_array[box_num].slot_array[box_array[box_num].slot_index].lens_size = lens_size;
		box_array[box_num].slot_array[box_array[box_num].slot_index].label = strdup(label);
		box_array[box_num].slot_index += 1;
	}
}

//marks lens in box_num slot with label as -1
//if no slot is found do nothing
void remove_label(int box_num, char *label)
{
	int find_label(int, char *);

	int slot_index = find_label(box_num, label);

	if(slot_index != -1)
		box_array[box_num].slot_array[slot_index].lens_size = -1;
}

//gets box number
int find_box_num(char *str)
{
	int str_hash = 0;
	for(int i = 0; isalpha(str[i]); ++i){
		str_hash += str[i];
		str_hash *= 17;
		str_hash %= 256;
	}
	return str_hash;
}

//finds index of label in box box_num
int find_label(int box_num, char *label)
{
	for(int i = 0; i < box_array[box_num].slot_index; ++i){
		if(box_array[box_num].slot_array[i].lens_size != -1 && !strcmp(label, box_array[box_num].slot_array[i].label)){
			return i;
		}
	}
	return -1;
}
