#pragma once
#include <stdlib.h>
extern "C" { // needed because libsodium written in C
#include <sodium.h>
}
#include "constants.h"
#include "polyFunctions.h"

Poly randomConv(int df);
PolyTriple randomTriple(int df1, int df2, int df3);
void generateKeyPair(int N, int p, int s, int r, int q, int df1, int df2, int df3, int dg, Poly * fPtr, Poly * hPtr, PolyTriple * FTriPtr);
//Conv randomConv(int df);
//void generateKeyPair(Conv* f, Conv*);