#include "keyGeneration.h"

Conv randomConv(int df) {
	// Create random degree N trinary polynomial with df coefficients 1
	if (2 * df > ees_N) { // too many indices would be non-zero
		printf("df too large!");
		exit(-1);
	}
	int * entries = (int*) calloc(ees_N, sizeof(int)); // array of zeroes
	int index;
	int t = 0;
	while (t < df) { // make df distinct coefficients 1
		index = randombytes_uniform(ees_N); // index random; 0 <= index < RingDegree
		if (entries[index] == 0) {
			entries[index] = 1;
			t++;
		}
	}
	t = 0;
	while (t < df) { // make df other coefficients -1
		index = randombytes_uniform(ees_N); // index random; 0 <= index < RingDegree
		if (entries[index] == 0) {
			entries[index] = -1;
			t++;
		}
	}
	Conv result(ees_N - 1, entries);
	return result;
}

