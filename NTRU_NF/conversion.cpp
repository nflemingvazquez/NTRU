#pragma once
#include "conversion.h"

void polyToString(Poly a, string str)
{ // convert coefficients of polynomial in R/(q) to string
	bitset <ees_r*ees_N> bits; // bitset used to store coefficients
	int deg = a.getDegree();
	int * arr = new int[deg+1];
	memcpy(arr,a.getEntries(),(deg+1)*sizeof(int));
	int counter = 0;
	for (int i = 0; i <= deg; ++i) {
		bitset <ees_r> curr(arr[i]); // current entry in binary
		for (int j = 0; j < ees_r; ++j) {
			bits[counter] = curr[j];
			counter += 1;
		}
	}
	string temp = bits.to_string();
}

Poly stringToPoly(const string str) {
	bitset <ees_r*ees_N> bits(str); // convert string to bitstream
	int * arr = new int[ees_N];
	return Poly();
}

bitset <ees_bLen> stringToBitset(string str) {
	// convert string to bitset, zero-padded to right, if too long bytes truncated
	bitset <ees_bLen> bits;
	for (int i = 0; i < max((int)str.length(), ees_bLen / 8); ++i) {
		char c = str[i];
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
	cout << bits.to_string() << endl; // debug
	int length = ees_bLen/8;
	char * chars = new char[length];
	for (int i = 0; i < length; ++i) { // iterate byte by byte
		char c = 0;
		for (int j = 0; j < 8; ++j) {
			c += bits[j] << j;
		}
	}
	return chars;
}

string getFile(string filename, string directory)
{ // read file from Alice/Bob/channel
	if (directory != "Alice" && directory != "Channel" && directory != "Bob") {
		printf("Invalid directory!\n");
		return "";
	}
	fstream fi ("../Demo/"+directory+"/"+filename, ios::in | ios::binary);
	if (fi) { // if opened
		string plaintext;
		fi.seekg(0,ios::end);
		plaintext.resize(fi.tellg());
		fi.seekg(0, ios::beg);
		fi.read(&plaintext[0], plaintext.size());
		fi.close();
		return(plaintext);
	}
	else {
		printf("File not found!\n");
		return "";
	}
}

void toFile(string str, string filename, string directory)
{ // write file to Alice/Bob/channel
	if (directory != "Alice" && directory != "Channel" && directory != "Bob") {
		printf("Invalid directory!\n");
	}
	fstream fi("../Demo/" + directory + "/" + filename, ios::out | ios::binary);
	fi.write(str.c_str(), str.length());
	fi.close();
}
