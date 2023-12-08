/*
take the LR instructions into and array where 1 is L and 0 is R
take the nodes onto and array of structs that store the node and the L and R nodes as text
if preformance is needed store the L and R nodes not as text but as pointers to the correct node
count the number of steps it takes to get from AAA to ZZZ
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node{
	char *node_name;
	char *left_node_name;
	char *right_node_name;
};

int main(void)
{
	int directions[1000];
	struct node *node_list[1000];//list of nodes
	char *line = NULL;
	char *line_tok = NULL;
	size_t length = 0;
	int direction_length = 0;
	int node_amount = 0;
	int trav_dist = 0;//number of nodes traversed

	int findnode(struct node **, char *, int );

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

	int i = findnode(node_list, "AAA", node_amount);
	int j = 0;

	while(strcmp(node_list[i]->node_name, "ZZZ")){
		printf("%s %d\n", node_list[i]->node_name, directions[j]);
		if(directions[j] == 1)
			i = findnode(node_list, node_list[i]->left_node_name, node_amount);
		else
			i = findnode(node_list, node_list[i]->right_node_name, node_amount);

		if(j < direction_length - 1)
			++j;
		else
			j = 0;

		++trav_dist;
	}

	printf("%d\n", trav_dist);

	return 0;
}

//finds node and returns the index of found node
int findnode(struct node *n_list[], char *node_name, int node_amount)
{
	for(int i = 0; i < node_amount; ++i){
		if(!strcmp(n_list[i]->node_name, node_name))
			return i;

	}
}
