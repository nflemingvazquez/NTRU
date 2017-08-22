#include "encryption.h"

Poly bytesToPoly(int * m, int length)
{
	return Poly();
}

void encryptMessage(string plaintext, Poly h, Poly * ePtr) {
	if (plaintext.length() > ees_maxMsgLenBytes) {
		printf("Message too long!\n");
	}
	else {
		char l = plaintext.length();
		char * buff = new char[ees_db / 8];
		randombytes_buf(buff, ees_db / 8); // generate ees_db/8 random bytes
		string p0(ees_maxMsgLenBytes - l, 0x0); // 
		string M = string(buff) + l + plaintext + p0;

		bitset <ees_bLen> mbin = stringToBitset(M);
		int * arr = (int*)calloc(ees_N + 1, sizeof(int)); // array with additional byte because ees_N odd
		for (int i = 0; i < ees_bLen / 3;++i) { // generate entries of ternary polynomial from bits of message
			int val = (mbin[3 * i] << 2) + (mbin[3 * i + 1] << 1) + mbin[3 * i + 2]; // value represented by 3 bits
			switch (val) {
			case(0):
				arr[2*i] = 0;
				arr[2*i + 1] = 0;
				break;
			case(1):
				arr[2*i] = 0;
				arr[2*i + 1] = 1;
				break;
			case(2):
				arr[2*i] = 0;
				arr[2*i + 1] = -1;
				break;
			case(3):
				arr[2*i] = 1;
				arr[2*i + 1] = 0;
				break;
			case(4):
				arr[2*i] = 1;
				arr[2*i + 1] = 1;
				break;
			case(5):
				arr[2*i] = 1;
				arr[2*i + 1] = -1;
				break;
			case(6):
				arr[2*i] = -1;
				arr[2*i + 1] = 0;
				break;
			case(7):
				arr[2*i] = -1;
				arr[2*i + 1] = 1;
				break;
			}
		}
		int * entries = new int[ees_N];
		memcpy(entries, arr, ees_N * sizeof(int));
		Poly m(ees_N - 1, entries);
		memset(arr, 0, sizeof(int)); // zero out entries
		memset(entries, 0, sizeof(int));
		free(arr);
		free(entries);
		PolyTriple rTri = randomTriple(ees_dr1, ees_dr2, ees_dr3);
		Poly r(rTri);
		Poly e = r*h + m;
		e.convolute();
		e %= ees_q;
		*ePtr = e;
	}
}

//unsigned char * hash1 = new unsigned char[50];
//crypto_generichash(hash1, 50,
//(unsigned char*)m.getEntries(), m.getDegree() * sizeof(int),
//NULL, 0);