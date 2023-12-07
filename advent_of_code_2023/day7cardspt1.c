/*
get list of hands and bets
write algorithm to find had type store it with the hand types eg. full house, two kind ect
sort based on hand types if hand types are the same sort based on order of cards
store number of matching cards in a bit mask it requires 10 bits layed out like so:

number of matching cards: 5 4 3 2 2 1 1 1 1 1
            current hand: 0 0 0 0 0 0 0 0 0 0

store hand types
store card order

note test input pulls evil trick where it gets the same answer if it sorted strongest to weakest or weakest to strongest
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum hand_types{// by weakest to strongest so weaker hand < stronger hand == true
	HIGH_CARD = 1,
	ONE_PAIR,
	TWO_PAIR,
	THREE_OF_KIND,
	FULL_HOUSE,
	FOUR_OF_KIND,
	FIVE_OF_KIND
};

struct cards{//value is set where high value card > lower value card == true
	int card;
	int value;
};

struct cards card_list[] = {
	{'2', 0},
	{'3', 1},
	{'4', 2},
	{'5', 3},
	{'6', 4},
	{'7', 5},
	{'8', 6},
	{'9', 7},
	{'T', 8},
	{'J', 9},
	{'Q', 10},
	{'K', 11},
	{'A', 12}
};

int num_of_cards = sizeof(card_list)/ sizeof(struct cards);

struct hand{
	char *hand;
	int bet;
	int hand_strength;
};

int main(void)
{
	struct hand *hand_list[10000];//list of pointers to hand structs
	char *line = NULL;
	char *hand_parts = NULL;
	size_t lim = 0;
	int hands_index = 0;//1 + index of the last hand
	long int total_winnings = 0;

	int gethandtype(char *);
	int hand_sort_cmp(const void *, const void *);

	while(getline(&line, &lim, stdin) > 1){
		hand_list[hands_index] = (struct hand *) malloc(sizeof(struct hand));//creates space for hands

		hand_parts = strtok(line, " \n");
		hand_list[hands_index]->hand = strdup(hand_parts);//gets hand
		printf("%s\n", hand_list[hands_index]->hand);

		hand_list[hands_index]->hand_strength = gethandtype(hand_list[hands_index]->hand);
		printf("%d\n", hand_list[hands_index]->hand_strength);

		hand_parts = strtok(NULL, " \n");
		hand_list[hands_index]->bet = atoi(hand_parts);
		printf("%d\n", hand_list[hands_index]->bet);

		free(line);
		line = NULL;
		++hands_index;
	}
	free(line);
	line = NULL;

	qsort(hand_list, hands_index, sizeof(struct hand *), hand_sort_cmp);

	for(int i = 0; i < hands_index; ++i){
		printf("%s %d %d\n", hand_list[i]->hand, hand_list[i]->hand_strength, hand_list[i]->bet);
		total_winnings += (hand_list[i]->bet * (i + 1));
	}
	printf("%ld\n", total_winnings);
}


/*
get dupped cards by sanning hand for every card
get number of dupped card types
put info on dup_card_num
use dictionary to convert to hand type
returns hand type
*/
int gethandtype(char *curr_hand)
{
	enum cardnumcount{
		FIVE = 1 << 9,
		FOUR = 1 << 8,
		THREE = 1 << 7,
		TWO_1 = 1 << 6,
		TWO_2 = 1 << 5,
		ONE_1 = 1 << 4,
		ONE_2 = 1 << 3,
		ONE_3 = 1 << 2,
		ONE_4 = 1 << 1,
		ONE_5 = 1
	};

	int dup_card_num = 0;
	int letter_dup = 0;

	for(int i = 0; i < num_of_cards; ++i, letter_dup = 0){//counts dupped cards
		for(int j = 0; curr_hand[j] != '\0'; ++j)
			if(curr_hand[j] == card_list[i].card)
				++letter_dup;

		switch (letter_dup)
		{
		case 5:
			dup_card_num |= FIVE;
			break;
		case 4:
			dup_card_num |= FOUR;
			break;
		case 3:
			dup_card_num |= THREE;
			break;
		case 2:
			if(dup_card_num & TWO_1)
				dup_card_num |= TWO_2;
			else
				dup_card_num |= TWO_1;
		case 0:
			break;
		case 1:
			break;
		default:
			printf("%d error wrong count\n", letter_dup);
			break;
		}
	}

	switch (dup_card_num){//converts dupped card count to hand type
		case FIVE:
			return FIVE_OF_KIND;
		case FOUR:
			return FOUR_OF_KIND;
		case THREE:
			return THREE_OF_KIND;
		case THREE | TWO_1:
			return FULL_HOUSE;
		case TWO_1 | TWO_2:
			return TWO_PAIR;
		case TWO_1:
			return ONE_PAIR;
		default:
			return HIGH_CARD;
	}
}

/*
compare value of hand_strength in struct hand for qsort
if a->hand_strength == b->hand_strength
compare values in hand for bigger value
compare card values by checking if a.hand[j] == b.hand[j] if not compare using numbers on card_list
if a.hand[j] == '\0' return 0
*/
int hand_sort_cmp(const void *a, const void *b)
{
	long int i;
	if((i = ((*((struct hand **)b))->hand_strength - (*((struct hand **)a))->hand_strength)) < 0)
		return 1;
	else if(i > 0)
		return -1;

	int temp_a_card_store = 0;
	int temp_b_card_store = 0;
	for(int j = 0; (*((struct hand **)a))->hand[j] != '\0'; ++j){
		if((*((struct hand **)a))->hand[j] != (*((struct hand **)b))->hand[j]){

			for(int k = 0; k < num_of_cards; ++k){//finds and returns value of card in a.hand[j]
				if((*((struct hand **)a))->hand[j] == card_list[k].card){
					temp_a_card_store = card_list[k].value;
					break;
				}
			}
			for(int k = 0; k < num_of_cards; ++k){//finds and returns value of card in b.hand[j]
				if((*((struct hand **)b))->hand[j] == card_list[k].card){
					temp_b_card_store = card_list[k].value;
					break;
				}
			}

			if(temp_a_card_store > temp_b_card_store){
				return 1;
			}
			else{
				return -1;
			}
		}
	}

	return 0;
}
