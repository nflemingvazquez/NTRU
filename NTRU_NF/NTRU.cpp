#include "keyGeneration.h"
#include "polyFunctions.h"
#include "conversion.h"
#include "encryption.h"
#include "decryption.h"
#include <time.h>
#include <sstream>

int main() {
	clock_t time1, time2;
	int param;
	string option;
	if (sodium_init() == -1) { // if LibSodium fails to initialise
		return 1;
	}
	cout << "NTRU Tests" << endl;
	cout << "Security level: " << ees_securityLevel << " bits" << endl;
	cout << "Options:" << endl;
	cout << "1) Generate key pair" << endl;
	cout << "2) Encrypt file" << endl;
	cout << "3) Decrypt file" << endl;
	cout << "4) Exit" << endl;
	while(1) 
	{
		param = 0; // handle cases where param not overwritten
		cout << "Choose option: ";
		getline(cin, option);
		stringstream(option) >> param;
		time1 = clock();
		switch (param) {
		case(1): {
				Poly fGen, hGen;
				PolyTriple tri;
				generateKeyPair(&fGen, &hGen, &tri);
				cout << "Key pair generated" << endl;
				polyToFile(fGen, "private.bin", "Bob");
				cout << "Private key written to Bob/private.bin" << endl;
				polyToFile(hGen, "public.bin", "Channel");
				cout << "Public key written to Channel/public.bin" << endl;
			break;
		}
		case(2): {
			Poly h;
			Poly e;
			char * plaintext=NULL;
			size_t lenPlaintext;
			if (getFile("plaintext.txt", "Alice", &plaintext, &lenPlaintext)
				&& convFromFile("public.bin", "Channel", &h) && encryptMessage(plaintext, lenPlaintext, h, &e)) { // if files successfully opened
				cout << "Message encryption completed!" << endl;
				polyToFile(e, "ciphertext.txt", "Channel");
				cout << "Ciphertext transmitted to Channel/ciphertext.txt" << endl;
				sodium_memzero(plaintext, lenPlaintext);
				free(plaintext);
			}
			else {
				cout << "Failed to encrypt message!" << endl;
			}
			break;
		}
		case(3): {
			Poly e, f, h;
			char * decrypted=NULL;
			size_t decLength;
			if (convFromFile("ciphertext.txt", "Channel", &e) &&
				convFromFile("private.bin", "Bob", &f) &&
				convFromFile("public.bin", "Channel", &h) &&
				decryptMessage(e, f, h, &decrypted, &decLength))
			{
				createFile(decrypted, decLength, "decrypted.txt", "Bob");
				cout << "Message decrypted successfully and stored in Bob/decrypted.txt" << endl;
				sodium_memzero(decrypted, decLength);
				free(decrypted);
			}
			else cout << "Failed to decrypt message!" << endl;
			break;
		}
		case(4):
			exit(0);
		default:
			cout << "Invalid option!" << endl;
			break;
		}
		time2 = clock();
		double elapsed = (double) (time2 - time1) / CLOCKS_PER_SEC;
				printf("Program took %.5f seconds to run \n", elapsed);
	}
	return 0;
}
