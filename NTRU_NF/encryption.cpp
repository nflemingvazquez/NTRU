#include "encryption.h"

void encryptMessage(char * plaintext, size_t lenPlaintext, Poly h, Poly * ePtr) {
	if (lenPlaintext > ees_maxMsgLenBytes) {
		cout << "Message too long!" << endl;
	}
	else {
		char * buff = (char*)malloc(ees_db / 8);
		randombytes_buf(buff, ees_db / 8); // generate ees_db/8 random bytes
		char * M = (char*)calloc(ees_bufferLenBits / 8 + 1, 1); // M = buff + plaintext + some zero bytes
		memcpy(M, buff, ees_db / 8);
		M[ees_db / 8] = lenPlaintext;
		memcpy(M + ees_db / 8 + 1, plaintext, lenPlaintext);
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
		int * entries = (int*)malloc(sizeof(int)*ees_N);
		memcpy(entries, arr, ees_N * sizeof(int));
		Poly m(ees_N - 1, entries);
		
		sodium_memzero(buff, ees_db / 8);
		sodium_memzero(M, ees_bufferLenBits / 8);
		sodium_memzero(arr, (ees_N+1)*sizeof(int)); // zero out entries
		sodium_memzero(entries, ees_N*sizeof(int));
		free(buff);
		free(M);
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