#include "keyGeneration.h"
#include "polyFunctions.h"
#include "conversion.h"
#include "encryption.h"
#include "decryption.h"
#include <time.h>
#include <sstream>

int main() {
	time_t time1, time2;
	int param=0;
	string option;
	if (sodium_init() == -1) { // if LibSodium fails to initialise
		return 1;
	}
	cout << "NTRU NF Encryption suite" << endl;
	cout << "Security level: " << ees_securityLevel << " bits" << endl;
	cout << "Options:" << endl;
	cout << "1) Generate key pair" << endl;
	cout << "2) Encrypt file" << endl;
	cout << "3) Decrypt file" << endl;
	cout << "4) Exit" << endl;
	do {
		cout << "Choose option: ";
		getline(cin, option);
		stringstream(option) >> param;
		switch (param) {
		case(1): {
			time(&time1);
			Poly f1, h;
			PolyTriple tri;
			generateKeyPair(ees_N, ees_p, ees_s, ees_r, ees_q, ees_df1, ees_df2, ees_df3, ees_dg, &f1, &h, &tri);
			polyToFile(f1, "private.bin", "Bob");
			cout << "Private key written to Bob/private.bin" << endl;
			polyToFile(h, "public.bin", "Channel");
			cout << "Public key written to Channel/public.bin" << endl;
			time(&time2);
			break;
		}
		case(2): {
			time(&time1);
			Poly h;
			Poly e;
			char * plaintext;
			size_t lenPlaintext;
			if (getFile("plaintext.txt", "Alice", &plaintext, &lenPlaintext)
				&& convFromFile("public.bin", "Channel", &h)) { // if files successfully opened
				encryptMessage(plaintext, lenPlaintext, h, &e);
				cout << "Message encryption completed!" << endl;
				polyToFile(e, "ciphertext.txt", "Channel");
				cout << "Ciphertext transmitted to Channel/ciphertext.txt" << endl;
				sodium_memzero(plaintext, lenPlaintext);
				free(plaintext);
			}
			else {
				cout << "Failed to open key files!" << endl;
			}
			time(&time2);
			break;
		}
		case(3): {
			time(&time1);
			Poly e, f, h;
			char * decrypted;
			size_t decLength;
			if (convFromFile("ciphertext.txt", "Channel", &e) &&
				convFromFile("private.bin", "Bob", &f) &&
				convFromFile("public.bin", "Channel", &h) &&
				decryptMessage(e, f, h, &decrypted, &decLength))
			{
				createFile(decrypted, decLength, "decrypted.txt", "Bob");
				cout << "Message decrypted successfully and stored in Bob/decrypted.txt" << endl;
				sodium_memzero(decrypted, strlen(decrypted));
				free(decrypted);
			}
			else cout << "Failed to decrypt message!" << endl;
			time(&time2);
			break;
		}
		case(4): 
			exit(0);
		default:
			cout << (param == 1) << endl;
			cout << "Invalid option!" << endl;
			break;
		}
		double elapsed = difftime(time2, time1);
		printf("Program took %f seconds to run \n", elapsed);
	} while (param != 4);
	return 0;
}


