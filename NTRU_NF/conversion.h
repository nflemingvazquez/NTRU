#pragma once
#include "Poly.h"
#include <bitset>
#include <math.h>
#include <string>
#include <fstream>
using namespace std;

bitset <ees_bLen> stringToBitset(char * str);
void createFile(const char * str, size_t size, string filename, string directory);
bool getFile(string filename, string directory, char ** strPtr, size_t * sizePtr);
char * bitsetToString(bitset <ees_bLen> bits);
void polyToFile(Poly a, string filename, string directory);
bool convFromFile(string filename, string directory, Poly * convPtr);