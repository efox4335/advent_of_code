/*
input galaxy map as 2d array
on input put the numbers row on the galaxy array + offset
if extpy row is found + offset by 1000000
do the same for columns
get sums of all distances
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define EXPAND 1000000

struct galaxy{// stores the location of galaxy maps
	long int row;
	long int column;
};

struct galaxy galaxy_array[1000];

int galaxy_row_num = 0;
int galaxy_num = 0;

int main(void)
{
	char *line = NULL;
	size_t lim = 0;
	long int dist_sum = 0;

	char *temp_store = (char *) malloc(sizeof(char) * 1000 * 1000);//access rows by galaxy_row_num * 1000

	for(int i = 0; i < 1000*1000; ++i){
		*(temp_store + i) = '.';
	}

	long int row_offset = 0;

	//put galaxy map in storage and put the row data on the galaxy array
	while(getline(&line, &lim, stdin) > 1){
		int has_gal = 0;
		int i;

		for(i = 0; line[i] != '\0'; ++i){
			*(temp_store + (galaxy_row_num * 1000) + i) = line[i];
			if(line[i] == '#'){
				has_gal = 1;
				galaxy_array[galaxy_num++].row = galaxy_row_num + row_offset;
			}
		}
		*(temp_store + (galaxy_row_num * 1000) + i) = '\0';
		printf(" %3d %s", galaxy_row_num + 1, (temp_store + (1000 * galaxy_row_num)));
		++galaxy_row_num;

		if(has_gal == 0){//puts line with no galaxys under line with no galaxys
			row_offset += EXPAND - 1;
		}
	}

	printf("%d\n", galaxy_num);

	free(line);

	//put the column data on the galaxy array
	long int column_offset = 0;
	galaxy_num = 0;
	for(int i = 0; *(temp_store + i) != '\0'; ++i){
		int emtp_col = 0;
		for(int j = 0; j < galaxy_row_num; ++j){
//			printf("%c", *(temp_store + (j * 1000) + i));
			if(*(temp_store + (j * 1000) + i) == '#'){
				galaxy_array[galaxy_num++].column = i + column_offset;
				emtp_col = 1;
			}
		}
		//printf("\n");
		if(emtp_col == 0){
			column_offset += EXPAND - 1;

		}
	//	printf("%d %d %d\n", row_offset, emtpgalaxy_row_num_col, i);
	}

	printf("%d\n", galaxy_num);
	free(temp_store);

	//calcs distances

	for(int i = 0; i < galaxy_num; ++i){
		for(int j = i + 1; j < galaxy_num; ++j)
			dist_sum += labs(galaxy_array[i].row - galaxy_array[j].row) + labs(galaxy_array[i].column - galaxy_array[j].column);
	}

	printf("%ld\n", dist_sum);

	return 0;
}
