#include "polynomials.h"
#include <sodium.h>

int main() {
	if (sodium_init() == -1) { // if LibSodium fails to initialise
		return 1;
	}
	int test1[RingDegree] = { 1 }; // x^N
	int* test2 = polyProduct(test1,test1);
	unsigned int bytes = (unsigned int)randombytes_uniform(RingDegree);
	printf("Random bytes are %u \n", bytes);

	return 0;
}