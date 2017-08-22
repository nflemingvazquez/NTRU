#include "NTRU.h"

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
			Poly f, h;
			PolyTriple tri;
			generateKeyPair(ees_N, ees_p, ees_s, ees_r, ees_q, ees_df1, ees_df2, ees_df3, ees_dg, &f, &h, &tri);
			polyToFile(f, "private.bin", "Bob");
			cout << "Private key written to Bob/private.bin" << endl;
			polyToFile(h, "public.bin", "Channel");
			cout << "Public key written to Channel/public.bin" << endl;
			time(&time2);
			break;
		}
		case(2): {
			time(&time1);
			Poly e;
			Poly h = convFromFile("public.bin", "Channel");
			string plaintext = getFile("plaintext.txt", "Alice");
			encryptMessage(plaintext, h, &e);
			cout << "Message encryption completed!" << endl;
			polyToFile(e, "ciphertext.txt", "Channel");
			cout << "Ciphertext transmitted to Channel/ciphertext.txt" << endl;
			time(&time2);
			break;
		}
		case(3): {
			time(&time1);
			Poly e = convFromFile("ciphertext.txt","Channel");
			Poly f = convFromFile("private.bin","Bob");
			Poly h = convFromFile("public.bin", "Channel");
			string decrypted;
			if (decryptMessage(e, f, h, &decrypted)) {
				createFile(decrypted, "decrypted.txt", "Bob");
				cout << "Message decrypted successfully and stored in Bob/decrypted.txt" << endl;
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


