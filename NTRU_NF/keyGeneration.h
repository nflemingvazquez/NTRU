#pragma once
#include <stdlib.h>
extern "C" { // needed because libsodium written in C
#include <sodium.h>
}
#include "constants.h"
#include "polyFunctions.h"

Poly randomConv(int df);
PolyTriple randomTriple(int df1, int df2, int df3);
void generateKeyPair(Poly * fPtr, Poly * hPtr, PolyTriple * FTriPtr);