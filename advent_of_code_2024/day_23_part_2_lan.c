/*
 * this is just a graph theory problem
 * ill store the graph in an edge adjacency matrix
 * to keep track of vertices ill have a 26 by 26 array where each leter in the vertex name is set to a number
 * if there is no entry for that vertex is is assigned a nuber
 * edges will only be added to the adjacency matrix with [lower number vertex][higher number vertex] to avoid double counting
 *
 * find the largest connect component
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

//returns size of connect component
int mark_componets(const int cur_ver, const char adj_matrix[MAX_VER_COUNT][MAX_VER_COUNT], char *comp_arr, int num)
{
	comp_arr[cur_ver] = num;
	int count = 1;

	for(int i = 0; i < MAX_VER_COUNT; ++i){
		if((adj_matrix[cur_ver][i] == EDGE || adj_matrix[i][cur_ver] == EDGE) && comp_arr[i] != num){
			count += mark_componets(i, adj_matrix, comp_arr, num);
		}
	}

	return count;
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

	char seen[MAX_VER_COUNT];

	for(int i = 0; i < MAX_VER_COUNT; ++i){
		seen[i] = 0;
	}



	free(input_line);
	return 0;
}
