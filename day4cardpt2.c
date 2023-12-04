/*
take number input from : to | for the test numbers
append -1 to the end of the list as end of list marker
and take input from | to \n
liner search through each to fine matches
put the number of matches in a struct
calculate the point mutpliers
*/

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

struct carddata{
	int match_number;
	int extra_cards;
};

struct carddata card_array[5000];

int main(void)
{
	void printintarr(int *);
	int getnumbers(int *);
	int linsearch(int *, int *);
	int totalpointcalc(void);
	int total_card_value = 0;
	int winning_numbers[100];
	int your_numbers[100];
	int c;
	int number_of_matches = 0;
	int card_index = 0;

	while((c = getchar()) != EOF){
		if(c == ':'){
			getnumbers(winning_numbers);
			getnumbers(your_numbers);

//			printf("\n");

//			printintarr(winning_numbers);
//			printintarr(your_numbers);

			number_of_matches = linsearch(winning_numbers, your_numbers);
//			printf("\n%d\n", number_of_matches);

			card_array[card_index].match_number = number_of_matches;
			card_array[card_index].extra_cards = 1;

			++card_index;

			winning_numbers[0] = -1;
			your_numbers[0] = -1;
			number_of_matches = 0;
		}
	}

	card_array[card_index].match_number = -1;
	card_array[card_index].extra_cards = -1;

	total_card_value = totalpointcalc();


	printf("%d\n", total_card_value);
	return 0;
}


/*
gets the numbers return 0 for success 1 for error appends 1 to the end of the list
append -1 to end of list
*/
int getnumbers(int *s)
{
	char number_buf[10] = {"\0"};
	int c;
	int i = 0;
	int j = 0;//inc for array
	while((c = getchar()) != '|' && c != '\n' && c != EOF){
		if(isdigit(c)){
			number_buf[i++] = c;
			number_buf[i] = '\0';
		}
		else if(number_buf[0] != '\0'){

			number_buf[i] = '\0';
			*(s + j++) = atoi(number_buf);
			i = 0;
			number_buf[0] = '\0';
		}

	}
	if(number_buf[0] != '\0'){
		number_buf[i] = '\0';
		*(s + j++) = atoi(number_buf);
		i = 0;
	}
	*(s + j) = -1;
	return 0;
}

/*
liner searches for matches int strings returns ammount of matches
*/
int linsearch(int *a, int *b)
{
	int total_found = 0;
	for(int i = 0; *(a + i) != -1; ++i)
		for(int j = 0; *(b + j) != -1; ++j){
			if(*(b + j) == *(a + i))
				++total_found;
		}
	return total_found;
}

/*
first add extra_cards to the card count
reach to card_index + 1 ... card_index + match_number
increase each of the extra_cards of the later cards by extra_cards of this card
returns total card count
*/
int totalpointcalc(void)
{
	int card_index = 0;
	int total_value = 0;

	while(card_array[card_index].match_number != -1){
		total_value += (card_array[card_index].extra_cards);
		for(int i = 1; (i - 1) != card_array[card_index].match_number; ++i)
			card_array[card_index+i].extra_cards += card_array[card_index].extra_cards;

		printf("%d %d\n", total_value,  (card_array[card_index].extra_cards));
		++card_index;
	}
	return total_value;
}


void printintarr(int *a)
{
	for(int i = 0; *(a + i) != -1; ++i)
		printf("%d ", *(a + i));
	printf("\n");
}
