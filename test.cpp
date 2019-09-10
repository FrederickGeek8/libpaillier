#include <stdio.h>
#include "public_key.h"

int main() {
    srand(time(0));
    keypair keys = generate_paillier_keypair(32);
    long plaintext = 1;
    EncryptedNumber test = keys.pub_key.encrypt(plaintext);
    // int prime = getprimeover(16);
    // bool miller = miller_rabin(19785041, 25);
    printf("%lu\n", test.ciphertext());
    // printf("%d, %d", getprimeover(16), getprimeover(16));
}