#pragma once
#include "keyGeneration.h"
#include "conversion.h"
#include <bitset>
#include <iostream>
using namespace std;

Poly bytesToPoly(int * m, int length);
void encryptMessage(string plaintext, Poly h, Poly * ePtr);