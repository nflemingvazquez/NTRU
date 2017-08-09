#include "polyFunctions.h"

int inverseMod(int x, int p) {
	// return multiplicative inverse of x modulo p
	// NB: gcd(x,p)!=1 case not handled
	int r_k1 = x, r_k2 = p, r_k3 = 1;
	// unsigned as r always positive
	int a_k1 = 1, a_k2 = 0, a_k3, q;
	while (r_k3 != 0) {
		q = r_k1 / r_k2;
		r_k3 = r_k1 - q*r_k2;
		r_k1 = r_k2; // increment k by 1
		r_k2 = r_k3;
		a_k3 = a_k1 - q*a_k2;
		a_k1 = a_k2;
		a_k2 = a_k3;
	}
	if (a_k1 < 0) { // a_k1 mod p
		a_k1 += p;
	}
	return (int) a_k1;
}

int * extendedEuclidean(int * x, int * y) {
	return NULL;
}

Poly mononomial(int n)
{
	// return polynomial of form x^n
	int * entries = (int*) calloc(n + 1, sizeof(int)); // array of zeroes
	entries[n] = 1; // set leading coefficient to 1
	Poly result(n, entries);
	// free(entries);
	return result;
}

void polyDiv(Poly x, Poly y, int modulus, Poly* qPtr, Poly* rPtr) {
	// Find q, r in Z[x]/(p) s.t. x=yq+r
	// Output: qPtr, rPtr
	x.mod(modulus);
	y.mod(modulus);
	int m = y.getDegree();
	*rPtr = x;
	*qPtr = Poly(); // reset polynomial dereferenced by qPtr to 0
	int d = rPtr->getDegree();
	Poly v;
	int u = inverseMod(y.getEntry(m), modulus); // invert leading entry of y modp
	if (u == 0) {
		printf("Polynomial division failed!\n");
		exit(-1);
	}
	int counter = 0; // count number of while loops
	while (d >= m && !(rPtr->isZero())) { // continue until r=0 or deg r < deg y
		v = u * rPtr->getEntry(d) * mononomial(d - m);
		*rPtr -= v*y;
		rPtr->mod(modulus);
		d = rPtr->getDegree(); // update value of degree of r
		*qPtr += v;
	}
	qPtr->mod(modulus);
}

void EEA(const Poly a, const Poly b, int p, Poly* uPtr, Poly* vPtr, Poly* dPtr) {
	// Find u, v, d in Z[x]/(p) s.t. a*u+b*v=d
	// Output: uPtr, vPtr, dPtr
	if (b.isZero()) {
		*uPtr = Poly();
		*vPtr = Poly()+1; // v = 1
		*dPtr = a;
	}
	else {
		*uPtr = Poly() + 1; // u = 1
		*dPtr = a;
		Poly v1, t1, t3, q; // initialise as 0
		Poly v3 = b;
		while (!(v3.isZero())) {
			polyDiv(*dPtr, v3, p, &q, &t3); // find q, t3 s.t. d=v3*q+t3
			t1 = *uPtr - q*v1;
			*uPtr = v1;
			*dPtr = v3;
			v1 = t1;
			v3 = t3;
		}
		// NB: no reduceDegree statements because all already implicitly reduced
		polyDiv(*dPtr - a*(*uPtr), b, p, vPtr, &v1); // set v to (d-a*u)/b
		if (!(v1.isZero())) {
			printf("EEA failed!\n");
			exit(-1);
		}
	}
}

void convInverse(const Conv a, int N, int p, Conv * b)
{
	Poly aPoly = (Poly)a;
	Poly ideal = mononomial(N) - 1; // x^N-1
	Poly u, v, d;
	EEA(aPoly, ideal, p, &u, &v, &d);
	if (d.getDegree() == 0 && (!d.isZero())) { // if d non-zero constant
		int d_0 = d.getEntry(0); // d
		Poly result = inverseMod(d_0, p)*u;
		*b = static_cast<Conv&>(result);
	}
	else { // mark as failed
		b = NULL;
	}
}

