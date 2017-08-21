#include "Poly.h"

int inverseMod(int x, int p);
Poly mononomial(int n);
void polyDiv(Poly x, Poly y, int modulus, Poly* qPtr, Poly* rPtr);
void EEA(Poly a, Poly b, int p, Poly* uPtr, Poly* vPtr, Poly* dPtr);
// void convInverse(const Conv a, int N, int p, Conv * b);
bool convPrimeInverse(const Poly a, int N, int p, Poly * b);
bool convPowerInverse(Poly a, int N, int s, int r, Poly * b);