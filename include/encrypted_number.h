#include "declaration.h"

#ifndef __EncryptedNumber__
#define __EncryptedNumber__

class EncryptedNumber {
   public:
    
    EncryptedNumber(PaillierPublicKey* public_key, mpz_class ciphertext, long exponent = 0);

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
    mpz_class _raw_add(mpz_class e_a, mpz_class e_b);
    mpz_class _raw_mult(mpz_class plaintext);
    mpz_class ciphertext(bool be_secure = true);
    EncryptedNumber decrease_exponent_to(long new_exp);
    void obfuscate();
    PaillierPublicKey* public_key;
    long exponent;

    private:
        mpz_class _ciphertext;
        bool _is_obfuscated;
};

#endif