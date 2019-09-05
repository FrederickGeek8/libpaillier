#include "declaration.h"

#ifndef __EncryptedNumber__
#define __EncryptedNumber__

class EncryptedNumber {
   public:
    
    EncryptedNumber(PaillierPublicKey* public_key, int ciphertext, int exponent = 0);

    EncryptedNumber operator+(EncryptedNumber& other);
    EncryptedNumber operator+(EncodedNumber& other);
    EncryptedNumber operator+(int other);
    EncryptedNumber operator+(float other);

    EncryptedNumber operator*(EncodedNumber& other);
    EncryptedNumber operator*(int other);
    EncryptedNumber operator*(float other);

    EncryptedNumber operator/(int scalar);
    EncryptedNumber operator/(float scalar);

    EncryptedNumber _add_encoded(EncodedNumber& encoded);
    EncryptedNumber _add_encrypted(EncryptedNumber& other);
    EncryptedNumber _add_scalar(int scalar);
    EncryptedNumber _add_scalar(float scalar);
    int _raw_add(int e_a, int e_b);
    int _raw_mult(int plaintext);
    int ciphertext(bool be_secure = true);
    EncryptedNumber decrease_exponent_to(int new_exp);
    void obfuscate();
    PaillierPublicKey* public_key;
    int exponent;

    private:
        int _ciphertext;
        bool _is_obfuscated;
};

#endif