#include "decryption.h"

bool decryptMessage(Poly e, Poly f, Poly h, Poly * cPtr)
{
	Poly a = (f*e).convolute()%ees_q;
	Poly b = a % ees_p;
	Poly f_p;
	convPrimeInverse(f, ees_N, ees_p, &f_p); // recalculate f_p
	Poly c = (f_p*b).convolute() % ees_p; // decrypted polynomial
	// convert c to string
	bitset <ees_bLen> decBits;
	int * entries = c.getEntries();
	int * arr = (int*)calloc(ees_N, sizeof(int));
	memcpy(arr, c.getEntries(), (c.getDegree() + 1) * sizeof(int));
	printf("\n\n");
	for (int i = 0; i < ees_bLen / 3; i += 2) { // can ignore last entry since last byte of message always padded
		int check = 10 * (arr[i] + 1) + arr[i+1] + 1; // convert pair of entries to 2 digit number so it can be handled by switch-case, 1 -> 2, 0 -> 1, -1 -> 0
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
			printf("check=%d,entries[i]=%d,[i+1]=%d\n", check,entries[i],entries[i+1]);
			printf("Failed to convert decrypted polynomial!\n");
			return false;
		}
	}
	char * decrypted = bitsetToString(decBits);
	cout << decrypted << endl;
	return true;
}
