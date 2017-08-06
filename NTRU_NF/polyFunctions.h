#include "Poly.h"

unsigned int inverseMod(unsigned int x, unsigned int p);
int * extendedEuclidean(int * x, int * y);
Poly mononomial(unsigned int n);
void polyDiv(Poly x, Poly y, unsigned int modulus, Poly* qPtr, Poly* rPtr);
void EEA(Poly a, Poly b, unsigned int p, Poly* uPtr, Poly* vPtr, Poly* dPtr);

