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
    int get_random_lt_n();
    int raw_encrypt(int plaintext, int r_value = 0);
    
    EncryptedNumber encrypt(EncodedNumber value, float precision = 0, int r_value = 0);
    EncryptedNumber encrypt(int value, float precision = 0, int r_value = 0);
    EncryptedNumber encrypt(float value, float precision = 0, int r_value = 0);

    EncryptedNumber encrypt_encoded(EncodedNumber encoding, int r_value);
};
#endif