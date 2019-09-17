#include "public_key.h"

PaillierPublicKey::PaillierPublicKey(mpz_class n) {
    this->g = n + 1;
    this->n = n;
    this->nsquare = n * n;
    this->max_int = n / 3 - 1;
}

bool PaillierPublicKey::operator==(const PaillierPublicKey* other) {
    return this->n == other->n;
}

mpz_class PaillierPublicKey::raw_encrypt(mpz_class plaintext, mpz_class r_value) {
    mpz_class nude_ciphertext;
    if (this->n - this->max_int <= plaintext && plaintext < this->n) {
        mpz_class neg_plaintext = this->n - plaintext;
        mpz_class neg_ciphertext = (this->n * neg_plaintext + 1) % this->nsquare;
        mpz_invert(nude_ciphertext.get_mpz_t(), neg_ciphertext.get_mpz_t(), this->nsquare.get_mpz_t());
    } else {
        nude_ciphertext = (this->n * plaintext + 1) % this->nsquare;
    }

    mpz_class r;
    if (r_value == 0) {
        r = this->get_random_lt_n(); 
    } else {
        r = r_value;
    }

    mpz_class obfuscator;
    mpz_powm(obfuscator.get_mpz_t(), r.get_mpz_t(), this->n.get_mpz_t(), this->nsquare.get_mpz_t());

    // printf("r: %lu, o: %lu, n: %lu, n2: %lu\n", r, obfuscator, this->n, this->nsquare);

    return (nude_ciphertext * obfuscator) % this->nsquare;
}

mpz_class PaillierPublicKey::get_random_lt_n() {
    return rand() % this->n + 1;
}

EncryptedNumber PaillierPublicKey::encrypt(EncodedNumber value, float precision, long r_value) {
    return this->encrypt_encoded(value, r_value);
}

EncryptedNumber PaillierPublicKey::encrypt(long value, float precision, long r_value) {
    return this->encrypt_encoded(EncodedNumber::encode(this, value, precision), r_value);
}

EncryptedNumber PaillierPublicKey::encrypt(float value, float precision, long r_value) {
    return this->encrypt_encoded(EncodedNumber::encode(this, value, precision), r_value);
}

EncryptedNumber PaillierPublicKey::encrypt_encoded(EncodedNumber encoding, long r_value) {
    long obfuscator;
    if (r_value == 0) {
        obfuscator = 1;
    } else {
        obfuscator = r_value;
    }

    mpz_class ciphertext = this->raw_encrypt(encoding.encoding, obfuscator);
    EncryptedNumber encrypted_number = EncryptedNumber(this, ciphertext, encoding.exponent);

    if (r_value == 0) {
        encrypted_number.obfuscate();
    }

    return encrypted_number;
}