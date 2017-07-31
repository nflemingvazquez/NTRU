#include <stdio.h>
#include "keyGeneration.h"
#include "polynomials.h"
#include <sodium.h>
#include <time.h>
void printpoly(int * poly, int degree);

int main() {
	if (sodium_init() == -1) { // if LibSodium fails to initialise
		return 1;
	}
	int * testpoly = randomPoly(20);
	printpoly(testpoly, RingDegree);
	return 0;
}

void printpoly(int * poly, int degree)
{
	for (int i = 0; i < degree; ++i) {
		printf("%d, ", poly[i]);
	}
	printf("\n");
}
