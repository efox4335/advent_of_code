//store spaces taken in 3d array
//store spaces taken as pointers to bricks nullptr means empty
//store list of bricks in array
//mark the starting place of all bricks in the spaces taken array
//from the bottom up search for the highest point under a brick and move it so it is at that hight + 1
//for each brick check if all bricks above it are suppoted by another brick
//for all bricks where this is not true add them to a list of bricks to be checked
//check until there are no more bricks to be checked
//print the sum of this value for all bricks
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
	int st_row;
	int st_col;
	int st_hight;
	int end_row;
	int end_col;
	int end_hight;
}brick;

//for moving bricks
typedef struct{
	int row;
	int col;
	int hight;
}cord;

brick brick_array[2000];
brick *brick_space[500][500][500];//stored by hight, row, col
brick ground;
int brick_num = 0;

int main(void)
{
	void reset_space(void);
	void print_space(int, int, int);
	void place_brick(brick *);
	void remove_brick(brick *);
	int get_hight(brick *);
	int hight_cmp(const void *, const void *);
	void brick_fall(brick *);
	void brick_fall(brick *);
	void swap_ends(brick *);
	int can_remove(brick *, brick *removed_brick_arr[], int *);

	char *line = NULL;
	size_t lim = 0;
	char *token;
	int total_removiable_bricks = 0;
	int scan_num;
	int remove_num;
	brick *can_remove_arr[2000];

	reset_space();

	while(getline(&line, &lim, stdin) > 1){
		//starting column
		token = strtok(line, " ,~\n");
		brick_array[brick_num].st_col = atoi(token);

		//starting row
		token = strtok(NULL, " ,~\n");
		brick_array[brick_num].st_row = atoi(token);

		//starting hight
		token = strtok(NULL, " ,~\n");
		brick_array[brick_num].st_hight = atoi(token);

		//ending column
		token = strtok(NULL, " ,~\n");
		brick_array[brick_num].end_col = atoi(token);

		//ending row
		token = strtok(NULL, " ,~\n");
		brick_array[brick_num].end_row = atoi(token);

		//ending hight
		token = strtok(NULL, " ,~\n");
		brick_array[brick_num].end_hight = atoi(token);

		if(brick_array[brick_num].st_hight > brick_array[brick_num].end_hight){
			swap_ends(&brick_array[brick_num]);
		}

		++brick_num;
	}

	//print_space(10, 10, 10);

	//sorts by hight so the lowest elements fall first
	qsort(brick_array, brick_num, sizeof(brick), hight_cmp);

	for(int i = 0; i < brick_num; ++i){
		place_brick(&brick_array[i]);
	}

	for(int i = 0; i < brick_num; ++i){
		brick_fall(&brick_array[i]);
	}

//	print_space(10, 10, 10);
	printf("%d\n", brick_num);

	for(int i = 0; i < brick_num; ++i){
		scan_num = 0;
		remove_num = 1;
		can_remove_arr[0] = &brick_array[i];
		while(scan_num != remove_num){
			total_removiable_bricks += can_remove(can_remove_arr[scan_num], can_remove_arr, &remove_num);
			++scan_num;
		}
		for(int j = 0; j < remove_num; ++j)
			place_brick(can_remove_arr[j]);
	}

	print_space(10, 10, 10);

	printf("%d\n", total_removiable_bricks);

	return 0;
}

//checks if a brick can be removed
int can_remove(brick *brick_ptr, brick *removed_brick_arr[], int *removed_brick_num)
{
	int get_cords(brick *, cord *);
	void remove_brick(brick *);
	void place_brick(brick *);
	int get_hight(brick *);

	cord cord_array[1000];
	int cord_num = 0;
	int old_hight;
	int new_hight;
	brick *above_ptr;
	int total_bricks_moved = 0;

	cord_num = get_cords(brick_ptr, cord_array);

	remove_brick(brick_ptr);

	for(int i = 0; i < cord_num; ++i){
		above_ptr = brick_space[cord_array[i].hight + 1][cord_array[i].row][cord_array[i].col];

		if(above_ptr != NULL){
			old_hight = above_ptr->st_hight;
			new_hight = get_hight(above_ptr);

			if(old_hight != new_hight){
				removed_brick_arr[*removed_brick_num] = above_ptr;
				*removed_brick_num += 1;
				++total_bricks_moved;
				remove_brick(above_ptr);
			}
		}
	}

	return total_bricks_moved;
}

//moves brick_ptr to the lowest spot it can be and changes brick_space
void brick_fall(brick *brick_ptr)
{
	int get_hight(brick *);
	void remove_brick(brick *);
	void place_brick(brick *);

	int new_hight = 0;

	new_hight = get_hight(brick_ptr);

	remove_brick(brick_ptr);

	if(brick_ptr->end_hight > brick_ptr->st_hight){
		brick_ptr->end_hight = new_hight + abs(brick_ptr->end_hight - brick_ptr->st_hight);
		brick_ptr->st_hight = new_hight;
	}
	else{
		brick_ptr->st_hight = new_hight + abs(brick_ptr->st_hight - brick_ptr->end_hight);
		brick_ptr->end_hight = new_hight;
	}

	place_brick(brick_ptr);
}

//returns the highest point below brick_ptr + 1
int get_hight(brick *brick_ptr)
{
	int get_cords(brick *, cord *);

	cord cord_array[1000];
	int cord_num = 1;
	int highest_point = 1;
	int temp_highest_point = 0;

	cord_num = get_cords(brick_ptr, cord_array);

	for(int i = 0; i < cord_num; ++i){
		for(temp_highest_point = cord_array[i].hight; brick_space[temp_highest_point][cord_array[i].row][cord_array[i].col] == brick_ptr ||
		brick_space[temp_highest_point][cord_array[i].row][cord_array[i].col] == NULL; --temp_highest_point);

		if(temp_highest_point + 1 > highest_point)
			highest_point = temp_highest_point + 1;
	}

	return highest_point;
}

//removes brick at brick_ptr in bricl_space
void remove_brick(brick *brick_ptr)
{
	int get_cords(brick *, cord *);

	cord cord_array[1000];
	int cord_num = 0;

	cord_num = get_cords(brick_ptr, cord_array);

	for(int i = 0; i < cord_num; ++i){
		brick_space[cord_array[i].hight][cord_array[i].row][cord_array[i].col] = NULL;
	}
}

//places brick_ptr on brick_space
void place_brick(brick *brick_ptr)
{
	int get_cords(brick *, cord *);

	cord cord_array[1000];
	int cord_num = 0;

	cord_num = get_cords(brick_ptr, cord_array);

	for(int i = 0; i < cord_num; ++i){
		brick_space[cord_array[i].hight][cord_array[i].row][cord_array[i].col] = brick_ptr;
	}
}

//gets list of all cords in brick_ptr and puts it on cord_array
//returns number of cords gotten
int get_cords(brick *brick_ptr, cord cord_arr[])
{
	int lowest_hight;
	int highest_hight;
	int lowest_row;
	int highest_row;
	int lowest_col;
	int highest_col;

	if(brick_ptr->end_hight > brick_ptr->st_hight){
		lowest_hight = brick_ptr->st_hight;
		highest_hight = brick_ptr->end_hight;
	}
	else{
		lowest_hight = brick_ptr->end_hight;
		highest_hight = brick_ptr->st_hight;
	}

	if(brick_ptr->end_col > brick_ptr->st_col){
		lowest_col = brick_ptr->st_col;
		highest_col = brick_ptr->end_col;
	}
	else{
		lowest_col = brick_ptr->end_col;
		highest_col = brick_ptr->st_col;
	}

	if(brick_ptr->end_row > brick_ptr->st_row){
		lowest_row = brick_ptr->st_row;
		highest_row = brick_ptr->end_row;
	}
	else{
		lowest_row = brick_ptr->end_row;
		highest_row = brick_ptr->st_row;
	}
	int cord_num = 0;
	for(int i = lowest_hight; i <= highest_hight; ++i)
		for(int j = lowest_col; j <= highest_col; ++j)
			for(int k = lowest_row; k <=highest_row; ++k){
				cord_arr[cord_num].hight = i;
				cord_arr[cord_num].col = j;
				cord_arr[cord_num].row = k;
				++cord_num;
			}
	return cord_num;
}

//sets all brick_space pointers to null
void reset_space(void)
{
	for(int i = 0; i < 500; ++i)
		for(int j = 0; j < 500; ++j)
			for(int k = 0; k < 500; ++k)
				brick_space[i][j][k] = NULL;

	for(int j = 0; j < 500; ++j)
		for(int k = 0; k < 500; ++k)
			brick_space[0][j][k] = &ground;
}

//prints brick_space in slices up to hight row col
void print_space(int hight, int row, int col)
{
	for(int i = 0; i <= hight; ++i){
		printf("layer %d:\n", i);
		for(int j = 0; j <= row; ++j){
			for(int k = 0; k <= col; ++k){
				if(brick_space[i][j][k] == NULL)
					printf("0");
				else
					printf("1");

			}
			printf("\n");
		}
		printf("\n");
	}
}

//compares starting hight of bricks
int hight_cmp(const void *a, const void *b){
	return (((brick *) a)->st_hight - ((brick *) b)->st_hight);
}

//swaps the start and end blocks
void swap_ends(brick *brick_ptr)
{
	int temp_hight;
	int temp_col;
	int temp_row;

	temp_hight = brick_ptr->end_hight;
	temp_col = brick_ptr->end_col;
	temp_row = brick_ptr->end_row;

	brick_ptr->end_hight = brick_ptr->st_hight;
	brick_ptr->end_col = brick_ptr->st_col;
	brick_ptr->end_row = brick_ptr->st_row;

	brick_ptr->st_hight = temp_hight;
	brick_ptr->end_col = temp_col;
	brick_ptr->st_row = temp_row;
}
