#include "polynomials.h"

int * polyProduct(int x[], int y[]) {
	// return product of 2 polynomials in R
	int * product = calloc(RingDegree, sizeof(int)); // create array of zeroes
	for (int i = 0; i < RingDegree; ++i) {
		for (int j = 0; j < RingDegree; ++j) {
			product[(i + j) % RingDegree] += x[i] * y[j];
		}
	}

	return product;
}

int * polyMod(int x[], unsigned int modulo) {
	// compute polynomial corresponding to x in R/modulo
	int * result = calloc(RingDegree, sizeof(int));
	for (int i = 0; i < RingDegree; ++i) result[i] = x[i] % modulo;
	return result;
}

unsigned int inverseMod(unsigned int x, unsigned int p) {
	// return multiplicative inverse of x modulo p
	// NB: gcd(x,p)!=1 case not handled
	unsigned int r_k1 = x, r_k2 = p, r_k3 = 1;
	// unsigned as r always positive
	int a_k1 = 1, a_k2 = 0, a_k3, q;
	while (r_k3 != 0) {
		q = r_k1 / r_k2;
		r_k3 = r_k1 - q*r_k2;
		r_k1 = r_k2; // increment k by 1
		r_k2 = r_k3;
		a_k3 = a_k1 - q*a_k2;
		a_k1 = a_k2;
		a_k2 = a_k3;
	}
	if (a_k1 < 0) { // a_k1 mod p
		a_k1 += p;
	}
	return (unsigned int) a_k1;
}

int * extendedEuclidean(int * x, int * y) {
	return NULL;
}

int * polyDivision(int x[], int y[], unsigned int p) {
	// Find q, r in Z[x]/p s.t. x=yq+r
	return NULL;
}