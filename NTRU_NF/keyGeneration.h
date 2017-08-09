#pragma once
#include <stdlib.h>
extern "C" { // needed because libsodium written in C
#include <sodium.h>
}
#include "constants.h"
#include "Poly.h"

Conv randomConv(int df);
void generateKeyPair(Conv* f, Conv*);