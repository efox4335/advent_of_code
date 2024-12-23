/*
 * this is just a graph theory problem
 * ill store the graph in an edge adjacency matrix
 * to keep track of vertices ill have a 26 by 26 array where each leter in the vertex name is set to a number
 * if there is no entry for that vertex is is assigned a nuber
 * edges will only be added to the adjacency matrix with [lower number vertex][higher number vertex] to avoid double counting
 *
 * finding the largest compelete subgraph is np hard :(
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum{EDGE = 1, NEDGE = 0};
enum{MAX_VER_COUNT = 26 * 26};

typedef struct{
	int ver_1;
	int ver_2;
}edge;

//returns 1 if new edge was added 0 if edge already existed
int add_ver(const char *ver, int ver_lookup[26][26], const int edge_num)
{
	const int f_letter = ver[0] - 'a';
	const int s_letter = ver[1] - 'a';

	if(ver_lookup[f_letter][s_letter] == -1){
		ver_lookup[f_letter][s_letter] = edge_num;

		return 1;
	}

	return 0;
}


//returns number of vertex exits program if called on edge that does not exist
int find_ver(const char *ver, int ver_lookup[26][26])
{
	const int f_letter = ver[0] - 'a';
	const int s_letter = ver[1] - 'a';

	if(ver_lookup[f_letter][s_letter] == -1){
		printf("no vertex for %s\n", ver);

		exit(0);
	}

	return ver_lookup[f_letter][s_letter];
}

//adds edge only in form [lowest][highest] will swap if needed
void add_edge(edge cur_edge, char adj_matrix[MAX_VER_COUNT][MAX_VER_COUNT])
{
	if(cur_edge.ver_1 > cur_edge.ver_2){
		int temp = cur_edge.ver_1;
		cur_edge.ver_1 = cur_edge.ver_2;
		cur_edge.ver_2 = temp;
	}

	adj_matrix[cur_edge.ver_1][cur_edge.ver_2] = EDGE;
}

int largest_compelete_subgraph(const char adj_matrix[MAX_VER_COUNT][MAX_VER_COUNT], int *subgraph, int *ver_arr, int ver_count, int cur_subgraph_size, int cur_largest)
{
	if(ver_count + cur_subgraph_size < cur_largest){
		return cur_subgraph_size;
	}

	if(ver_count == 0){
		return cur_subgraph_size;
	}

	int *nabs = malloc((ver_count - 1) * sizeof(int));
	int largest = 0;

	int *cur_subgraph = malloc(MAX_VER_COUNT * sizeof(int));
	for(int i = 0; i < cur_subgraph_size; ++i){
		cur_subgraph[i] = subgraph[i];
	}

	for(int i = 0; i < ver_count; ++i){
		int nab_count = 0;

		for(int j = i + 1; j < ver_count; ++j){
			if(adj_matrix[ver_arr[i]][ver_arr[j]] == EDGE || adj_matrix[ver_arr[j]][ver_arr[i]] == EDGE){
				nabs[nab_count] = ver_arr[j];

				++nab_count;
			}
		}

		cur_subgraph[cur_subgraph_size] = ver_arr[i];

		int temp = largest_compelete_subgraph(adj_matrix, cur_subgraph, nabs, nab_count, cur_subgraph_size + 1, largest);

		if(temp > largest){
			largest = temp;
			for(int j = 0; j < temp; ++j){
				subgraph[j] = cur_subgraph[j];
			}
		}
	}

	free(nabs);
	free(cur_subgraph);

	return largest;
}

int alpha_cmp(const void *a, const void *b)
{
	char *str_1 = ((char *) a);
	char *str_2 = ((char *) b);

	if(str_1[0] == str_2[0]){
		return str_1[1] > str_2[1];
	}

	return str_1[0] > str_2[0];
}

int main(void)
{
	char *input_line = NULL;
	size_t lim = 0;

	int ver_lookup[26][26];

	for(int i = 0; i < 26; ++i){
		for(int j = 0; j < 26; ++j){
			ver_lookup[i][j] = -1;
		}
	}

	char adj_matrix[MAX_VER_COUNT][MAX_VER_COUNT];

	for(int i = 0; i < MAX_VER_COUNT; ++i){
		for(int j = 0; j < MAX_VER_COUNT; ++j){
			adj_matrix[i][j] = NEDGE;
		}
	}

	char delim[] = " \n-";
	int ver_count = 0;

	char num_to_ver[MAX_VER_COUNT][3];

	for(int i = 0; i < MAX_VER_COUNT; ++i){
		num_to_ver[i][0] = '\0';
		num_to_ver[i][1] = '\0';
		num_to_ver[i][2] = '\0';
	}

	while(getline(&input_line, &lim, stdin) > 1){
		const char *ver_1 = strtok(input_line, delim);
		const char *ver_2 = strtok(NULL, delim);

		ver_count += add_ver(ver_1, ver_lookup, ver_count);
		ver_count += add_ver(ver_2, ver_lookup, ver_count);

		strcpy(num_to_ver[find_ver(ver_1, ver_lookup)], ver_1);
		strcpy(num_to_ver[find_ver(ver_2, ver_lookup)], ver_2);

		edge temp = {find_ver(ver_1, ver_lookup), find_ver(ver_2, ver_lookup)};
		add_edge(temp, adj_matrix);
	}

	int largest_comp_size = 0;

	int ver_arr[MAX_VER_COUNT];
	int ver_num = 0;

	for(int i = 0; i < 26; ++i){
		for(int j = 0; j < 26; ++j){
			if(ver_lookup[i][j] != -1){
				ver_arr[ver_num] = ver_lookup[i][j];
				++ver_num;
			}
		}
	}

	int subgraph[MAX_VER_COUNT];

	largest_comp_size = largest_compelete_subgraph(adj_matrix, subgraph, ver_arr, ver_num, 0, 0);

	char pass[100];
	pass[0] = '\0';

	for(int i = 0; i < largest_comp_size; ++i){
		strcat(pass, num_to_ver[subgraph[i]]);
		strcat(pass, ",");
	}

	qsort(pass, largest_comp_size, 3, alpha_cmp);
	pass[(largest_comp_size * 3) - 1] = '\0';

	printf("%s\n", pass);

	free(input_line);
	return 0;
}
