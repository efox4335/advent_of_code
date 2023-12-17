/*
use dijkstra's algorithm to find shortest path to end node
the alg with eb inplemented with a
	find node function finds the closest node to start and returns its index in the list of open nodes
	get new node function that finds new nodes to be added eg. takes into account direction of the calling node and line place
	node that adds new nodes to an array of open nodes
	function that removes the calling node and places the last node in it's place
	function that turns relative direction to absolute direction into a new absolute direction and calls add node with this

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum directions{
	NORTH,
	SOUTH,
	EAST,
	WEST
};

enum ret_directions{
	LEFT,
	RIGHT,
	STRAIGHT
};

struct node{
	int cool_val;
	int dist_from_start;
	int seen;
};

struct open_node{
	int row;
	int col;
	int call_direrection;
	int line_place;
	struct node *node_pointer;
};

//lookup table of relative direction to absolute direction
//stored left right centerfrom north south east west
int new_dir[4][3] = {
	{WEST, EAST, NORTH},//for north
	{EAST, WEST, SOUTH},//for south
	{NORTH, SOUTH, EAST},//for east
	{SOUTH, NORTH, WEST}//for west
};

//lookup table of relative offsets direction to absolute direction
//stored left right centerfrom north south east west
int dir_off[4][3] = {
	{-1, 1, -1},//for north
	{1, -1, 1},//for south
	{-1, 1, 1},//for east
	{1, -1, -1}//for west
};

struct node node_list[3][4][200][200];//line place, call_direction, row, col
struct open_node open_node_array[100000];

int map_hight = 0;
int map_length = 0;
int open_node_number = 0;

int city_map[200][200];
char path[200][200];

int main(void)
{
	char *line = NULL;
	size_t lim = 0;
	int dist_from_start = 0;
	int expand_node = 0;

	void clear_node_list(void);
	int find_end(void);
	void add_wrap(struct open_node *, int);
	void delete_node(int);
	int find_closest(void);
	void prt_path(void);
	void set_path(void);

	while(getline(&line, &lim, stdin) > 1){
		for(int i = 0; line[i] != '\n'; ++i){
			map_length = i + 1;
			city_map[map_hight][i] = line[i] - 48;
		}
		++map_hight;
	}

	clear_node_list();

	node_list[0][EAST][0][0].cool_val = 0;
	node_list[0][EAST][0][0].seen = 1;
	node_list[0][EAST][0][0].dist_from_start = -1;
	open_node_array[0].call_direrection = EAST;
	open_node_array[0].col = 0;
	open_node_array[0].row = 0;
	open_node_array[0].line_place = 0;
	open_node_array[0].node_pointer = &node_list[0][EAST][0][0];
	++open_node_number;

	set_path();

	while((dist_from_start = find_end()) == 0){
//		prt_path();
//		printf("\n");
		expand_node = find_closest();
		add_wrap(&open_node_array[expand_node], LEFT);
		add_wrap(&open_node_array[expand_node], RIGHT);
		add_wrap(&open_node_array[expand_node], STRAIGHT);
		delete_node(expand_node);
	}

	printf("%d\n", dist_from_start);
	return 0;
}

//convets absolute direction and node to proper args for add_node
void add_wrap(struct open_node *node, int direction)
{
	void add_node(int, int, int, int, int);
	if(direction == STRAIGHT){
		switch(node->call_direrection){
			case NORTH:
				add_node(node->line_place + 1, new_dir[node->call_direrection][direction],
				node->row + dir_off[node->call_direrection][direction], node->col, node->node_pointer->dist_from_start);
				break;
			case SOUTH:
				add_node(node->line_place + 1, new_dir[node->call_direrection][direction],
				node->row + dir_off[node->call_direrection][direction], node->col, node->node_pointer->dist_from_start);
				break;
			case EAST:
				add_node(node->line_place + 1, new_dir[node->call_direrection][direction],
				node->row, node->col + dir_off[node->call_direrection][direction], node->node_pointer->dist_from_start);
				break;
			case WEST:
				add_node(node->line_place + 1, new_dir[node->call_direrection][direction],
				node->row, node->col + dir_off[node->call_direrection][direction], node->node_pointer->dist_from_start);
				break;
		}
	}
	else{
		switch(node->call_direrection){
			case NORTH:
				add_node(0, new_dir[node->call_direrection][direction],
				node->row, node->col + dir_off[node->call_direrection][direction], node->node_pointer->dist_from_start);
				break;
			case SOUTH:
				add_node(0, new_dir[node->call_direrection][direction],
				node->row, node->col + dir_off[node->call_direrection][direction], node->node_pointer->dist_from_start);
				break;
			case EAST:
				add_node(0, new_dir[node->call_direrection][direction],
				node->row + dir_off[node->call_direrection][direction], node->col, node->node_pointer->dist_from_start);
				break;
			case WEST:
				add_node(0, new_dir[node->call_direrection][direction],
				node->row + dir_off[node->call_direrection][direction], node->col, node->node_pointer->dist_from_start);
				break;
		}
	}
}

//finds if end node it in start and return dist from start if so returns 0 otherwise
int find_end(void)
{
	for(int i = 0; i < open_node_number; ++i)
		if(open_node_array[i].row == map_hight - 1 && open_node_array[i].col == map_length - 1)
			return open_node_array[i].node_pointer->dist_from_start;
	return 0;
}

//deletes open node by overwriting it with last node in list and shorting open_node_number
void delete_node(int delete_node)
{
	void copy_open_node(struct open_node *, struct open_node *);

	copy_open_node(&open_node_array[delete_node], &open_node_array[open_node_number - 1]);
	--open_node_number;
}

//adds node at new index and checks if the insertion is valid
void add_node(int line_place, int call_direction, int row, int col, int dist_from_start)
{
	void add_path(int, int, int);
	if(row < 0 || col < 0 || row >= map_hight || col >= map_length || line_place > 2 || node_list[line_place][call_direction][row][col].seen == 1)
		return;

	add_path(row, col, call_direction);
	open_node_array[open_node_number].call_direrection = call_direction;
	open_node_array[open_node_number].col = col;
	open_node_array[open_node_number].row = row;
	open_node_array[open_node_number].node_pointer = &node_list[line_place][call_direction][row][col];
	open_node_array[open_node_number].node_pointer->seen = 1;

	//-1 marks starting block
	if(dist_from_start != -1){
		open_node_array[open_node_number].line_place = line_place;
		open_node_array[open_node_number].node_pointer->dist_from_start += dist_from_start +
		open_node_array[open_node_number].node_pointer->cool_val;
	}
	else{
		open_node_array[open_node_number].line_place = 0;
		open_node_array[open_node_number].node_pointer->dist_from_start += open_node_array[open_node_number].node_pointer->cool_val;
	}
	++open_node_number;
}

//finds closest node to start
int find_closest(void)
{
	int lowest_dist = 0;

	for(int i = 1; i < open_node_number; ++i)
		if(open_node_array[i].node_pointer->dist_from_start < open_node_array[lowest_dist].node_pointer->dist_from_start)
			lowest_dist = i;
	return lowest_dist;
}

//copies open_node 2 to open_node 1
void copy_open_node(struct open_node *node_1, struct open_node *node_2)
{
	node_1->call_direrection = node_2->call_direrection;
	node_1->col = node_2->col;
	node_1->line_place = node_2->line_place;
	node_1->row = node_2->row;
	node_1->node_pointer = node_2->node_pointer;
}

//clears node_list
void clear_node_list(void)
{
	for(int i = 0; i < 200; ++i)
		for(int j = 0; j < 200; ++j)
			for(int k = 0; k < 4; ++k)
				for(int h = 0; h < 3; ++h){
					node_list[h][k][i][j].cool_val = city_map[i][j];
					node_list[h][k][i][j].dist_from_start = 0;
					node_list[h][k][i][j].seen = 0;
				}

}

//adds path with direction
void add_path(int row, int col, int direction)
{
	switch (direction){
		case NORTH:
			path[row][col] = '^';
			break;
		case SOUTH:
			path[row][col] = 'v';
			break;
		case EAST:
			path[row][col] = '>';
			break;
		case WEST:
			path[row][col] = '<';
			break;
	}
}

//sets path to all '.'
void set_path(void)
{
	for(int i = 0; i < map_hight; ++i)
		for(int j = 0; j < map_length; ++j)
			path[i][j] = '.';
}
//prints path
void prt_path(void)
{
	for(int i = 0; i < map_hight; ++i){
		for(int j = 0; j < map_length; ++j)
			printf("%c", path[i][j]);
		printf("\n");
	}
}
//prints map
void prt_map(void)
{
	for(int i = 0; i < map_hight; ++i){
		for(int j = 0; j < map_length; ++j)
			printf("%d", city_map[i][j]);
		printf("\n");
	}
}
