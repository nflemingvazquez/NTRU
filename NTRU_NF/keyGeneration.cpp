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
	//sodium_memzero(arr, ees_N * sizeof(int));
	free(arr);
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

void generateKeyPair(Poly * fPtr, Poly * hPtr, PolyTriple * FTriPtr)
{
	Poly f, f_q, g, g_q, h;
	PolyTriple triple;
	bool fInvertible = false;
	while (fInvertible == false) { // continue until invertible polynomial generated
		triple = randomTriple(ees_df1,ees_df2,ees_df3);
		Poly F(triple);
		f = 1 + ees_p*F;
		f.convolute();
		f %= ees_q;
		fInvertible = convPowerInverse(f, ees_N, ees_s, ees_r, &f_q);
	}
	bool gInvertible = false;
	while (gInvertible == false) {
		g = randomConv(ees_dg) + 1;
		gInvertible = convPowerInverse(g, ees_N, ees_s, ees_r, &g_q);
	}
	*fPtr = f;
	h = f_q*g*ees_p;
	h.convolute();
	h %= ees_q;
	*fPtr = f;
	*hPtr = h;
	*FTriPtr = triple;
}

