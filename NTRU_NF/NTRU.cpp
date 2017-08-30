#include "keyGeneration.h"
#include "encryption.h"
#include "decryption.h"
#include <time.h>
#include <sstream>
#include <stdio.h>
#include <experimental/filesystem>
namespace fs = experimental::filesystem;

int main() {
	clock_t time1, time2;
	double CPUTimeUsed;
	int param=0;
	string option;
	if (sodium_init() == -1) { // if LibSodium fails to initialise
		return 1;
	}
	// rename constants defined in LibSodium for readability
	const size_t cipherBaseSize = crypto_aead_xchacha20poly1305_ietf_ABYTES;

	cout << "NTRU NF Encryption Suite" << endl;
	cout << "Security level: " << ees_securityLevel << " bits" << endl;
	cout << "Options:" << endl;
	cout << "1) Initiate session" << endl;
	cout << "2) Encrypt file" << endl;
	cout << "3) Decrypt file" << endl;
	cout << "4) Close session" << endl;
	cout << "5) Exit" << endl;
	while(1) 
	{
		cout << "Choose option: ";
		getline(cin, option);
		stringstream(option) >> param;
		time1 = clock();
		switch (param) {
		case(1): {
			// NTRU key generation, performed by Bob
			Poly f, h;
			PolyTriple tri;
			generateKeyPair(ees_N, ees_p, ees_s, ees_r, ees_q, ees_df1, ees_df2, ees_df3, ees_dg, &f, &h, &tri);
			fs::create_directory("../Demo/Channel");
			polyToFile(h, "public.bin", "Channel");
			cout << "Generated NTRU key pair" << endl;
			// generation and encryption of secret key, performed by Alice
			Poly hAlice;
			Poly e;
			const size_t secretKeySize = crypto_aead_xchacha20poly1305_ietf_KEYBYTES;
			char * secretKey=(char*)malloc(secretKeySize);
			randombytes_buf(secretKey, secretKeySize); // generate secret key
			createFile(secretKey, secretKeySize, "secret.bin", "Alice");
			char * nonce = (char*)malloc(crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);
			randombytes_buf(nonce, crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);
			if (convFromFile("public.bin", "Channel", &hAlice)) { // if public key found
				encryptMessage(secretKey, secretKeySize, hAlice, &e);
				polyToFile(e, "encryptedKey.bin", "Channel"); // write ciphertext to channel
			}
			else {
				cout << "Failed to create session; invalid NTRU public key" << endl;
				break;
			}
			sodium_memzero(secretKey, secretKeySize);
			free(secretKey);
			// decryption of secret key, performed by Bob
			Poly eBob;
			char * decrypted = NULL;
			size_t decLength;
			if (convFromFile("encryptedKey.bin", "Channel", &eBob) && decryptMessage(eBob, f, h, &decrypted, &decLength))
			{
				createFile(decrypted, decLength, "secret.bin", "Bob");
				sodium_memzero(decrypted, strlen(decrypted));
				free(decrypted);
				cout << "Successfully initiated new session" << endl;
			}
			else {
				cout << "Failed to create session; NTRU ciphertext decryption failed" << endl;
				break;
			}
			break;
		}
		case(2): {
			// create nonce for use with this message
			const size_t nonceSize = crypto_aead_xchacha20poly1305_ietf_NPUBBYTES;
			char * nonce = (char*)malloc(nonceSize);
			randombytes_buf(nonce, nonceSize);

			char * plaintext=NULL;
			size_t plaintextSize;
			char * secretKey=NULL;
			size_t secretKeySize;
			createFile(nonce, nonceSize, "nonce.bin", "Channel");
			if (getFile("secret.bin", "Alice", &secretKey, &secretKeySize) &&
				getFile("plaintext", "Alice", &plaintext, &plaintextSize)) {
				size_t ciphertextSize;
				char * ciphertext = (char*)malloc(cipherBaseSize + plaintextSize);
				crypto_aead_xchacha20poly1305_ietf_encrypt((unsigned char*)ciphertext, &ciphertextSize,
					(const unsigned char*)plaintext, plaintextSize,
					NULL, 0,
					NULL, (const unsigned char*)nonce, (const unsigned char*)secretKey);
				createFile(ciphertext, ciphertextSize, "ciphertext.bin", "Channel");
				cout << "Message encrypted successfully!" << endl;
				free(ciphertext);
			}
			else {
				cout << "Message encryption failed, try initiating new session.";
			}
			free(nonce);
			free(plaintext);
			free(secretKey);
			break;
		}
		case(3): {
			char * nonce=NULL;
			char * secretKey=NULL;
			char * ciphertext=NULL;
			size_t nonceSize;
			size_t secretKeySize;
			size_t ciphertextSize;
			if (getFile("nonce.bin", "Channel", &nonce, &nonceSize) && // if files opened successfully
				getFile("secret.bin", "Bob", &secretKey, &secretKeySize) &&
				getFile("ciphertext.bin","Channel", &ciphertext, &ciphertextSize) && 
				ciphertextSize > cipherBaseSize) {
				char * decrypted = (char*)malloc(ciphertextSize - cipherBaseSize);
				size_t decryptedSize;
				if (crypto_aead_xchacha20poly1305_ietf_decrypt((unsigned char*)decrypted, &decryptedSize,
					NULL,
					(const unsigned char*)ciphertext, ciphertextSize,
					NULL,
					0,
					(const unsigned char*)nonce, (const unsigned char*)secretKey) == 0) {
					createFile(decrypted, decryptedSize, "decrypted", "Bob");
					cout << "Message decrypted successfully!" << endl;
				 }
				else {
					cout << "Message forged!" << endl;
				}
			}
			else {
				cout << "Message decryption failed!" << endl;
			}
			break;
		}
		case(4): { 
			string ext = "../Demo/";
			fs::remove_all(ext+"Channel"); // delete channel directory
			fs::remove(ext + "Alice/secret.bin");
			fs::remove(ext + "Bob/secret.bin");
			cout << "Session files deleted" << endl;
			break;
		}
		case(5): 
			exit(0);
		default:
			cout << (param == 1) << endl;
			cout << "Invalid option!" << endl;
			break;
		}
		time2 = clock();
		CPUTimeUsed = (double)(time2 - time1) / CLOCKS_PER_SEC;
		printf("Process took %.3f seconds to run \n", CPUTimeUsed);
	}
	return 0;
}


