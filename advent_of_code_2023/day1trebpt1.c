/*
get line walk left and right to get first digit
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(void){
	char line[1000];
	int line_index = 0;
	int numbers[10000];
	char pot_number[10] = {"\0"};
	int number_index = 0;
	int total_value = 0;
	int c;
	int i;

	while((c = getchar()) != EOF){
		line[line_index++] = c;
		if(c == '\n'){
			line[line_index] = '\0';
			pot_number[0] = '\0';
			for(i = 0; i != line_index; ++i){
				if(isdigit(line[i])){
					pot_number[0] = line[i];
					break;
				}
			}
			for(i = line_index; i != -1; --i){
				if(isdigit(line[i])){
					pot_number[1] = line[i];
					break;
				}
			}
			pot_number[2] = '\0';
			if(pot_number[0] != '\0')
				numbers[number_index++] = atoi(pot_number);
			line_index = 0;
		}
	}

	for(i = 0; i < number_index; i++)
		total_value += numbers[i];

	printf("%d\n", total_value);

	return 0;
}
