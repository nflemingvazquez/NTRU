#include "keyGeneration.h"
#include "polyFunctions.h"
extern "C" {
	#include <sodium.h>
}
#include <time.h>

int main() {
	double time1 = (double)time(NULL);
	if (sodium_init() == -1) { // if LibSodium fails to initialise
		return 1;
	}
	Poly rand = randomConv(100)+1;
	Poly inverse;
	convPowerInverse(rand, ees_N, 2, ees_r, &inverse);
	printf("Program took %f seconds to run \n", time(NULL) - time1);
	return 0;
}


