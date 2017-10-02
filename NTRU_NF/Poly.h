#pragma once
#include <stdlib.h>
#include <string>
#include <iostream>
#include <algorithm> // required by max
extern "C" {
#include <sodium.h>
}
#include "constants.h"
using namespace std;

struct PolyTriple;

class Poly {
protected: // protected, not private so that inherited by Conv class
	int degree;
	int * entries;
public:
	Poly() { degree = 0; entries = (int*) calloc(1, sizeof(int)); }; // default constructor is 0
	Poly(int a, int * b);
	Poly(PolyTriple triple); // a=a1*a2+a3

	int getDegree() const { return degree; }
	int * getEntries();
	int getEntry(int n) const;	
	Poly& reduceDegree();
	bool isZero() const;
	Poly& convolute();
	Poly& shift(int n);
	unsigned char * hash();

	~Poly() { // destructor
		//sodium_memzero(entries, (degree + 1) * sizeof(int));
		free(entries);
	}

	Poly(const Poly& poly2) { // copy constructor
		degree = poly2.degree;
		entries = (int*)malloc(sizeof(int)*(degree + 1));
		memcpy(entries, poly2.entries, sizeof(int)*(degree + 1));
	}

	Poly& operator=(const Poly& rhs) {
		if (this == &rhs) { // comparison with same pointer
			return *this;
		}
		if (entries != NULL) { // clean memory already allocated
			//sodium_memzero(entries, sizeof(int)*(degree + 1));
			free(entries);
		}
		degree = rhs.degree;
		entries = (int*)malloc(sizeof(int)*(degree + 1));
		memcpy(entries, rhs.entries, sizeof(int)*(degree + 1));
		return *this;
	}

	Poly& operator+=(const Poly rhs);
	Poly& operator+=(int rhs);

	Poly& operator*=(const Poly rhs);
	Poly& operator*=(int rhs);
	Poly& operator*=(PolyTriple rhs);

	Poly& operator-();
	Poly& operator-=(const Poly rhs);

	Poly& operator%=(int p);

	friend Poly operator+(Poly lhs, const Poly& rhs); // operator overloaders for +
	friend Poly operator+(Poly lhs, int rhs);
	friend Poly operator+(int lhs, Poly& rhs);

	friend Poly operator*(Poly lhs, const Poly& rhs);
	friend Poly operator*(Poly lhs, int rhs);
	friend Poly operator*(int lhs, Poly& rhs);
	friend Poly operator*(PolyTriple lhs, Poly& rhs);
	friend Poly operator*(Poly& rhs, PolyTriple lhs);

	friend Poly operator- (Poly lhs, const Poly& rhs);
	friend Poly operator- (Poly lhs, int rhs);
	friend Poly operator- (int lhs, Poly& rhs);

	friend Poly operator% (Poly lhs, int rhs);

	void printValue() const;
};

struct PolyTriple { // represents polynomial a=a1*a2+a3 
	Poly a1;
	Poly a2;
	Poly a3;
};

// derived class
//class Conv : public Poly { // convolution polynomial class
//public:
//	Conv(int a, int* b) : Poly(a, b) {};
//	Conv() : Poly() {};
//	Conv& operator*=(const Conv rhs);
//	friend Conv operator*(Conv lhs, const Conv& rhs);
//};