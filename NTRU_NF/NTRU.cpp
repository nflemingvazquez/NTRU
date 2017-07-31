#include <stdio.h>
#include "keyGeneration.h"
#include "polynomials.h"
extern "C" {
	#include <sodium.h>
}
#include <time.h>
void printpoly(int * poly, int degree);

int main() {
	double time1 = (double) time(NULL);
	if (sodium_init() == -1) { // if LibSodium fails to initialise
		return 1;
	}
	// test Poly class
	int a[2] = { 1,10 };
	int b[3] = { 2,1, 0 };
	Poly poly1(1, a);
	Poly poly2(2, b);
	Poly poly3 = poly1 - poly2;
	poly3.printValue();
	printf("Program took %f seconds to run \n", time(NULL) - time1);
	return 0;
}


