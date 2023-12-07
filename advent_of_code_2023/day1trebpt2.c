/*
walk each line getting the first and last number on each line when i find the first number make sure to set it to both the first and last
bug fails on strings like threeight
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char lookforlist[][50] = {
	{"one"},
	{"two"},
	{"three"},
	{"four"},
	{"five"},
	{"six"},
	{"seven"},
	{"eight"},
	{"nine"},
	{"0"},
	{"1"},
	{"2"},
	{"3"},
	{"4"},
	{"5"},
	{"6"},
	{"7"},
	{"8"},
	{"9"}
};

int main(void)
{
	char firstnum[5000];
	char lastnum[5000];
	char line[1000];
	int running_total = 0;
	int c;
	int i = 0;
	int linecount = 1;
	int tempstore;

	firstnum[0] = '\0';
	lastnum[0] = '\0';

	int findstring(char *, char);
	char find_latest(char *);

	while((c = getchar()) != EOF){
		line[i++] = c;
		line[i] = '\0';
	//	printf("%d ", c);
		if(isdigit(tempstore = find_latest(line))){//put numbers on char string for later conversion
		//	printf("%s\n", line);
			if(firstnum[0] == '\0'){
				tempstore = findstring(line, c);
				printf("%c\n", tempstore);
				firstnum[0] = tempstore;
				lastnum[0] = tempstore;
				lastnum[1] = '\0';
				firstnum[1] = '\0';
			}
			else{
				lastnum[0] = tempstore;
				lastnum[1] = '\0';
			}
		//	i = 0;
		//	line[0] = '\0';
		}

		if(c == 10 || c == EOF ){
			printf("%d: %d\n", linecount, (atoi(firstnum) * 10) + atoi(lastnum));
			running_total += (atoi(firstnum) * 10) + atoi(lastnum);
			printf("%d: %d\n", linecount++, running_total);
			firstnum[0] = '\0';
			lastnum[0] = '\0';
			i = 0;
			line[0] = '\0';
		}
	}
	printf("\n");
	printf("%d\n", running_total);
	return 0;
}

char find_latest(char *s)
{
	int findstring(char *s, char c);
	char *store;
	char *pot;
	char tempval = 0;
	store = NULL;
	pot = NULL;
	for(int i = 0; i < 19; ++i){
		pot = strstr(s, lookforlist[i]);
		if(store == NULL)
			store = pot;
		else if(pot > store)
			store = pot;
	}
	if(store != NULL)
		if((tempval = find_latest(store + 1)) == 0){
			if(isdigit(*store))
				return *store;
			else
				return findstring(store, 1);
		}
		else
			return tempval;
	else
		return 0;

}

int findstring(char *s, char c)
{
//	if(strstr(s, "zero") != NULL)
//		return '0';
	if(strstr(s, "one") != NULL){
		return '1';
	}
	if(strstr(s, "two") != NULL){
		return '2';
	}
	if(strstr(s, "three") != NULL){
		return '3';
	}
	if(strstr(s, "four") != NULL){
		return '4';
	}
	if(strstr(s, "five") != NULL){
		return '5';
	}
	if(strstr(s, "six") != NULL){
		return '6';
	}
	if(strstr(s, "seven") != NULL){
		return '7';
	}
	if(strstr(s, "eight") != NULL){
		return '8';
	}
	if(strstr(s, "nine") != NULL){
		return '9';
	}
	return c;
}
