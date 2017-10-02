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

Poly mononomial(int n)
{
	// return polynomial of form x^n
	int * entries = (int*) calloc(n + 1, sizeof(int)); // array of zeroes
	entries[n] = 1; // set leading coefficient to 1
	Poly result(n, entries);
	free(entries);
	return result;
}

void polyDiv(Poly x, Poly y, int modulus, Poly* qPtr, Poly* rPtr) {
	// Find q, r in Z[x]/(p) s.t. x=yq+r
	// Output: qPtr, rPtr
	x %= modulus;
	y %= modulus;
	int m = y.getDegree();
	Poly r = x;
	Poly q;
	int d = r.getDegree();
	Poly v;
	int u = inverseMod(y.getEntry(m), modulus); // invert leading entry of y modp
	if (u == 0) {
		printf("Polynomial division failed!\n");
		exit(-1);
	}
	int counter = 0; // count number of while loops
	while (d >= m && !(r.isZero())) { // continue until r=0 or deg r < deg y
		v = u * r.getEntry(d) * mononomial(d - m);
		r -= v*y;
		r %= modulus;
		d = r.getDegree(); // update value of degree of r
		q += v;
	}
	q %= modulus;
	*rPtr = r;
	*qPtr = q;
}

bool primeAlmostInverse(Poly a, Poly * bPtr) {
	if (a.isZero()) return false;
	int k = 0;
	Poly b;
	b += 1; // b=1
	Poly c;
	Poly f = a;
	Poly g = mononomial(ees_N) - 1;
	while (1) {
		while (f.getEntry(0) == 0) {
			f.shift(-1);
			c.shift(1);
			k++;
		}
		if (f.getDegree()==0) {
			b *= inverseMod(f.getEntry(0), ees_p);
			b.shift(ees_N - k);
			b %= ees_p;
			*bPtr = b;
			return true;
		}
		if (f.getDegree() < g.getDegree()) {
			swap(f, g);
			swap(b, c);
		}
		int u = f.getEntry(0)*inverseMod(g.getEntry(0), ees_p);
		f -= u*g;
		f %= ees_p;
		b -= u*c;
		b %= ees_p;
	}
}

void EEA(Poly a, Poly b, int p, Poly* uPtr, Poly* vPtr, Poly* dPtr) {
	// Find u, v, d in Z[x]/(p) s.t. a*u+b*v=d
	// Output: uPtr, vPtr, dPtr
	if (b.isZero()) { // a*1+0*0=a
		*uPtr = Poly()+1; // u = 1
		*vPtr = Poly(); // v = 0
		*dPtr = a;
	}
	else {
		Poly u = Poly() + 1; // u = 1
		Poly d = a;
		Poly v1, t1, t3, q; // initialise as 0
		Poly v3 = b;
		while (!(v3.isZero())) {
			polyDiv(d, v3, p, &q, &t3); // find q, t3 s.t. d=v3*q+t3
			t1 = u - q*v1;
			u = v1;
			d = v3;
			v1 = t1;
			v3 = t3;
			v1 %= p;
			t1 %= p;
			t3 %= p;
			v3 %= p;
		}
		// NB: no reduceDegree statements because all already implicitly reduced
		polyDiv(d-a*u, b, p, vPtr, &v1); // set v to (d-a*u)/b
		if (!(v1.isZero())) { // if d-a*u not divisible by b
			printf("EEA failed!\n a*u+b*v-d is: ");
			Poly residue = a*(*uPtr) + b*(*vPtr) - *dPtr;
			residue %= p;
			residue.printValue();
			exit(-1);
		}
		else { // output
			*uPtr = u;
			*dPtr = d;
		}
	}
}

bool convPrimeInverse(const Poly a, int N, int p, Poly * b)
{
	Poly ideal = mononomial(N) - 1; // x^N-1
	Poly u, v, d;
	EEA(a, ideal, p, &u, &v, &d);
	if (d.getDegree() == 0 && (!d.isZero())) { // if d non-zero constant
		int d_0 = d.getEntry(0); // d
		Poly result = inverseMod(d_0, p)*u;
		// *b = static_cast<Conv&>(result);
		result %= p;
		*b = result;
		return true;
	}
	else { // mark as failed
		b = NULL;
		return false;
	}
}

bool convPowerInverse(Poly a, int N, int s, int r, Poly * bPtr)
{
	/* find inverse of a in R/(s^r), 
	NB: modified according to https://assets.onboardsecurity.com/static/downloads/NTRU/resources/NTRUTech014.pdf */
	Poly b;
	int q =  (int) pow(s, r); // recalculate q from s, r
	if (convPrimeInverse(a, N, s ,&b) == false) { // if a not invertible in R/(q)
		return false;
	}
	int t = s;
	while (t < q) {
		t *= t; // square t
		b = b*(2 - a*b);
		b.convolute();
		b %= t;
	}
	*bPtr = b;
	if (b.isZero()) return false;
	return true;
}

