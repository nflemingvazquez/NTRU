#include <stdlib.h>
#include "constants.h"
#include <algorithm>
using namespace std;

int * convProduct(int x[], int y[]);
int * polyMod(int x[], unsigned int modulo);
unsigned int inverseMod(unsigned int x, unsigned int p);
int * extendedEuclidean(int * x, int * y);

class Poly {
public:
	unsigned int degree;
	int * entries;
	Poly() {};
	Poly (unsigned int a, int * b) : degree(a), entries(b) {} // constructor for this class
	Poly operator+ (const Poly&);
	Poly operator- (const Poly&);
	Poly operator* (const Poly&);
	void printValue();
};

