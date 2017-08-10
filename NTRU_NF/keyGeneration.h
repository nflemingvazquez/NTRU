#pragma once
#include <stdlib.h>
extern "C" { // needed because libsodium written in C
#include <sodium.h>
}
#include "constants.h"
#include "Poly.h"

Poly randomConv(int df);
PolyTriple randomTriple(int df1, int df2, int df3);
void generateKeyPair(int N, int q, int p, int df1, int df2, int df3, Poly * f, Poly * g);

//Conv randomConv(int df);
//void generateKeyPair(Conv* f, Conv*);