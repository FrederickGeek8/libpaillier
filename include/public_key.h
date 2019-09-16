#include "declaration.h"

#ifndef __PaillierPublicKey__
#define __PaillierPublicKey__

class PaillierPublicKey {
   public:
    mpz_class max_int;
    mpz_class n;
    mpz_class nsquare;
    mpz_class g;
    PaillierPublicKey(mpz_class n);
    bool operator==(const PaillierPublicKey* other);
    mpz_class get_random_lt_n();
    mpz_class raw_encrypt(mpz_class plaintext, mpz_class r_value = 0);
    
    EncryptedNumber encrypt(EncodedNumber value, float precision = 0, long r_value = 0);
    EncryptedNumber encrypt(long value, float precision = 0, long r_value = 0);
    EncryptedNumber encrypt(float value, float precision = 0, long r_value = 0);

    EncryptedNumber encrypt_encoded(EncodedNumber encoding, long r_value);
};
#endif