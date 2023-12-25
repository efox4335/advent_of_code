//import graph
//do Stoer-Wagner algorithm to find group with the least cuts count size of group
//multiply by the ammount of nodes not in group
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node{
	int has_been;
	char *name;
	struct node *connections[2000];
	int con_id[2000];
	int weight[2000];
	char *con_name[2000];
	int con_count;
	int real_con_count;
};

typedef struct{
	struct node *node_1;
	struct node *node_2;
	int num;
}path;

struct node node_arr[2000];
int node_num = 0;
int con_num = 0;
path path_arr[5000];

int main(void)
{
	void add_con(struct node *);
//	void merge(struct node *, struct node *);
	void bfs(struct node *, struct node *);
	int comp(const void *, const void *);
	int count_bfs(struct node *);

	char *line = NULL;
	size_t lim = 0;
	char *token;
	int biggest_ind[3] = {0, 0, 0};
	int store;
	int store_b;

	while(getline(&line, &lim, stdin) > 1){
		token = strtok(line, ": \n");
		node_arr[node_num].name = strdup(token);
		node_arr[node_num].con_count = 0;
		node_arr[node_num].has_been = 0;

		while((token = strtok(NULL, ": \n")) != NULL){
			node_arr[node_num].con_name[node_arr[node_num].con_count] = strdup(token);
			node_arr[node_num].con_count += 1;
			node_arr[node_num].real_con_count = 0;
		}

		++node_num;
	}

	for(int i = 0; i < node_num; ++i){
	//	printf("%s : ", node_arr[i].name);
	//	for(int j = 0; j < node_arr[i].con_count; ++j)
	//		printf("%s ", node_arr[i].con_name[j]);
	//	printf("\n");

		add_con(&node_arr[i]);
	}

	//printf("%s\n", node_arr[2].name);

	for(int i = 0; i < node_num; ++i){
		printf("%s : ", node_arr[i].name);
		for(int j = 0; j < node_arr[i].real_con_count; ++j){
			if(node_arr[i].connections[j] != NULL)
				printf("%s ", node_arr[i].connections[j]->name);
		}
		printf("\n");
	}

	for(int i = 0; i < node_num; ++i){
		for(int j = i + 1; j < node_num; ++j)
			bfs(&node_arr[i], &node_arr[j]);
	}

	//qsort(path_arr, con_num, sizeof(path), comp);

	for(int i = 0; i < con_num; ++i){
		printf("%s %s %d\n", path_arr[i].node_1->name, path_arr[i].node_2->name, path_arr[i].num);

		if(path_arr[i].num > path_arr[biggest_ind[0]].num){
			biggest_ind[2] = biggest_ind[1];
			biggest_ind[1] = biggest_ind[0];
			biggest_ind[0] = i;
		}
		else if(path_arr[i].num > path_arr[biggest_ind[1]].num){
			biggest_ind[2] = biggest_ind[1];
			biggest_ind[1] = i;
		}
		else if(path_arr[i].num > path_arr[biggest_ind[2]].num){
			biggest_ind[2] = i;
		}
	}

	printf("%d %d %d\n", path_arr[biggest_ind[0]].num, path_arr[biggest_ind[1]].num, path_arr[biggest_ind[2]].num);
	path_arr[biggest_ind[0]].num = -1;
	path_arr[biggest_ind[1]].num = -1;
	path_arr[biggest_ind[2]].num = -1;

	store = count_bfs(&node_arr[0]);
	store_b = node_num - store;

	printf("%d %d %d %d\n", store, store_b, node_num, store * store_b);

	return 0;
}

//finds shortest distance between a and b and adds the paths to path_arr
int count_bfs(struct node *a)
{
	typedef struct{
		struct node *call_node;
		struct node *curr_node;
		int trav_id;
		int call_id;
	}open_node;

	open_node open_list[200000];

	int open_que = 1;
	int current_place = 0;
	struct node *curr_node = a;
	int back_track;

	open_list[0].curr_node = a;
	open_list[0].call_node = NULL;
	open_list[0].trav_id = -1;
	open_list[0].call_id = -1;

	for(int i = 0; i < node_num; ++i){
		node_arr[i].has_been = 0;
	}

	a->has_been = 1;

	while(current_place != open_que){
		curr_node = open_list[current_place].curr_node;

		for(int i = 0; i < curr_node->real_con_count; ++i){
			if(curr_node->connections[i]->has_been == 0 && path_arr[curr_node->con_id[i]].num != -1){
				open_list[open_que].call_node = curr_node;
				open_list[open_que].curr_node = curr_node->connections[i];
				open_list[open_que].trav_id = curr_node->con_id[i];
				open_list[open_que].call_id = current_place;
				curr_node->connections[i]->has_been = 1;
				++open_que;
				printf("%d %s %s\n", path_arr[curr_node->con_id[i]].num, curr_node->name,
				curr_node->connections[i]->name);
			}
		}
		++current_place;
	}

	return current_place;
}

//finds shortest distance between a and b and adds the paths to path_arr
void bfs(struct node *a, struct node *b)
{
	typedef struct{
		struct node *call_node;
		struct node *curr_node;
		int trav_id;
		int call_id;
	}open_node;

	open_node open_list[200000];

	int open_que = 1;
	int current_place = 0;
	struct node *curr_node = a;
	int back_track;

	open_list[0].curr_node = a;
	open_list[0].call_node = NULL;
	open_list[0].trav_id = -1;
	open_list[0].call_id = -1;

	for(int i = 0; i < node_num; ++i)
		node_arr[i].has_been = 0;

	while((curr_node = open_list[current_place].curr_node) != b){
		for(int i = 0; i < curr_node->real_con_count; ++i){
			//printf("%d\n", i);
			if(curr_node->connections[i]->has_been == 0){
				open_list[open_que].call_node = curr_node;
				open_list[open_que].curr_node = curr_node->connections[i];
				open_list[open_que].trav_id = curr_node->con_id[i];
				open_list[open_que].call_id = current_place;
				curr_node->connections[i]->has_been = 1;
				++open_que;
			}
		}
		//printf("%d %d\n", current_place, open_que);
		//printf("%s %s %s\n", a->name, b->name, open_list[current_place].curr_node->name);
		++current_place;

		if(current_place == open_que)
			return;
	}

//	printf("%s %s %s\n", a->name, b->name, open_list[current_place].curr_node->name);

	for(back_track = current_place; open_list[back_track].call_node != NULL; back_track = open_list[back_track].call_id)
		path_arr[open_list[back_track].trav_id].num += 1;

}

//adds both way connections too and from node
void add_con(struct node *np)
{
	struct node *find_node(char *);

	struct node *cnp;

	for(int i = 0; i < np->con_count; ++i){
		cnp = find_node(np->con_name[i]);
		if(cnp != NULL){
			np->connections[np->real_con_count] = cnp;
			np->con_id[np->real_con_count] = con_num;
			np->real_con_count += 1;

			cnp->connections[cnp->real_con_count] = np;
			cnp->con_id[cnp->real_con_count] = con_num;
			cnp->real_con_count += 1;

			path_arr[con_num].node_1 = np;
			path_arr[con_num].node_2 = cnp;
			path_arr[con_num].num = 0;

			++con_num;
		}
	}
}

//finds node in node_arr
struct node *find_node(char *name)
{
	for(int i = 0; i < node_num; ++i)
		if(!strcmp(node_arr[i].name, name))
			return &node_arr[i];

	node_arr[node_num].name = strdup(name);
	node_arr[node_num].con_count = 0;
	node_arr[node_num].has_been = 0;
	node_arr[node_num].real_con_count = 0;

	++node_num;

	return &node_arr[node_num - 1];
}

int comp(const void *a, const void *b)
{
	return (((path *) b)->num - ((path *) a)->num);
}

/*
//gets the new node to add to group
struct node *get_new_add(struct node *start)
{
	struct conn{
		int weight;
		struct node *out;

	};

	struct node *group[2000];
	int group_size = 0;

	//while(group_size != )
}

void copy_node_arr(struct node *a, struct node)
{

}

//add connections from b to a and removes connection a to b
void merge(struct node *a, struct node *b)
{
	int does_connect(struct node *, struct node *);
	void add_weight(struct node *, struct node *, struct node *, int b_to_c);

	int disscon = does_connect(a, b);
	a->weight[disscon] = -1;

	for(int i = 0; i < b->real_con_count; ++b){
		add_weight(a, b, b->connections[i], i);
	}
}

//checks if node a and b connect to node c if they do adds sum of a and b weight to a and b weight
void add_weight(struct node *a, struct node *b, struct node *c, int b_to_c)
{
	static int count = 0;
	count++;
	int does_connect(struct node *, struct node *);

	int a_to_c = does_connect(a, c);
	printf("in1\n");
	int c_to_b = does_connect(c, b);
	printf("in2\n");
	int c_to_a = does_connect(c, a);
	printf("in3\n");

	if(a_to_c != -1){
		printf("ee%s %s %s %d\n", a->name, b->name, c->name, count);
		a->weight[a_to_c] += b->weight[b_to_c];
		c->weight[c_to_a] = a->weight[a_to_c] + b->weight[b_to_c];
		c->weight[c_to_b] = -1;
	}
	else{
		printf("ee%s %s %s %d\n", a->name, b->name, c->name, count);
		a_to_c = a->real_con_count;
		c_to_a = c->real_con_count;

		a->weight[a_to_c] = b->weight[b_to_c];
		a->connections[a_to_c] = c;
		c->weight[c_to_a] = b->weight[b_to_c];
		c->connections[c_to_a] = a;
		c->weight[c_to_b] = -1;

		a->real_con_count += 1;
		c->real_con_count += 1;
	}

}

//checks if node a and node b connect and returns the index of the connection in a
int does_connect(struct node *a, struct node *b)
{
	for(int i = 0; i < a->real_con_count; ++i){
		if(a->connections[i] == b){
			printf("con: %s %s %s\n", a->name, b->name, a->connections[i]->name);
			return i;
		}
	}
	printf("not found %s %s\n", a->name, b->name);
	return -1;
}
*/
