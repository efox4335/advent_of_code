#include <stddef.h>

//euclids algorithm
size_t gcd(size_t a, size_t b)
{
	while(1){
		a %= b;

		if(a == 0){
			a = b;
			break;
		}

		b %= a;

		if(b == 0){
			break;
		}
	}

	return a;
}
