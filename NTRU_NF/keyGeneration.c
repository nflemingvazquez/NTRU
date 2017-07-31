#include "keyGeneration.h"

int * randomPoly(unsigned int df) {
	// Create random degree N trinary polynomial with df coefficients 1
	if (2 * df > RingDegree) { // too many indices would be non-zero
		return NULL;
	}
	int * poly = calloc(RingDegree, sizeof(int)); // initialise zero polynomial
	int index;
	unsigned int t = 0;
	while (t < df) { // make df distinct coefficients 1
		index = randombytes_uniform(RingDegree); // index random; 0 <= index < RingDegree
		if (poly[index] == 0) {
			poly[index] = 1;
			t++;
		}
	}
	t = 0;
	while (t < df) { // make df other coefficients -1
		index = randombytes_uniform(RingDegree); // index random; 0 <= index < RingDegree
		if (poly[index] == 0) {
			poly[index] = -1;
			t++;
		}
	}
	return poly;
}