#pragma once
#include "Poly.h"
#include <bitset>
#include <math.h>
#include <string>
#include <fstream>
using namespace std;

void polyToString(Poly a, string str);
Poly stringToPoly(const string str);
bitset <ees_bLen> stringToBitset(string str);
string getFile(string filename, string directory);
char * bitsetToString(bitset <ees_bLen> bits);
void toFile(string str, string filename, string directory);