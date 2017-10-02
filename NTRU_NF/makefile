CC=g++
CFLAGS=-Wall
LDFLAGS=-lsodium
NTRU: NTRU.o encryption.o decryption.o keyGeneration.o conversion.o polyFunctions.o Poly.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
.cpp.o:
	$(CC) $(CFLAGS) -c $< $(LDFLAGS) -std=c++11
clean:
	rm -f *.o
