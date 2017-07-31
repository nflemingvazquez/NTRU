#include "polynomials.h"

int * convProduct(int x[], int y[]) {
	// return convoluted product of 2 polynomials in R
	int * product = (int*) calloc(RingDegree, sizeof(int)); // create array of zeroes
	for (int i = 0; i < RingDegree; ++i) {
		for (int j = 0; j < RingDegree; ++j) {
			product[(i + j) % RingDegree] += x[i] * y[j];
		}
	}

	return product;
}

int * polyMod(int x[], unsigned int modulo) {
	// compute polynomial corresponding to x in R/modulo
	int * result = (int*) calloc(RingDegree, sizeof(int));
	for (int i = 0; i < RingDegree; ++i) result[i] = x[i] % modulo;
	return result;
}

unsigned int inverseMod(unsigned int x, unsigned int p) {
	// return multiplicative inverse of x modulo p
	// NB: gcd(x,p)!=1 case not handled
	unsigned int r_k1 = x, r_k2 = p, r_k3 = 1;
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
	return (unsigned int) a_k1;
}

int * extendedEuclidean(int * x, int * y) {
	return NULL;
}

int * polyDivision(Poly x, Poly y, unsigned int p) {
	// Find q, r in Z[x]/p s.t. x=yq+r
	Poly r = x;
	Poly q(0, { 0 });
	unsigned int u = inverseMod(q.entries[q.degree], p);
	while (r.degree >= y.degree) {
		
	}
	return NULL;
}

Poly Poly::operator+(const Poly & param)
{
	// class used to handle polynomials in Z[x]
	Poly sum;
	unsigned int deg = max(degree, param.degree); // degree of sum is maximum of degrees
	int * arr = (int*)calloc(deg+1, sizeof(int)); // initialise array of zeroes
	for (unsigned int a = 0; a <= degree; ++a) {
		arr[a] += entries[a];
	}
	for (unsigned int b = 0; b <= param.degree; ++b) { // add b mononomials to sum
		arr[b] += param.entries[b];
	}
	while (arr[deg] == 0 && deg > 0) { // trailing zeroes
		deg--;
	}
	int* reduced_arr = (int*)calloc(deg+1, sizeof(int));
	memcpy(reduced_arr, arr, (deg + 1) * sizeof(int)); // copy non-trailing part
	sum.degree = deg;
	sum.entries = arr;
	return sum;
}

Poly Poly::operator-(const Poly & param)
{
	Poly tosubtract;
	int * arr = (int*)calloc(param.degree + 1, sizeof(int));
	for (unsigned int i = 0; i <= param.degree; ++i) { // set value of arr to -(param.entries)
		arr[i] = -param.entries[i];
	}
	tosubtract.degree = param.degree;
	tosubtract.entries = arr;
	return *this+tosubtract;
}

Poly Poly::operator*(const Poly & param)
{
	Poly product;
	unsigned int deg = degree + param.degree;
	int * arr = (int*)calloc(deg + 1, sizeof(int));
	for (unsigned int i = 0; i <= degree; ++i) {
		for (unsigned int j = 0; j <= param.degree; ++j) {
			arr[i + j] += entries[i] * param.entries[j];
		}
	}
	product.degree = deg;
	product.entries = arr;
	return product;
}

void Poly::printValue()
{
	// print e.g. 1+1*x^1+0*x^2+3*x^3
	printf("%d ", entries[0]);
	for (unsigned int i = 1; i <= degree; i++) {
		printf("+ %d x^%d ", entries[i], i);
	}
	printf("\n");
}
