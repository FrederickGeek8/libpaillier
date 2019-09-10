#include <stdio.h>
#include "public_key.h"

int main() {
    srand(time(0));
    // keypair keys = generate_paillier_keypair(32);
    long plaintext = 5;
    // EncryptedNumber test = keys.pub_key.encrypt(plaintext);

    // int prime = getprimeover(16);
    // bool miller = miller_rabin(19785041, 25);
    // printf("%lu\n%lu\n%lu", test.ciphertext(), keys.pub_key.n, keys.pub_key.max_int);
    // printf("%d, %d", getprimeover(16), getprimeover(16));
    keypair keys = generate_paillier_keypair(32);

    EncodedNumber num = EncodedNumber::encode(&(keys.pub_key), plaintext);
    EncryptedNumber test = keys.pub_key.encrypt_encoded(num, rand());
    printf("%lu, %lu", num.encoding, test.ciphertext());
}