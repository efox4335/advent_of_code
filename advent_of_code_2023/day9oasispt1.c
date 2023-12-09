/*
find the derivatives of the points and calculate based on that what the next point will be
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	long int getnext(int *, int);

	char *line = NULL;
	char *num = NULL;
	size_t lim = 0;
	int point_num = 0;
	long int sum_of_next = 0;

	int points[1000];

	while(getline(&line, &lim, stdin) > 1){
		num = strtok(line, " ");
		while(num != NULL){
			points[point_num++] = atoi(num);
			num = strtok(NULL, " ");
		}

		sum_of_next += getnext(points, point_num);
		printf("%ld\n", sum_of_next);
		point_num = 0;
		printf("\n");
	}
}


//gets the next point from input array by extrapolating based on derivatives of points on an array
long int getnext(int *inp_arr, int point_num)
{
	int layers[100];//stores the layers of derivatives as we calculate them
	long int next_store;//stores the points extrapolated from the derivatives
	long int last_num_store[100];//stores the last point calculated from the derivatives
	int num_of_layers = 0;//stores the number of times we have to calculate the derivatives till we get to 0
	int break_0 = 1;//stores is all derivatives are 0

	for(int i = 0; i < point_num; ++i)//copies input to local array
		layers[i] = inp_arr[i];

	while(break_0){//calculates the derivatives layers
		last_num_store[num_of_layers] = layers[point_num - num_of_layers - 1];

		break_0 = 0;
		for(int i = 1; i < point_num - num_of_layers; ++i){
			layers[i - 1] = layers[i - 1] - layers[i];
			printf("%d ", layers[i - 1]);
			if(layers[i - 1] != 0)
				break_0 = 1;
		}
		++num_of_layers;
		printf(", %d %ld\n", num_of_layers, last_num_store[num_of_layers - 1]);

	}

	next_store = last_num_store[num_of_layers - 1];

	for(int i = num_of_layers - 2; i >= 0; --i)
		next_store = last_num_store[i] - next_store;

	return next_store;
}
