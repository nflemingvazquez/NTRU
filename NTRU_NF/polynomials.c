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