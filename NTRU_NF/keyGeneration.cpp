#include "keyGeneration.h"

Poly randomConv(int df) {
	// Create random degree N trinary polynomial with df coefficients 1
	if (2 * df > ees_N) { // too many indices would be non-zero
		printf("df too large!");
		exit(-1);
	}
	int * arr = (int*) calloc(ees_N, sizeof(int)); // array of zeroes
	int index;
	int t = 0;
	while (t < df) { // make df distinct coefficients 1
		index = randombytes_uniform(ees_N); // index random; 0 <= index < RingDegree
		if (arr[index] == 0) {
			arr[index] = 1;
			t++;
		}
	}
	t = 0;
	while (t < df) { // make df other coefficients -1
		index = randombytes_uniform(ees_N); // index random; 0 <= index < RingDegree
		if (arr[index] == 0) {
			arr[index] = -1;
			t++;
		}
	}
	Poly result(ees_N - 1, arr);
	return result;
}

PolyTriple randomTriple(int df1, int df2, int df3)
{
	PolyTriple result;
	result.a1 = randomConv(df1);
	result.a2 = randomConv(df2);
	result.a3 = randomConv(df3);
	return result;
}

void generateKeyPair(int N, int p, int s, int r, int q, int df1, int df2, int df3, int dg, Poly * fPtr, Poly * hPtr, PolyTriple * FTriPtr)
{
	Poly f, F, f_q, g, g_q, h;
	PolyTriple tri;
	bool fInvertible = false;
	while (fInvertible == false) { // continue until invertible polynomial generated
		tri = randomTriple(df1,df2,df3);
		Poly F(tri);
		f = 1 + p*F;
		f.convolute();
		f %= q;
		fInvertible = convPowerInverse(f, N, s, r, &f_q);
	}
	bool gInvertible = false;
	while (gInvertible == false) {
		g = randomConv(dg) + 1;
		gInvertible = convPowerInverse(g, N, s, r, &g_q);
	}
	*fPtr = f;
	h = f_q*g*p;
	h.convolute();
	h %= q;
	*fPtr = f;
	*hPtr = h;
	*FTriPtr = tri;
}

