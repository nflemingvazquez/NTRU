#pragma once
#include <stdlib.h>
extern "C" { // needed because libsodium written in C
#include <sodium.h>
}
#include "constants.h"

int * randomPoly(unsigned int df);
