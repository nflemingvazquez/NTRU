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
void createFile(string str, string filename, string directory);
string bitsetToString(bitset <ees_bLen> bits);
void polyToFile(Poly a, string filename, string directory);
Poly convFromFile(string filename, string directory);