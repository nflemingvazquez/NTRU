#include <iostream>
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
	int arr1[] = { 1,-2,1 };
	int arr2[] = { 1,2,1 };
	Poly p1(2, arr1);
	p1.printValue();
	p1.mod(5);
	p1.printValue();
	Poly p2(10, arr2);
	Poly u, v, d;
	EEA(p1, p2, 3, &u, &v, &d);
	printf("Program took %f seconds to run \n", time(NULL) - time1);
	return 0;
}


