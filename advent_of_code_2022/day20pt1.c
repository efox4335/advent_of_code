/*

*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct node{
	long int num;
	struct node *next;
	struct node *past;
}node;

node order_arr[6000];
node *zero_node = NULL;
int arr_size = 0;

int main(void)
{
	void print_ll(node *);
	void move_node(node *, node *);
	node *find_dest(node *);

	char *line = NULL;
	size_t lim;
	node *dest;
	long int place1000;
	long int place2000;
	long int place3000;

	while(getline(&line, &lim, stdin) > 0){
		order_arr[arr_size].num = atol(line);

	//	printf("%ld\n", order_arr[arr_size].num);

		if(order_arr[arr_size].num == 0)
			zero_node = &order_arr[arr_size];
		++arr_size;
	}

	for(int i = 1; i < arr_size - 1; ++i){
		order_arr[i].next = &order_arr[i + 1];
		order_arr[i].past = &order_arr[i - 1];
	}

	order_arr[0].past = &order_arr[arr_size - 1];
	order_arr[arr_size - 1].next = &order_arr[0];
	order_arr[0].next = &order_arr[1];
	order_arr[arr_size - 1].past = &order_arr[arr_size - 2];

	print_ll(zero_node);

	//move_node(&order_arr[1], &order_arr[4]);

	printf("%p\n", zero_node);
	printf("\n\n\n");

	for(int i = 0; i < arr_size; ++i){
		if(order_arr[i].num == 0)
			continue;
		printf("%p\n", &order_arr[i]);
		dest = find_dest(&order_arr[i]);

		move_node(&order_arr[i], dest);

//		print_ll(zero_node);
//		printf("\n");
	}


	dest = zero_node;
	for(int i = 0; i < 1000; ++i){
		dest = dest->next;
	}

	place1000 = dest->num;

	dest = zero_node;
	for(int i = 0; i < 2000; ++i){
		dest = dest->next;
	}

	place2000 = dest->num;

	dest = zero_node;
	for(int i = 0; i < 3000; ++i){
		dest = dest->next;
	}

	place3000 = dest->num;

	print_ll(zero_node);

	printf("\n\n %ld %ld %ld\n", place1000, place2000, place3000);

	printf("\n%ld\n", place1000 + place2000 + place3000);

	printf("%d\n", arr_size);

	return 0;
}

//takes the node to move and the new pos
void move_node(node *mv_node, node *dest)
{
	if(mv_node == dest)
		return;

	node *un_next = mv_node->next;
	node *un_prev = mv_node->past;

	un_next->past = un_prev;
	un_prev->next = un_next;

	node *new_next = dest->next;

	dest->next = mv_node;
	new_next->past = mv_node;

	mv_node->next = new_next;
	mv_node->past = dest;
}

node *find_dest(node *np){
	node *dest = np;

	if(np->num > 0){//for positive
		for(int i = 0; i < np->num; ++i){
			dest = dest->next;
			if(dest == np)
				dest = dest->next;
		}
	}
	else{
		for(int i = 0; i < labs(np->num); ++i){
			dest = dest->past;
			if(dest == np)
				dest = dest->past;
		}

		dest = dest->past;
	}

	printf("%p\n", dest);

	return dest;
}

void print_ll(node *st)
{
	for(int i = 0; i < arr_size; ++i){
		printf("%ld   %p\n", st->num, st);
		st = st->next;
	}
}
