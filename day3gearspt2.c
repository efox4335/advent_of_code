/*
import inpu into a multi dimentional array that represents a cord plane
go through finding symbols
when a symbol is found call a function to get all numbers and put them on an array
add all numbers on the array and print
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int part_values[10000];
int part_index = 0;

char scme_rep[1000][1000];

struct syminfo{
	int x;
	int y;
};

struct syminfo sym_array[10000];
int sym_index = 0;


int main(void)
{
	int c;
	int x_cord = 0;
	int y_cord = 0;
	int totalnum = 0;
	void findscmenums(void);

	while((c = getchar()) != EOF){
		scme_rep[y_cord][x_cord] = c;

		if(c != '.' && ispunct(c)){
			sym_array[sym_index].x = x_cord;
			sym_array[sym_index].y = y_cord;
			++sym_index;
		}
		++x_cord;
		if(c == '\n'){
			scme_rep[y_cord][x_cord] = '\0';
			++y_cord;
			x_cord = 0;
		}
	}

	scme_rep[y_cord][x_cord + 1] = 'a';

	findscmenums();

	for(int i = 0; i != part_index; ++i)
		totalnum += part_values[i];

	printf("%d\n", totalnum);


/*	for(int i = 0; i != y_cord; i++)
		printf("%s", scme_rep[i]);

	for(int i = 0; i != sym_index; ++i)
		printf("(%d, %d): %c\n", sym_array[i].x, sym_array[i].y, scme_rep[sym_array[i].y][sym_array[i].x]);*/
	return 0;
}

//calls getnum on each of the eight places around a symbol makes sure above and below aren't touching to avoid double adding more then one part value

int pot_gear_storage[10];
int pot_gear_arr_index = 0;

void findscmenums(void)
{
	void getnum(int, int);
	for(int i = 0; i != sym_index; ++i){
		pot_gear_arr_index = 0;
		getnum(sym_array[i].x - 1, sym_array[i].y);
		getnum(sym_array[i].x + 1, sym_array[i].y);

		if(isdigit(scme_rep[sym_array[i].y + 1][sym_array[i].x])){

			getnum(sym_array[i].x, sym_array[i].y + 1);
		}
		else{
			getnum(sym_array[i].x + 1, sym_array[i].y + 1);
			getnum(sym_array[i].x - 1, sym_array[i].y + 1);
		}

		if(isdigit(scme_rep[sym_array[i].y - 1][sym_array[i].x])){
			getnum(sym_array[i].x, sym_array[i].y - 1);
		}
		else{
			getnum(sym_array[i].x + 1, sym_array[i].y - 1);
			getnum(sym_array[i].x - 1, sym_array[i].y - 1);
		}
		if(pot_gear_arr_index == 2){
			part_values[part_index++] = pot_gear_storage[0] * pot_gear_storage[1];
		}
	}
}

//gets full number from any cord with part in it (x, y)
void getnum(int x_cord, int y_cord){
	if(!isdigit(scme_rep[y_cord][x_cord]))
		return;

	char numstorage[100];
	int i = 0;

	while(x_cord > 0 && isdigit(scme_rep[y_cord][x_cord - 1]))
		--x_cord;

	while(isdigit(scme_rep[y_cord][x_cord])){
		numstorage[i] = scme_rep[y_cord][x_cord];
		++x_cord;
		++i;
	}
	numstorage[i] = '\0';
	pot_gear_storage[pot_gear_arr_index++] = atoi(numstorage);
}
