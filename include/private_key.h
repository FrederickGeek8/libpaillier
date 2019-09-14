#include "declaration.h"

#ifndef __PaillierPrivateKey__
#define __PaillierPrivateKey__

class PaillierPrivateKey {
    public:
        PaillierPrivateKey(PaillierPublicKey* public_key, long p, long q);
        PaillierPublicKey* public_key;
        long p;
        long q;
        long psquare;
        long qsquare;
        long p_inverse;
        long hp;
        long hq;

        static PaillierPrivateKey from_totient(PaillierPublicKey* public_key, long totient);
        float decrypt(EncryptedNumber& encrypted_number);
        float decrypt_encoded(EncryptedNumber& encrypted_number);
        long raw_decrypt(long ciphertext);
        long h_function(long x, long x_square);
        long l_function(long x, long p);
        long crt(long mp, long mq);
        
};

#endif