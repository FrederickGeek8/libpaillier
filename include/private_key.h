#include "declaration.h"

#ifndef __PaillierPrivateKey__
#define __PaillierPrivateKey__

class PaillierPrivateKey {
    public:
        PaillierPrivateKey(PaillierPublicKey* public_key, mpz_class p, mpz_class q);
        PaillierPublicKey* public_key;
        mpz_class p;
        mpz_class q;
        mpz_class psquare;
        mpz_class qsquare;
        mpz_class p_inverse;
        mpz_class hp;
        mpz_class hq;

        static PaillierPrivateKey from_totient(PaillierPublicKey* public_key, mpz_class totient);
        float decrypt(EncryptedNumber& encrypted_number);
        EncodedNumber decrypt_encoded(EncryptedNumber& encrypted_number);
        long raw_decrypt(mpz_class ciphertext);
        mpz_class h_function(mpz_class x, mpz_class x_square);
        mpz_class l_function(mpz_class x, mpz_class p);
        long crt(mpz_class mp, mpz_class mq);
        
};

#endif