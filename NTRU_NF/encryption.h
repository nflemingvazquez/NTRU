#pragma once
#include "keyGeneration.h"
#include "conversion.h"
#include <bitset>
#include <iostream>
using namespace std;

bool encryptMessage(char * plaintext, size_t lenPlaintext, Poly h, Poly * ePtr);