/*
 * extended euclidian algrorithm
 * returns answer in form gcd = a_cof * a + b_cof * b
 * can be used to find modular inverse
 * if the coefficient for the desired value is negative (eg trying to find a^-1 mod b but a_cof < 0) a definition of mod where
 * a = kb + |a mod b| must be used
 * where |k| is the smallest value that satisfies kb < a
 * if a is negative add b till a is positive
 *
 * the mod operator in c just ignores the sign
 * to convert to a value that they both agree on add the number that you are modulating by to the other number corfficient till it is positive
 *
 * eg if you want to find a^-1 mod b and a_cof < 0
 * a_cof += b till a_cof >= 0
*/
long ext_euclid(long a, long b, long *a_cof, long *b_cof)
{
	long gcd = 0;

	if(b == 0){
		*a_cof = 1;
		*b_cof = 0;
		return a;
	}

	gcd = ext_euclid(b, a % b, a_cof, b_cof);
	long temp = *b_cof;
	*b_cof = *a_cof - (*b_cof * (a/b));
	*a_cof = temp;

	return gcd;
}
