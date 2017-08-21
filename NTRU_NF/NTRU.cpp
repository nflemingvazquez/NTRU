#include "NTRU.h"

int main() {
	time_t time1, time2;
	time(&time1);
	if (sodium_init() == -1) { // if LibSodium fails to initialise
		return 1;
	}
	Poly f, h, e, c;
	PolyTriple tri;
	generateKeyPair(ees_N, ees_p, ees_s, ees_r,ees_q, ees_df1, ees_df2, ees_df3, ees_dg, &f, &h, &tri);
	string plaintext = getFile("plaintext.txt", "Alice");
	encryptMessage(plaintext, h, &e);
	bool success=decryptMessage(e, f, h, &c);
	time(&time2);
	double elapsed = difftime(time2, time1);
	printf("Program took %f seconds to run \n",elapsed);
	return 0;
}


