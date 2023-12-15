/*
take input apply the hashing algorithm
hash of string is current_hash = ((current_hash + char)*17) % 256 for each char in string where current hash starts at 0
print the sum of all string hashes
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	char *line = NULL;
	char *hash_string;
	size_t lim = 0;
	long int hash_sum = 0;
	long int str_hash = 0;

	getline(&line, &lim, stdin);

	hash_string = strtok(line, ",\n");

	do{
		str_hash = 0;
		for(int i = 0; hash_string[i] != '\0'; ++i){
			str_hash += hash_string[i];
			str_hash *= 17;
			str_hash %= 256;
		}

		hash_sum += str_hash;

	}while ((hash_string = strtok(NULL, ",\n")) != NULL);

	printf("%ld\n", hash_sum);
	return 0;
}
