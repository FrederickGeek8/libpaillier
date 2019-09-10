#include "declaration.h"

#ifndef __PaillierPublicKey__
#define __PaillierPublicKey__

class PaillierPublicKey {
   public:
    long max_int;
    long n;
    long nsquare;
    long g;
    PaillierPublicKey(long n);
    bool operator==(const PaillierPublicKey& other);
    long get_random_lt_n();
    long raw_encrypt(long plaintext, long r_value = 0);
    
    EncryptedNumber encrypt(EncodedNumber value, float precision = 0, long r_value = 0);
    EncryptedNumber encrypt(long value, float precision = 0, long r_value = 0);
    EncryptedNumber encrypt(float value, float precision = 0, long r_value = 0);

    EncryptedNumber encrypt_encoded(EncodedNumber encoding, long r_value);
};
#endif