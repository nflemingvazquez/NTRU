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

string bitsetToString(bitset <ees_bLen> bits) {
	// convert bitset to string, ignoring last bits
	int length = ees_bLen/8;
	char * chars = new char[length+1]; // +1 due to null terminator
	for (int i = 0; i < length; ++i) { // iterate byte by byte
		char c = 0;
		for (int j = 0; j < 8; ++j) {
			c += bits[8 * i + j] << (7 - j);
		}
		chars[i] = c;
	}
	chars[length] = '\0';
	string str(chars);
	return str;
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
		cout << "File " << filename << " not found!" << endl;
		return "";
	}
}

void createFile(string str, string filename, string directory)
{
	if (directory != "Alice" && directory != "Channel" && directory != "Bob") {
		printf("Invalid directory!\n");
	}
	const char * temp = str.c_str();
	fstream fi("../Demo/" + directory + "/" + filename, ios::out | ios::binary);
	fi.write(temp, str.length());
	fi.close();
}

void polyToFile(Poly a, string filename, string directory) 
{ // write polynomial to file
	if (directory != "Alice" && directory != "Channel" && directory != "Bob") {
		printf("Invalid directory!\n");
	}
	char *str = new char[(a.getDegree()+1) * sizeof(int)];
	memcpy(str, a.getEntries(), (a.getDegree()+1) * sizeof(int));
	fstream fi("../Demo/" + directory + "/" + filename, ios::out | ios::binary);
	fi.write(str, (a.getDegree()+1) * sizeof(int));
	fi.close();
}

Poly convFromFile(string filename, string directory) {
	string str = getFile(filename, directory);
	if (str.length() > ees_N * sizeof(int) || str.length() % sizeof(int) != 0) {
		cout << "Error: file " << filename << " invalid!";
		exit(-1);
	}
	else {
		int * entries = (int*)str.c_str();
		int degree = (int)(str.length() / sizeof(int) - 1);
		Poly result(degree, entries);
		return result;
	}
}