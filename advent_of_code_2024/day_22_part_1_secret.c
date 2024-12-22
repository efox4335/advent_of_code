/*
 * sn = (((s << 6) ^ s) % (1 << 24))
 * sf = (((sn >> 5) ^ sn) % (1 << 24))
 * sk = (((sf << 11) ^ sf) % (1 << 24))
 * s + 1 = sk
 * just a simple calculation to calculate the 2000th secret number
*/
#include <stdio.h>
#include <stdlib.h>

long get_next_num(long cur_num)
{
	cur_num = (((cur_num << 6) ^ cur_num) % (1 << 24));
	cur_num = (((cur_num >> 5) ^ cur_num) % (1 << 24));
	cur_num = (((cur_num << 11) ^ cur_num) % (1 << 24));

	return cur_num;
}

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	long secret_num_sum = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		long num = atol(input_line);

		for(int i = 0; i < 2000; ++i){
			num = get_next_num(num);
		}

		secret_num_sum += num;
	}

	printf("%ld\n", secret_num_sum);

	free(input_line);
	return 0;
}
