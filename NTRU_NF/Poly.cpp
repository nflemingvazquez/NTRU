#include "Poly.h"

Poly operator+(Poly lhs, const Poly& rhs)
{ // define + operator using +=
	lhs += rhs;
	return lhs;
}

Poly operator+(Poly lhs, int rhs)
{
	lhs += rhs;
	return lhs;
}

Poly operator+(int lhs, Poly & rhs)
{
	rhs += lhs;
	return rhs;
}

Poly operator*(Poly lhs, const Poly &rhs)
{ // define * operator using *=
	lhs *= rhs;
	return lhs;
}

Poly operator*(Poly lhs, int rhs)
{
	lhs *= rhs;
	return lhs;
}

Poly operator*(int lhs, Poly & rhs)
{
	rhs *= lhs;
	return rhs;
}

Poly operator-(Poly lhs, const Poly & rhs)
{
	lhs -= rhs;
	return lhs;
}

Poly operator-(Poly lhs, int rhs)
{
	lhs += -rhs;
	return lhs;
}

Poly operator-(int lhs, Poly & rhs)
{
	rhs = lhs + -rhs;
	return rhs;
}

Poly::Poly(unsigned int a, int * b)
{
	degree = a;
	entries = (int*) calloc(a + 1, sizeof(int));
	memcpy(entries, b, (a + 1) * sizeof(int));
	this->reduceDegree(); // eliminate trailing zeroes
}

int Poly::getEntry(unsigned int n)
{
	// return x^n coefficient of polynomial
	if (n > degree) {
		return 0;
	}
	return entries[n];
}

Poly & Poly::reduceDegree()
{
	// remove trailing zero entries from polynomial
	while (entries[degree] == 0 && degree > 0) {
		degree--;
	}
	int * reduced_arr = (int*)calloc(degree + 1, sizeof(int));
	memcpy(reduced_arr, entries, (degree + 1) * sizeof(int)); // copy non-trailing part
	this->entries = reduced_arr;
	// free(reduced_arr);
	return *this;
}

Poly & Poly::mod(unsigned int p)
{
	for (unsigned int i = 0; i <= degree; ++i) {
		entries[i] %= (int) p; // entry modulo p
		if (entries[i] < 0) { // % returns negative value if argument negative
			entries[i] += (int) p;
		}
	}
	this->reduceDegree();
	return *this;
}

bool Poly::isZero() // return true iff x = 0
{
	if (this->degree == 0 && this->getEntry(0) == 0) {
		return true;
	}
	return false;
}

Poly & Poly::operator+=(const Poly rhs)
{
	unsigned int deg = max(degree, rhs.degree); // degree of sum is maximum of degrees
	int * arr = (int*)calloc(deg + 1, sizeof(int)); // initialise array of zeroes
	for (unsigned int a = 0; a <= degree; ++a) {
		arr[a] += entries[a];
	}
	for (unsigned int b = 0; b <= rhs.degree; ++b) { // add b mononomials to sum
		arr[b] += rhs.entries[b];
	}
	this->degree = deg;
	this->entries = arr;
	this->reduceDegree();
	// free(arr);
	return *this;
}

Poly & Poly::operator+=(int rhs)
{
	entries[0] += rhs;
	return *this;
}

Poly & Poly::operator*=(const Poly rhs)
{
	unsigned int deg = degree + rhs.degree;
	int * arr = (int*)calloc(deg + 1, sizeof(int));
	for (unsigned int i = 0; i <= degree; ++i) {
		for (unsigned int j = 0; j <= rhs.degree; ++j) {
			arr[i + j] += entries[i] * rhs.entries[j];
		}
	}
	degree = deg;
	entries = arr;
	// free(arr); // free memory taken by arr
	return *this;
}

Poly & Poly::operator*=(int rhs)
{
	for (unsigned int i = 0; i <= degree; ++i) { // multiply all entries by rhs
		entries[i] *= rhs;
	}
	return *this;
}

Poly & Poly::operator-() // unary operator to multiply by -1
{
	*this *= -1;
	return *this;
}

Poly & Poly::operator-=(const Poly rhs)
{
	Poly temp = rhs; // introducing this is bad practice
	*this = *this + -temp;
	return *this;
}

void Poly::printValue()
{
	// print e.g. 1+1*x^1+0*x^2+3*x^3
	printf("%d ", entries[0]);
	for (unsigned int i = 1; i <= degree; i++) {
		printf("+ %d x^%u ", entries[i], i);
	}
	printf("\n");
}

Conv operator*(Conv lhs, const Conv & rhs)
{
	lhs *= rhs;
	return lhs;
}

Conv & Conv::operator*=(const Conv rhs) // convolution product
{
	unsigned int deg = max(degree + rhs.degree, (unsigned int) ees_N + 1);
	int * arr = (int*)calloc(deg + 1, sizeof(int));
	for (unsigned int i = 0; i <= degree; ++i) {
		for (unsigned int j = 0; j <= rhs.degree; ++j) {
			arr[i + j % ees_N] += entries[i] * rhs.entries[j];
		}
	}
	degree = deg;
	entries = arr;
	free(arr); // free memory taken by arr	
	return *this;
}
