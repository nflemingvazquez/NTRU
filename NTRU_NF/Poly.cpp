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

Poly::Poly(int a, int * b)
{
	degree = a;
	entries = (int*) calloc(a + 1, sizeof(int));
	memcpy(entries, b, (a + 1) * sizeof(int));
	this->reduceDegree(); // eliminate trailing zeroes
}

int Poly::getEntry(int n) const
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

Poly & Poly::mod(int p)
{
	for (int i = 0; i <= degree; ++i) {
		entries[i] %= (int) p; // entry modulo p
		if (entries[i] < 0) { // % returns negative value if argument negative
			entries[i] += (int) p;
		}
	}
	this->reduceDegree();
	return *this;
}

bool Poly::isZero() const // return true iff x = 0
{
	if (degree == 0 && getEntry(0) == 0) {
		return true;
	}
	return false;
}

Poly & Poly::operator+=(const Poly rhs)
{
	int deg = max(degree, rhs.degree); // degree of sum is maximum of degrees
	int * arr = (int*)calloc(deg + 1, sizeof(int)); // initialise array of zeroes
	for (int a = 0; a <= degree; ++a) {
		arr[a] += entries[a];
	}
	for (int b = 0; b <= rhs.degree; ++b) { // add b mononomials to sum
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
	if (isZero() || rhs.isZero()) { // avoid returning 0x+0x^1+...
		*this = Poly();
		return *this;
	}
	int deg = degree + rhs.degree;
	int * arr = (int*)calloc(deg + 1, sizeof(int));
	for (int i = 0; i <= degree; ++i) {
		for (int j = 0; j <= rhs.degree; ++j) {
			arr[i + j] += entries[i] * rhs.entries[j];
		}
	}
	degree = deg;
	entries = arr;
	return *this;
}

Poly & Poly::operator*=(int rhs)
{
	for (int i = 0; i <= degree; ++i) { // multiply all entries by rhs
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
	for (int i = 1; i <= degree; i++) {
		printf("+ %d x^%u ", entries[i], i);
	}
	printf("\n");
}

Conv operator*(Conv lhs, const Conv &rhs)
{
	lhs *= rhs;
	return lhs;
}

Conv & Conv::operator*=(const Conv rhs) // convolution product
{
	if (isZero() || rhs.isZero()) { // avoid returning 0x+0x^1+...
		*this = Conv();
		return *this;
	}
	int deg = max(degree + rhs.degree, (int) ees_N - 1);
	int * arr = (int*)calloc(deg + 1, sizeof(int));
	for (int i = 0; i <= degree; ++i) {
		for (int j = 0; j <= rhs.degree; ++j) {
			arr[i + j % ees_N] += entries[i] * rhs.entries[j];
		}
	}
	degree = deg;
	entries = arr;
	free(arr); // free memory taken by arr	
	return *this;
}
