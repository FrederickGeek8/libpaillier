#include <stdio.h>
#include "declaration.h"

using namespace std;

int main() {
    srand(time(0));
    keypair keys = generate_paillier_keypair(32);
    
    long plaintext = 5;

    EncryptedNumber test = keys.pub_key.encrypt(plaintext);

    float decrypt = keys.priv_key.decrypt(test);
    // int prime = getprimeover(16);
    // bool miller = miller_rabin(19785041, 25);
    cout << test.ciphertext() << endl;
    cout << keys.pub_key.n << endl;
    cout << keys.pub_key.max_int << endl;
    cout << decrypt << endl;
    
    // printf("%d, %d", getprimeover(16), getprimeover(16));
    // keypair keys = generate_paillier_keypair(32);

    // EncodedNumber num = EncodedNumber::encode(&(keys.pub_key), (long)5);
    // EncryptedNumber test = keys.pub_key.encrypt_encoded(num, rand());
    // cout << num.encoding << endl;
    // cout << test.ciphertext() << endl;
    // printf("%Ru, %Ru", num.encoding, test.ciphertext());
}

// Compile command
// g++ -o output.o -w test.cpp -L. -lgmpfrxx -lmpfr -lgmpxx -lgmp -lm