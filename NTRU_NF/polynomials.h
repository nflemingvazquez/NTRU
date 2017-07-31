#include <stdlib.h>
#include "structs.h"
#include "constants.h"

int * polyProduct(int x[], int y[]);
int * polyMod(int x[], unsigned int modulo);
unsigned int inverseMod(unsigned int x, unsigned int p);
int * extendedEuclidean(int * x, int * y);