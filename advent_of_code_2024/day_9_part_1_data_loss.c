/*
 * when taking in data keep track of the max id
 * have a 2 index sweep of the file system one from the start and one from the end
 * when the start one encounters a file block it's id * index shall be added to a total
 * when the start one encounters an empty space block the index at the end shall find the first file block
 * when the file block is found it will simulate a move to fill the empty space and add the id * index to the total
 * if the file block at the end is empty and there is more empty space the index at the end shall find the next file block
 * if the empty block is filled before the file block is emptyed then the start index will continue
*/
#include <stdio.h>
#include <stdlib.h>

int final_id(int block_count)
{
	block_count = (!(block_count & 1))? block_count - 1: block_count;//block_count works for odd numbers if even convert to odd
	return block_count / 2;
}

int main(void)
{
	int block_arr[20000];
	int block_count = 0;

	char *input_line = NULL;
	size_t lim = 0;

	while(getline(&input_line, &lim, stdin) > 1){
	}

	for(int i = 0; input_line[i] != '\n'; ++i){
		block_arr[i] = input_line[i] - '0';
		++block_count;
	}

	size_t cur_fs_pos = 0;
	size_t st_index = 0;
	size_t end_index = (block_count & 1)? block_count - 1: block_count - 2;//skips last empty block if there is one
	size_t st_id = 0;
	size_t end_id = final_id(block_count);
	size_t checksum = 0;

	while(st_index <= end_index){
		if(st_index & 1){
			if(block_arr[end_index] == 0){
				end_index -= 2;
				--end_id;
				if(st_index >= end_index){//if st_index is at the last empty block and the last file block is empty to avoid counting anything twice
					break;
				}
			}

			while(block_arr[st_index] > 0){
				if(block_arr[end_index] == 0){
					end_index -= 2;
					--end_id;
					if(st_index >= end_index){//if st_index is at the last empty block and the last file block is empty to avoid counting anything twice
						break;
					}
				}

				block_arr[end_index] -= 1;
				block_arr[st_index] -= 1;
				checksum += end_id * cur_fs_pos;
				++cur_fs_pos;
			}

		}else{
			for(int i = 0; i < block_arr[st_index]; ++i){
				checksum += st_id * cur_fs_pos;
				++cur_fs_pos;
			}

			++st_id;
		}

		++st_index;
	}

	printf("%lu\n", checksum);

	free(input_line);
	return 0;
}
