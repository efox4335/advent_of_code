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

	free(input_line);
	return 0;
}
