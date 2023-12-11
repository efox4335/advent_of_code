/*
input galaxy map as 2d array
on input expand empty rows on a temporary array
then copy it over to a permanent array by columns adding in an empty column for each
put the cords of all galaxys in an array
calculate distance by |galaxy1.row - galaxy2.row| + |galaxy1.column - galaxy.column|
get sums of all distances
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct galaxy{// stores the location of galaxy maps
	int row;
	int column;
};

char galaxy_map[1000][1000];
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

	//put galaxy map in temp storage expinding thr rows when needed
	while(getline(&line, &lim, stdin) > 1){
		int has_gal = 0;
		int i;

		for(i = 0; line[i] != '\0'; ++i){
			*(temp_store + (galaxy_row_num * 1000) + i) = line[i];
			if(line[i] == '#')
				has_gal = 1;
		}
		*(temp_store + (galaxy_row_num * 1000) + i) = '\0';
		printf(" %3d %s", galaxy_row_num + 1, (temp_store + (1000 * galaxy_row_num)));
		++galaxy_row_num;

		if(has_gal == 0){//puts line with no galaxys under line with no galaxys
			for(i = 0; line[i] != '\0'; ++i)
				*(temp_store + (galaxy_row_num * 1000) + i) = '.';
			*(temp_store + (galaxy_row_num * 1000) + i - 1) = '\n';
			*(temp_store + (galaxy_row_num * 1000) + i) = '\0';
			printf(" %3d %s", galaxy_row_num + 1, (temp_store + (1000 * galaxy_row_num)));
			++galaxy_row_num;
		}
	}

	free(line);

	//copies temp map to the permanent array
	int row_offset = 0;
	for(int i = 0; *(temp_store + i) != '\0'; ++i){
		int emtp_col = 0;
		for(int j = 0; j < galaxy_row_num; ++j){
//			printf("%c", *(temp_store + (j * 1000) + i));
			if(*(temp_store + (j * 1000) + i) == '\n')
				galaxy_map[j][i + row_offset] = '\0';
			else
				galaxy_map[j][i + row_offset] = *(temp_store + (j * 1000) + i);
			if(*(temp_store + (j * 1000) + i) == '#')
				emtp_col = 1;
		}
		//printf("\n");
		if(emtp_col == 0){
			++row_offset;
			for(int j = 0; j < galaxy_row_num; ++j)
				galaxy_map[j][i + row_offset] = '.';
		}
	//	printf("%d %d %d\n", row_offset, emtpgalaxy_row_num_col, i);
	}

	free(temp_store);

	//put galaxy cords on array
	for(int i = 0; i < galaxy_row_num; ++i)
		for(int j = 0; galaxy_map[i][j] != '\0'; ++j)
			if(galaxy_map[i][j] == '#'){
				galaxy_array[galaxy_num].row = i;
				galaxy_array[galaxy_num++].column = j;
			}

	for(int i = 0; i < galaxy_num; ++i){
		printf("%c %d %d\n", galaxy_map[galaxy_array[i].row][galaxy_array[i].column], galaxy_array[i].column, galaxy_array[i].row);
		for(int j = i + 1; j < galaxy_num; ++j)
			dist_sum += abs(galaxy_array[i].row - galaxy_array[j].row) + abs(galaxy_array[i].column - galaxy_array[j].column);
	}

	printf("%ld\n", dist_sum);

	return 0;
}
