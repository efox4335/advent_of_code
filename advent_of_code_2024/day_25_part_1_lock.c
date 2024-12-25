/*
 * im just gonna do the n^2 solution
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	enum{LOCK, KEY, NONE};

	char *input_line = NULL;
	size_t lim = 0;

	int lock_arr[1000][5];
	int lock_count = 0;

	int key_arr[1000][5];
	int key_count = 0;

	int part = NONE;

	while(getline(&input_line, &lim, stdin) > 0){
		if(input_line[0] == '\n'){
			if(part == LOCK){
				++lock_count;
			}else{
				++key_count;
			}

			part = NONE;
			continue;
		}

		switch(part){
		case NONE:
			if(input_line[0] == '#'){
				for(int i = 0; i < 5; ++i){
					lock_arr[lock_count][i] = 0;
				}

				part = LOCK;
				goto lock_part;
			}else{
				for(int i = 0; i < 5; ++i){
					key_arr[key_count][i] = 0;
				}
				part = KEY;
				goto key_part;
			}
		case KEY:
		key_part:
			for(int i = 0; i < 5; ++i){
				if(input_line[i] == '#'){
					key_arr[key_count][i] += 1;
				}
			}

			break;
		case LOCK:
		lock_part:
			for(int i = 0; i < 5; ++i){
				if(input_line[i] == '#'){
					lock_arr[lock_count][i] += 1;
				}
			}

			break;
		}
	}

	if(part == LOCK){
		++lock_count;
	}else{
		++key_count;
	}

	int comp_count = 0;

	for(int i = 0; i < lock_count; ++i){
		for(int j = 0; j < key_count; ++j){
			int is_comp = 1;

			for(int k = 0; k < 5; ++k){
				if(lock_arr[i][k] + key_arr[j][k] >= 8){
					is_comp = 0;
					break;
				}
			}

			if(is_comp == 1){
				++comp_count;
			}
		}
	}

	printf("%d\n", comp_count);

	free(input_line);
	return 0;
}
