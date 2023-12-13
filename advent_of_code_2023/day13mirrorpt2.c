/*
find valid row/columns
change each square int mirror_pattern to the opposite and check for valid row/columns without the original
*/
#include <stdio.h>
#include <string.h>

char mirror_pattern[100][100];
int patt_max_row = 0;

int main(void)
{
	char *line = NULL;
	size_t lim = 0;
	long int running_total = 0;

	long int find_patt(void);

	while(!feof(stdin)){
		if(getline(&line, &lim, stdin) > 1)
			strcpy(mirror_pattern[patt_max_row++], line);
		else{
			running_total += find_patt();
			printf("%d\n", patt_max_row);
			patt_max_row = 0;
		}
	}

	printf("%ld\n", running_total);
	return 0;
}

long int find_patt(void)
{
	long int total = 0;
	int temp_col = -1;
	int temp_row = -1;
	int temp = 0;

	int chk_column(int);
	int chk_row(int);

	if(patt_max_row == 0)
		return 0;//if no pattern is gotten

	temp_row = chk_row(-1);

	temp_col = chk_column(-1);

	for(int i = 0; i < patt_max_row; ++i){
		for(int j = 0; mirror_pattern[i][j] != '\n'; ++j){
			if(mirror_pattern[i][j] == '.')
				mirror_pattern[i][j] = '#';
			else
				mirror_pattern[i][j] = '.';
			if((temp = chk_row(temp_row)) != -1){
				total += (temp + 1) * 100;
				return total;
			}

			if((temp = chk_column(temp_col)) != -1){
				total += temp + 1;
				return total;
			}
			if(mirror_pattern[i][j] == '.')
				mirror_pattern[i][j] = '#';
			else
				mirror_pattern[i][j] = '.';
		}
	}

	for(int i = 0; i < patt_max_row; ++i)
		printf("%s", mirror_pattern[i]);
	return total;
}

/*
checks rows till duplicate row is found
checks rest of pattern for mirrors along that line
returns index of upper duplicate row if rest of pattern is mirrored
if not keep checking
if no pattern found return - 1
*/
int chk_row(int sk)
{
	int is_mirror = 0;
	for(int i = 1; i < patt_max_row; ++i){
		if(i == sk + 1)
			continue;
		if(strcmp(mirror_pattern[i - 1], mirror_pattern[i]) == 0){
			is_mirror = 1;
			for(int j = 0; i + j < patt_max_row && i - 1 - j >= 0; ++j){
				if(strcmp(mirror_pattern[i + j], mirror_pattern[i - j - 1])){
					is_mirror = 0;
					break;
				}
			}
		}
		if(is_mirror == 1)
			return i - 1;
	}
	return -1;
}

/*
checks columns till duplicate row is found
checks rest of pattern for mirrors along that line
returns index of left most duplicate column if rest of pattern is mirrored
if not keep checking
if no pattern found return - 1
*/
int chk_column(int sk)
{
	int is_mirror = 0;
	int temp_ind = 0;
	char temp_a[100];
	char temp_b[100];
	for(int i = 1; mirror_pattern[0][i] != '\n'; ++i){
		if(i == sk + 1)
			continue;

		temp_ind = 0;
		for(int j = 0; j < patt_max_row; ++j)
			temp_a[temp_ind++] = mirror_pattern[j][i - 1];
		temp_a[temp_ind] = '\0';
		temp_ind = 0;
		for(int j = 0; j < patt_max_row; ++j)
			temp_b[temp_ind++] = mirror_pattern[j][i];
		temp_b[temp_ind] = '\0';

		if(strcmp(temp_a, temp_b) == 0){
			is_mirror = 1;
			for(int k = 0; mirror_pattern[0][k + i] != '\n' && i - k - 1 >= 0; ++k){
				temp_ind = 0;
				for(int j = 0; j < patt_max_row; ++j)
				temp_a[temp_ind++] = mirror_pattern[j][i - k - 1];
				temp_a[temp_ind] = '\0';
				temp_ind = 0;
				for(int j = 0; j < patt_max_row; ++j)
				temp_b[temp_ind++] = mirror_pattern[j][i + k];
				temp_b[temp_ind] = '\0';

				if(strcmp(temp_a, temp_b)){
					is_mirror = 0;
					break;
				}
			}
		}
		if(is_mirror == 1)
			return i - 1;
	}
	return -1;
}
