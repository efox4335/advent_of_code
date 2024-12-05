/*
 * input rules into an array where the left value is an index in an array that contains all the right values
 * when inputing the updates mark all numbers previously visited in an array by index and compare it with the
 * right values in the rule array
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	int rule_count;
	int rules[100];
}rule;

enum{RULE_NUM = 100};//no rule left value > 99

void reset_rules(rule *rule_arr)
{
	for(int i = 0; i < RULE_NUM; ++i){
		rule_arr[i].rule_count = 0;
	}
}

//appends rule to list of rules
void rule_append(rule *rule_ptr, int rule_num)
{
	rule_ptr->rules[rule_ptr->rule_count] = rule_num;
	++(rule_ptr->rule_count);
}

int main(void)
{
	enum{RULES, UPDATES};

	char *input_line = NULL;
	size_t lim = 0;

	rule rule_arr[RULE_NUM];
	reset_rules(rule_arr);

	char rule_delim[] = "|";
	char *num_ptr = NULL;
	int right_rule = 0;

	int input_part = RULES;

	while(getline(&input_line, &lim, stdin) > 0){
		if(input_line[0] == '\n'){
			input_part = UPDATES;
		}

		if(input_part == RULES){
			num_ptr = strtok(input_line, rule_delim);
			right_rule = atoi(num_ptr);

			num_ptr = strtok(NULL, rule_delim);
			rule_append(&rule_arr[right_rule], atoi(num_ptr));
		}
	}

	free(input_line);
	return 0;
}
