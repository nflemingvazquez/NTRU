#include "Poly.h"

int inverseMod(int x, int p);
int * extendedEuclidean(int * x, int * y);
Poly mononomial(int n);
void polyDiv(const Poly x, const Poly y, int modulus, Poly* qPtr, Poly* rPtr);
void EEA(const Poly a, const Poly b, int p, Poly* uPtr, Poly* vPtr, Poly* dPtr);
void convInverse(const Conv a, int N, int p, Conv * b);
