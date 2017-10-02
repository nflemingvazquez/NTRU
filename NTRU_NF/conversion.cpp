#pragma once
#include "conversion.h"

bitset <ees_bLen> stringToBitset(char * str) {
	// convert string to bitset, padded to right, if too long bytes truncated
	bitset <ees_bLen> bits;
	for (int i = 0; i < ees_bLen / 8; ++i) {
		//cout << "Maximum is " << max((int)str.length(), ees_bLen / 8) << endl;
		unsigned char c = str[i];
		for (int j = 7; j >= 0 && c; --j) {
			if (c & 1) { // if last digit of c is 1
				bits.set(8 * i + j); // set corresponding bit of mbin to 1
			}
			c >>= 1; // shift digit of c to right
		}
	}
	return bits;
}

char * bitsetToString(bitset <ees_bLen> bits) {
	// convert bitset to string, ignoring last bits
	int length = ees_bLen/8;
	char * chars = (char*)malloc(length); // +1 due to null terminator
	for (int i = 0; i < length; ++i) { // iterate byte by byte
		char c = 0;
		for (int j = 0; j < 8; ++j) {
			c += bits[8 * i + j] << (7 - j);
		}
		chars[i] = c;
	}
	return chars;
}

bool getFile(string filename, string directory, char ** strPtr, size_t * sizePtr)
{ // return char array without null terminator
	if (directory != "Alice" && directory != "Channel" && directory != "Bob") {
	 	printf("Invalid directory!\n");
		return false;
	}
	fstream fi ("../Demo/"+directory+"/"+filename, ios::in | ios::binary);
	if (fi) { // if opened
		fi.seekg(0,ios::end);
		size_t len = (size_t) fi.tellg();
		char * plaintext = (char*)malloc(len);
		fi.seekg(0, ios::beg);
		fi.read(plaintext, len);
		fi.close();
		*strPtr = plaintext;
		*sizePtr = len;
		return(true);
	}
	else {
		cout << "File " << filename << " not found!" << endl;
		return false;
	}
}

void createFile(const char * str, size_t size, string filename, string directory)
{
	if (directory != "Alice" && directory != "Channel" && directory != "Bob") {
		printf("Invalid directory!\n");
	}
	fstream fi("../Demo/" + directory + "/" + filename, ios::out | ios::binary);
	fi.write(str, size);
	fi.close();
}

void polyToFile(Poly a, string filename, string directory) 
{ // write polynomial to file
	char * arr = (char*)a.getEntries();
	int size = sizeof(int)*(a.getDegree() + 1);
	createFile(arr, size, filename, directory);
	//sodium_memzero(arr, size);
	free(arr);
}

bool convFromFile(string filename, string directory, Poly * convPtr) {
	char * str;
	size_t size;
	if (!getFile(filename, directory, &str, &size)) {
		cout << "Failed to open " << filename << endl;
		return false;
	}
	if (size > ees_N * sizeof(int) || size % sizeof(int) != 0) {
		cout << "Error: file " << filename << " invalid!" << endl;
		return false;
	}
	else {
		int * entries = (int*)malloc(size);
		memcpy(entries, str, size);
		int degree = (int)(size / sizeof(int) - 1);
		Poly result(degree, entries);
		//sodium_memzero(str, size);
		//sodium_memzero(entries, size);
		free(str);
		free(entries);

		*convPtr = result;
		return true;
	}
}
