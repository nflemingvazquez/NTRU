#pragma once
#include <stdlib.h>
#include <algorithm> // required by max
#include "constants.h"
using namespace std;

class Poly {
protected: // protected, not private so that inherited by Conv class
	unsigned int degree;
	int * entries;
public:
	Poly() { degree = 0; entries = (int*) calloc(1, sizeof(int)); }; // default constructor is 0
	Poly(unsigned int a, int * b);

	unsigned int getDegree() { return degree; }
	int * getEntries() { return entries; }
	int getEntry(unsigned int n);	
	Poly& reduceDegree();
	Poly& mod(unsigned int p);
	
	bool isZero();

	Poly& operator+=(const Poly rhs);
	Poly& operator+=(int rhs);

	Poly& operator*=(const Poly rhs);
	Poly& operator*=(int rhs);

	Poly& operator-();
	Poly& operator-=(const Poly rhs);

	friend Poly operator+(Poly lhs, const Poly& rhs); // operator overloaders for +
	friend Poly operator+(Poly lhs, int rhs);
	friend Poly operator+(int lhs, Poly& rhs);

	friend Poly operator*(Poly lhs, const Poly& rhs);
	friend Poly operator*(Poly lhs, int rhs);
	friend Poly operator*(int lhs, Poly& rhs);

	friend Poly operator- (Poly lhs, const Poly& rhs);
	friend Poly operator- (Poly lhs, int rhs);
	friend Poly operator- (int lhs, Poly& rhs);

	void printValue();
};
// derived class
class Conv : public Poly { // convolution polynomial class
public:
	Conv& operator*=(const Conv rhs);
	friend Conv operator*(Conv lhs, const Conv& rhs);
};