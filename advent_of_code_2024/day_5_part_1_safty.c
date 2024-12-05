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

//returns one if update follows rule for num
int check_compliance(rule *cur_rule, int *past_visited, int update_count)
{
	for(int i = 0; i < cur_rule->rule_count; ++i){
		if(past_visited[cur_rule->rules[i]] == update_count){
			return 0;
		}
	}

	return 1;
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

	char update_delim[] = ",";
	int past_update_val_arr[100];
	int update_count = 0;//if we use this to set the val in past_update_val_arr there is no need to 0 arr between updates

	for(int i = 0; i < 100; ++i){
		past_update_val_arr[i] = 0;
	}

	int mid_page_count = 0;

	while(getline(&input_line, &lim, stdin) > 0){
		if(input_line[0] == '\n'){
			input_part = UPDATES;
			continue;//skips past newline
		}

		if(input_part == RULES){
			num_ptr = strtok(input_line, rule_delim);
			right_rule = atoi(num_ptr);

			num_ptr = strtok(NULL, rule_delim);
			rule_append(&rule_arr[right_rule], atoi(num_ptr));
		}else{
			int update_len = 0;
			int update[100];
			num_ptr = strtok(input_line, update_delim);
			++update_count;

			//because each page only appears once in each
			while(num_ptr != NULL){
				int cur_num = atoi(num_ptr);
				if(!check_compliance(&rule_arr[cur_num], past_update_val_arr, update_count)){
					break;
				}
				past_update_val_arr[cur_num] = update_count;
				update[update_len] = cur_num;

				++update_len;
				num_ptr = strtok(NULL, update_delim);
			}

			//will be non null if check_compliance failed
			if(num_ptr == NULL){
				mid_page_count += update[((update_len - 1) / 2)];
			}
		}
	}

	printf("%d\n", mid_page_count);

	free(input_line);
	return 0;
}
