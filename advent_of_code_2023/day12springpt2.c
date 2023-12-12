/*
for each block of bad springs start at the beginning of the list of input + the offset of bad springs before it
compute the list of all posiable positions it can be in and do this for each of the blocks after this
cashe the result for each block and offset return the cashed result if needed
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct spr_data{
	int length;
	int offset;
	//offset starts at broken_spr_arr[i].offset = broken_spr_arr[i - 1].offset + 1 +  broken_spr_arr[i - 1].length
	//first offset = 0
	int max_offset;
	//broken_spr_arr[i].max_offset = broken_spr_arr[i + 1].max_offset - 1 - broken_spr_arr[i].length
	//last max_offset = length of input - length + 1
};

struct spr_data broken_spr_arr[1000];//stores data about broken spring blocks
int broken_spr_num = 0;//stores the number of broken spring blocks for a line
char curpt_data[1000];
long int cashe[1000][1000];//accessed by st_index and broken_spr_arr[st_index].offset

/*
get input data
set starting offset
set max offset
add result of find_valid configs to total_valid_configs
*/
int main(void)
{
	char *line = NULL;
	char *brok_len = NULL;
	size_t lim = 0;
	long int total_valid_configs = 0;
	char *temp;
	char *use;

	long int find_valid_configs(int);
	void set_offset(int);

	while(getline(&line, &lim, stdin) > 1){
		for(int i = 0; i < 1000; ++i)
			for(int j = 0; j < 1000; ++j)
				cashe[i][j] = -1;

		temp = strdup(line);
		brok_len = strtok(line, " ,\n");
		strcpy(curpt_data, brok_len);
		strcat(curpt_data, "?");
		strcat(curpt_data, brok_len);
		strcat(curpt_data, "?");
		strcat(curpt_data, brok_len);
		strcat(curpt_data, "?");
		strcat(curpt_data, brok_len);
		strcat(curpt_data, "?");
		strcat(curpt_data, brok_len);

		for(brok_len = strtok(NULL, " ,\n"); brok_len != NULL; brok_len = strtok(NULL, " ,\n")){
			broken_spr_arr[broken_spr_num++].length = atoi(brok_len);//gets lengths of broken gear blocks
		}

		for(int j = 0; j < 4; ++j){
			use = strdup(temp);
			brok_len = strtok(use, " ,\n");

			for(brok_len = strtok(NULL, " ,\n"); brok_len != NULL; brok_len = strtok(NULL, " ,\n")){

					broken_spr_arr[broken_spr_num++].length = atoi(brok_len);//gets lengths of broken gear blocks
			}
			free(use);
		}

		free(temp);

		broken_spr_arr[0].offset = 0;
		set_offset(0);//sets starting offset

		for(int i = 0; curpt_data[i] != '\0'; ++i)//gets max offset for broken_spr_num - 1
			broken_spr_arr[broken_spr_num - 1].max_offset = i - broken_spr_arr[broken_spr_num - 1].length + 1;

		for(int i = broken_spr_num - 2; i >= 0; --i)//gets max offset
			broken_spr_arr[i].max_offset = broken_spr_arr[i + 1].max_offset - 1 - broken_spr_arr[i].length;

		total_valid_configs += find_valid_configs(0);//get valid configs for line

		printf("bad data: %s\n", curpt_data);

		printf("broken spring blocks: ");
		for(int i = 0; i < broken_spr_num; ++i)
			printf("%d ", broken_spr_arr[i].length);


		printf("\n");

		printf("broken spring offsets: ");
		for(int i = 0; i < broken_spr_num; ++i)
			printf("%d ", broken_spr_arr[i].offset);

		printf("\n");

		printf("broken spring max offsets: ");
		for(int i = 0; i < broken_spr_num; ++i)
			printf("%d ", broken_spr_arr[i].max_offset);

		broken_spr_num = 0;
		printf("\n%ld", total_valid_configs);
		printf("\n\n");
		//return 0;
	}

	printf("%ld\n", total_valid_configs);
	return 0;
}

/*
if(broken_spr_num - st_index > 1)
	if curpt_data[broken_spr_arr[st_index].offset - 1] == '#'
		return valid_track

	if broken_spr_arr[broken_spr_num - 1].offset > broken_spr_arr[broken_spr_num - 1].max_offset
		return valid_track

	if curpt_data[broken_spr_arr[st_index].offset + broken_spr_arr[st_index].length] == '#'
		1 to offset from broken_spr_arr[st_index] to broken_spr_arr[broken_spr_num - 1] and re check loop

	if curpt_data[broken_spr_arr[st_index].offset] to curpt_data[broken_spr_arr[st_index].offset broken_spr_arr[st_index].length - 1] == '?'
		if available valid_track += find_valid_configs(st_index + 1)
		then add 1 to offset from broken_spr_arr[st_index] to broken_spr_arr[broken_spr_num - 1] and re check loop

		if not add 1 to offset from broken_spr_arr[st_index] to broken_spr_arr[broken_spr_num - 1] and re check loop
else
	if curpt_data[broken_spr_arr[st_index].offset - 1] == '#'
		return valid_track

	if broken_spr_arr[st_index].offset > broken_spr_arr[st_index].max_offset
		return valid_track

	if anywhere after curpt_data[broken_spr_arr[st_index].offset + broken_spr_arr[st_index].length] == '#'
		1 to offset from broken_spr_arr[st_index] and re check loop

	if curpt_data[broken_spr_arr[st_index].offset] to curpt_data[broken_spr_arr[st_index].offset broken_spr_arr[st_index].length - 1] == '?'
		if available ++valid_track
		if not add 1 to offset from broken_spr_arr[st_index] to broken_spr_arr[broken_spr_num - 1] and re check loop

finds valid configs comparing generated string
*/
long int find_valid_configs(int st_index)
{
	long int valid_track = 0;
	int valid_place = 0;
	int temp_ind_store = 0;
	long int temp_store = 0;
	static long int is_pross = 0;
	is_pross = 0;

	void set_offset(int);
	void deprint(void);

//	return 0;

	//if(cashe[st_index][broken_spr_arr[st_index].offset] != -1){
	//		printf("hi\n");
	//		return cashe[st_index][broken_spr_arr[st_index].offset];
	//	}

	while(broken_spr_num - st_index > 1){
//		printf("start new %d %d\n", st_index, broken_spr_arr[st_index].offset);
		//checks if # is before current place
		if(broken_spr_arr[st_index].offset > 0 && curpt_data[broken_spr_arr[st_index].offset - 1] == '#'){
//			printf("# before\n");
		//	cashe[st_index][broken_spr_arr[st_index].offset] = valid_track;
		//	printf("in: %d %d\n", st_index, broken_spr_arr[st_index].offset);
			return valid_track;
			is_pross = 0;
		}

		//checks if past max offset
		if(broken_spr_arr[broken_spr_num - 1].offset > broken_spr_arr[broken_spr_num - 1].max_offset){
	//		printf("offset to big\n");
		//	cashe[st_index][broken_spr_arr[st_index].offset] = valid_track;
		//	printf("in: %d %d\n", st_index, broken_spr_arr[st_index].offset);

			return valid_track;
			is_pross = 0;
		}

		//checks if # is right after place
		if(curpt_data[broken_spr_arr[st_index].offset + broken_spr_arr[st_index].length] == '#'){
//			printf("# after\n");
			broken_spr_arr[st_index].offset += 1;
			set_offset(st_index);
			continue;
		}

		//checks if place is valid
		for(int i = broken_spr_arr[st_index].offset; i < broken_spr_arr[st_index].offset + broken_spr_arr[st_index].length; ++i){
//			printf("%c %d", curpt_data[i], i);
			if(curpt_data[i] == '.'){
//				printf("\n");
				valid_place = 1;
				break;
			}
		}
		if(valid_place == 1){
			broken_spr_arr[st_index].offset += 1;
			set_offset(st_index);
			valid_place = 0;
			continue;
		}

//		printf("recur\n");
//		printf("%d\n", cashe[st_index + 1][broken_spr_arr[st_index + 1].offset]);
//		printf("out: %d %d\n", st_index, broken_spr_arr[st_index].offset);
		if(cashe[st_index + 1][broken_spr_arr[st_index + 1].offset] != -1){
			valid_track += cashe[st_index + 1][broken_spr_arr[st_index + 1].offset];
		}
		else{
			temp_ind_store =  broken_spr_arr[st_index + 1].offset;
			temp_store = find_valid_configs(st_index + 1);

			cashe[st_index + 1][temp_ind_store] = temp_store;

			valid_track += temp_store;
		}
		broken_spr_arr[st_index].offset += 1;
		set_offset(st_index);
	}

	if(is_pross > 0){
	//	deprint();
	//	printf("e:%ld\n", is_pross);
	//	return is_pross--;
	}

	while(1){
//		printf("hi5\n");
		//checks if # is before current place
		if(broken_spr_arr[st_index].offset > 0 && curpt_data[broken_spr_arr[st_index].offset - 1] == '#'){
//			printf("hi9\n");
			break;
		}

		//checks if past max offset
		if(broken_spr_arr[broken_spr_num - 1].offset > broken_spr_arr[broken_spr_num - 1].max_offset){
//			printf("hi10\n");
			break;
		}

		//checks if # is right after place
		if(valid_place == 0){
			for(int i = broken_spr_arr[st_index].offset + broken_spr_arr[st_index].length; curpt_data[i] != '\0'; ++i){
//				printf("hi11\n");
				if(curpt_data[i] == '#'){
					is_pross = 0;
					valid_place = 1;
					break;
				}
			}
			if(valid_place == 0)
				valid_place = 3;
		}

		//checks if place is valid
		for(int i = broken_spr_arr[st_index].offset; i < broken_spr_arr[st_index].offset + broken_spr_arr[st_index].length; ++i){
			if(curpt_data[i] == '.'){
				valid_place = 1;
				break;
			}
		}
		if(valid_place == 1){
//			printf("hi12\n");
			broken_spr_arr[st_index].offset += 1;
			valid_place = 0;
			continue;
		}

		++valid_track;
	//	if(valid_track > 10){
	//	deprint();
	//	}
	//	printf("%ld\n", valid_track);
		broken_spr_arr[st_index].offset += 1;
	}

	if(valid_place == 3){
		is_pross = valid_place - 1;
	}
	//deprint();
//	printf("%ld %ld\n", valid_track, is_pross);

	return valid_track;
}

//sets offset of all bad blocks based on the block at the array index passed in argumen
void set_offset(int index)
{
	for(int i = index + 1; i < broken_spr_num; ++i)//sets offset
		broken_spr_arr[i].offset = broken_spr_arr[i - 1].offset + 1 +  broken_spr_arr[i - 1].length;
}

void deprint(void){
	char local[100];
	char hash[100];
	for(int i = 0; curpt_data[i] != '\0'; ++i){
		local[i] = curpt_data[i];
		hash[i] = '#';
	}

	for(int i = 0; i < broken_spr_num; ++i)
		strncpy(&local[broken_spr_arr[i].offset], hash, broken_spr_arr[i].length);

	local[broken_spr_arr[broken_spr_num - 1].max_offset + broken_spr_arr[broken_spr_num - 1].length] = '\0';
	printf("%s\n", local);
}
