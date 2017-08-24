#include "decryption.h"

bool decryptMessage(Poly e, Poly f, Poly h, char ** decPtr, size_t * lengthPtr)
{
	Poly a = (f*e).convolute()%ees_q;
	Poly b = a % ees_p;
	Poly f_p;
	convPrimeInverse(f, ees_N, ees_p, &f_p); // recalculate f_p
	Poly c = (f_p*b).convolute() % ees_p; // decrypted polynomial
	// convert c to string
	bitset <ees_bLen> decBits;
	int * entries = c.getEntries();
	int * arr = (int*)calloc(ees_N, sizeof(int)); // stores entries of c, plus any zero entries of degree < ees_N
	memcpy(arr, entries, (c.getDegree() + 1) * sizeof(int));
	for (int i = 0; i < ees_bLen / 3; ++i) {
		int check = 10 * (arr[2*i] + 1) + (arr[2*i+1] + 1); // convert pair of entries to 2 digit number so it can be handled by switch-case, 1 -> 2, 0 -> 1, -1 -> 0
		switch (check) {
		case(11): // <-> {0,0}
			// all 3 bits already set to 0
			break;
		case(12): // <-> {0,1}
			decBits[3 * i + 2] = 1;
			break;
		case(10): // <-> {0,-1}
			decBits[3 * i + 1] = 1;
			break;
		case(21): // <-> {1,0}
			decBits[3 * i + 1] = 1;
			decBits[3 * i + 2] = 1;
			break;
		case(22): // <-> {1,1}
			decBits[3 * i] = 1;
			break;
		case(20): // <-> {1,-1}
			decBits[3 * i] = 1;
			decBits[3 * i + 2] = 1;
			break;
		case(1): // <-> {-1,0}
			decBits[3 * i] = 1;
			decBits[3 * i + 1] = 1;
			break;
		case(2): // <-> {-1,1}
			decBits[3 * i] = 1;
			decBits[3 * i + 1] = 1;
			decBits[3 * i + 2] = 1;
			break;
		default:
			cout << "Failed to convert polynomial to message" << endl; // debug
			return false;
		}
	}
	char * msg = bitsetToString(decBits);
	int size = msg[ees_db / 8]; // byte storing length
	// retrieve original message from surrounding buffer
	char * dec = new char[size];
	if (ees_db / 8 + 1 + size > ees_bLen / 8 || size == 0) return false; // avoid buffer overflow
	memcpy(msg + ees_db / 8 + 1, dec, size);
	sodium_memzero(entries, (c.getDegree() + 1) * sizeof(int));
	sodium_memzero(arr, (c.getDegree() + 1) * sizeof(int));
	sodium_memzero(msg, ees_bLen / 8);
	free(entries);
	free(arr);
	free(msg);
	*lengthPtr = size;
	*decPtr = dec;
	return true;
}
