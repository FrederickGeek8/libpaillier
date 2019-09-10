#include "declaration.h"

#ifndef __EncryptedNumber__
#define __EncryptedNumber__

class EncryptedNumber {
   public:
    
    EncryptedNumber(PaillierPublicKey* public_key, long ciphertext, long exponent = 0);

    EncryptedNumber operator+(EncryptedNumber& other);
    EncryptedNumber operator+(EncodedNumber& other);
    EncryptedNumber operator+(long other);
    EncryptedNumber operator+(float other);

    EncryptedNumber operator*(EncodedNumber& other);
    EncryptedNumber operator*(long other);
    EncryptedNumber operator*(float other);

    EncryptedNumber operator/(long scalar);
    EncryptedNumber operator/(float scalar);

    EncryptedNumber _add_encoded(EncodedNumber& encoded);
    EncryptedNumber _add_encrypted(EncryptedNumber& other);
    EncryptedNumber _add_scalar(long scalar);
    EncryptedNumber _add_scalar(float scalar);
    long _raw_add(long e_a, long e_b);
    long _raw_mult(long plaintext);
    long ciphertext(bool be_secure = true);
    EncryptedNumber decrease_exponent_to(long new_exp);
    void obfuscate();
    PaillierPublicKey* public_key;
    long exponent;

    private:
        long _ciphertext;
        bool _is_obfuscated;
};

#endif