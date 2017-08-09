#include "keyGeneration.h"
#include "polyFunctions.h"
extern "C" {
	#include <sodium.h>
}
#include <time.h>

int main() {
	double time1 = (double) time(NULL);
	if (sodium_init() == -1) { // if LibSodium fails to initialise
		return 1;
	}
	Conv rand = randomConv(200);
	Conv inverse;
	 rand.printValue();
	 for (int t = 0; t < 10; ++t) {
		 printf("%d\n", rand.getEntry(t));
	 }
	convInverse(rand,20,5,&inverse);
	for (int t = 0; t < 10; ++t) {
		printf("Now %d\n", rand.getEntry(t));
	}
	inverse.printValue();
	(rand*inverse).printValue();
	Poly test1 = (Conv)rand;
	Poly test2 = inverse;
	printf("Test 1 is "); test1.printValue();
	Conv test3 = static_cast<Conv&>(test1);
	printf("Test 3 is "); test3.printValue();
	Conv a = rand * inverse;
	// printf("a is: "); a.printValue();
	printf("Program took %f seconds to run \n", time(NULL) - time1);
	return 0;
}


