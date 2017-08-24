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

Poly operator*(PolyTriple lhs, Poly & rhs)
{
	rhs *= lhs;
	return rhs;
}

Poly operator*(Poly & lhs, PolyTriple rhs)
{
	lhs *= rhs;
	return lhs;
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

Poly operator%(Poly lhs, int rhs)
{
	lhs %= rhs;
	return lhs;
}

Poly::Poly(int a, int * b)
{
	degree = a;
	entries = (int*) calloc(a + 1, sizeof(int));
	memcpy(entries, b, (a + 1) * sizeof(int));
	this->reduceDegree(); // eliminate trailing zeroes
}

Poly::Poly(PolyTriple triple)
{
	*this = triple.a1*triple.a2 + triple.a3;
}

int * Poly::getEntries()
{
	int * result = new int[degree + 1];
	memcpy(result, entries, (degree + 1) * sizeof(int));
	return result;
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

bool Poly::isZero() const // return true iff x = 0
{
	if (degree == 0 && getEntry(0) == 0) {
		return true;
	}
	return false;
}

Poly & Poly::convolute()
{ // return convolution form
	if (degree < ees_N) { // if degree < ees_N already convoluted
		return *this;
	}
	else {
		int * arr = new int[ees_N];
		memcpy(arr, entries, sizeof(int)*ees_N); // initialise arr using first N entries of poly
		for (int i = ees_N; i <= degree; ++i) { // x^k coefficient of result = sum of x^(k modN) coefficients
			arr[i % ees_N] += entries[i];
		}
		delete entries; // free memory used by entries
		entries = new int[degree + 1];
		memcpy(entries,arr,sizeof(int)*ees_N);
		sodium_memzero(arr, ees_N*sizeof(int));
		delete arr;
		degree = ees_N - 1;
		this->reduceDegree();
		return *this;
	}
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

Poly & Poly::operator*=(PolyTriple rhs)
{ // return this*rhs where rhs = a1*a2+a3
	Poly t1 = rhs.a1*(*this);
	Poly t2 = rhs.a2*t1;
	Poly t3 = rhs.a3*(*this);
	Poly c = t2 + t3;
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

Poly & Poly::operator%=(int p)
{ // mod coefficients by p, over interval [-ceil(p/2),floor(p/2)]
	for (int i = 0; i <= degree; ++i) {
		int entry=entries[i]%p; // entry modulo p
		if (entry < -ceil(p/2)) { // % returns negative value if argument negative
			entry += p;
		}
		else if (entry > floor(p / 2)) {
			entry -= p;
		}
		entries[i] = entry;
	}
	this->reduceDegree();
	return *this;
}

void Poly::printValue() const
{
	// print e.g. 1+1*x^1+0*x^2+3*x^3
	printf("%d ", entries[0]);
	for (int i = 1; i <= degree; i++) {
		printf("+ %d x^%u ", entries[i], i);
	}
	printf("\n");
}

//Conv operator*(Conv lhs, const Conv &rhs)
//{
//	lhs *= rhs;
//	return lhs;
//}
//
//Conv & Conv::operator*=(const Conv rhs) // convolution product
//{
//	if (isZero() || rhs.isZero()) { // avoid returning 0x+0x^1+...
//		*this = Conv();
//		return *this;
//	}
//	int deg = max(degree + rhs.degree, (int) ees_N - 1);
//	int * arr = (int*)calloc(deg + 1, sizeof(int));
//	for (int i = 0; i <= degree; ++i) {
//		for (int j = 0; j <= rhs.degree; ++j) {
//			arr[i + j % ees_N] += entries[i] * rhs.entries[j];
//		}
//	}
//	degree = deg;
//	entries = arr;
//	free(arr); // free memory taken by arr	
//	return *this;
//}
