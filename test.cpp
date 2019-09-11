#include <stdio.h>
#include "public_key.h"

using namespace std;

int main() {
    srand(time(0));
    // keypair keys = generate_paillier_keypair(32);
    mpz_class plainz = 5;
    float test = 2.0;
    mpfr_class output = 5 * test;
    cout << "This is " << output << endl;
    mpfr_class plainf = 5.12;
    cout << "This is " << mpfr_get_flt(output.get_mpfr_t(), MPFR_RNDD) << endl;
    // EncryptedNumber test = keys.pub_key.encrypt(plaintext);

    // int prime = getprimeover(16);
    // bool miller = miller_rabin(19785041, 25);
    // printf("%lu\n%lu\n%lu", test.ciphertext(), keys.pub_key.n, keys.pub_key.max_int);
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