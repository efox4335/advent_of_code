/*
take the LR instructions into and array where 1 is L and 0 is R
take the nodes onto and array of structs that store the node and the L and R nodes as text
if preformance is needed store the L and R nodes not as text but as pointers to the correct node
get all start nodes
travel down list with all nodes ending in until all nodes end in z
find where z starts looping
it turns out the inputs will start looping atfter 1 run so just find the travel discance for each input and get the lcm
this could've been way worse

with the discances just calculate thhe lcm and print
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node{
	char *node_name;
	char *left_node_name;
	char *right_node_name;
};

struct node *node_list[1000];//list of nodes
int node_amount = 0;
int direction_length = 0;
int directions[1000];

int main(void)
{
	char *line = NULL;
	char *line_tok = NULL;
	size_t length = 0;
	int max_trav_dist = 0;//max distance from all starting nodes
	int temp_dist_store = 0;
	int list_of_start_nodes[100];
	int start_node_index = 0;

	int findnodelenght(int *);
	int findnode(char *, int, int, int);
	long int lcmcalc(long int, long int);


	getline(&line, &length, stdin);

	for(int c = 0; line[c] != '\n'; ++c){//imports directions
		if(line[c] == 'R')
			directions[direction_length++] = 0;
		else
			directions[direction_length++] = 1;

	}
	free(line);
	line = NULL;

	getline(&line, &length, stdin);//skips blank line

	free(line);
	line = NULL;

	while(getline(&line, &length, stdin) > 1){//gets nodes
		node_list[node_amount] = (struct node *) malloc(sizeof(struct node));

		line_tok = strtok(line, " =(),");
		node_list[node_amount]->node_name = strdup(line_tok);

		line_tok = strtok(NULL, " =(),");
		node_list[node_amount]->left_node_name = strdup(line_tok);

		line_tok = strtok(NULL, " =(),");
		node_list[node_amount]->right_node_name = strdup(line_tok);

		free(line);
		line = NULL;
		++node_amount;
	}
	free(line);
	line = NULL;

	long int loop_z[100];

	for(int i = 0; i < node_amount; ++i){//gets list of start nodes
		if(!strncmp(node_list[i]->node_name + 2, "A", 1)){
			list_of_start_nodes[start_node_index++] = i;
			printf("%s\n", node_list[i]->node_name);
		}

	}

	for(int i = 0; i < start_node_index; ++i){
		loop_z[i] = findnodelenght(&list_of_start_nodes[i]);
	}

	printf("hi\n");

	for(int i = 1; i < start_node_index; ++i){
		loop_z[0] = lcmcalc(loop_z[0], loop_z[i]);
	}

	printf("answer: %ld\n", loop_z[0]);

	return 0;
}

//returns the distance from a starting node to a node ending in "Z"
int findnodelenght(int *starting_index)
{
	int findnode(char *, int, int, int);
	int i = *starting_index;
	int j = 0;
	int trav_dist = 0;//number of nodes traversed

	if(directions[j] == 1)
			i = findnode(node_list[i]->left_node_name, 3, 0, 0);
		else
			i = findnode(node_list[i]->right_node_name, 3, 0, 0);

	++trav_dist;

	if(j < direction_length - 1)
			++j;
		else
			j = 0;


	while(strncmp(node_list[i]->node_name + 2, "Z", 1)){
		//printf("%s %d\n", node_list[i]->node_name, directions[j]);
		if(directions[j] == 1)
			i = findnode(node_list[i]->left_node_name, 3, 0, 0);
		else
			i = findnode(node_list[i]->right_node_name, 3, 0, 0);

		if(i == -1)//node not found
			return -1;

		if(j < direction_length - 1)
			++j;
		else
			j = 0;

		++trav_dist;
	}
	*starting_index = i;
	return trav_dist;
}

//args name of node to search for, length of the comperison, where to start looking, where in st1 to start compare
//finds node and returns the index of found node returns -1 if node not found
int findnode(char *node_name, int cmplen, int st_from, int st_cmp)
{
	for(int i = st_from; i < node_amount; ++i){
		if(!strncmp(node_list[i]->node_name + st_cmp, node_name, cmplen))
			return i;

	}
	return -1;
}

//uses euclidean algorithm
long int get_gcd(long int a, long int b)
{
	while(1){
		if(a >= b){
			a -= b;
		}
		else if(a < b){
			b -= a;
		}
		if(a == 0)
			return b;
	}
}

/*
gets lcm form lcm(a, b) = (a*b)/gcd(a,b)
*/
long int lcmcalc(long int a, long int b)
{
	long int get_gcd(long int, long int);
	return (a * b)/ get_gcd(a,b);
}
