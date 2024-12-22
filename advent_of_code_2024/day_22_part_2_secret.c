/*
 * sn = (((s << 6) ^ s) % (1 << 24))
 * sf = (((sn >> 5) ^ sn) % (1 << 24))
 * sk = (((sf << 11) ^ sf) % (1 << 24))
 * s + 1 = sk
 * just a simple calculation to calculate the 2000th secret number
 *
 * store each possable combinations of differences in an array along with the total sum for that sequence and weather it has been seen for this seed to avoid counting more than once
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

typedef struct{
	long sum;
	long last_arr_seen;
}seq_sum;

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	seq_sum seq_arr[20][20][20][20];

	for(int i = 0; i < 20; ++i){
		for(int j = 0; j < 20; ++j){
			for(int k = 0; k < 20; ++k){
				for(int l = 0; l < 20; ++l){
					seq_arr[i][j][k][l].sum = 0;
					seq_arr[i][j][k][l].last_arr_seen = -1;
				}
			}
		}
	}

	int cur_seed = 0;
	long largest_sum = 0;

	while(getline(&input_line, &lim, stdin) > 1){
		long num = atol(input_line);

		int cur_seq[4] = {0, 0, 0, 0};
		int seq_ind = 0;
		for(int i = 0; i < 2000; ++i){
			long next_num = get_next_num(num);

			long cur_seq_pt = ((next_num % 10) - (num % 10)) + 9;

			if(seq_ind >= 4){
				cur_seq[0] = cur_seq[1];
				cur_seq[1] = cur_seq[2];
				cur_seq[2] = cur_seq[3];
				cur_seq[3] = cur_seq_pt;

				if(seq_arr[cur_seq[0]][cur_seq[1]][cur_seq[2]][cur_seq[3]].last_arr_seen != cur_seed){
					seq_arr[cur_seq[0]][cur_seq[1]][cur_seq[2]][cur_seq[3]].last_arr_seen = cur_seed;
					seq_arr[cur_seq[0]][cur_seq[1]][cur_seq[2]][cur_seq[3]].sum += (next_num % 10);

					if(seq_arr[cur_seq[0]][cur_seq[1]][cur_seq[2]][cur_seq[3]].sum > largest_sum){
						largest_sum = seq_arr[cur_seq[0]][cur_seq[1]][cur_seq[2]][cur_seq[3]].sum;
					}
				}
			}else{
				cur_seq[seq_ind] = cur_seq_pt;
				++seq_ind;
			}

			num = next_num;
		}

		++cur_seed;
	}

	printf("%ld\n", largest_sum);

	free(input_line);
	return 0;
}
