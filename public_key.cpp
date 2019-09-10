#include "public_key.h"

PaillierPublicKey::PaillierPublicKey(long n) {
    this->g = n + 1;
    this->n = n;
    this->nsquare = n * n;
    this->max_int = n / 3 - 1;
}

bool PaillierPublicKey::operator==(const PaillierPublicKey& other) {
    return this->n == other.n;
}

long PaillierPublicKey::raw_encrypt(long plaintext, long r_value) {
    long nude_ciphertext;
    if (this->n - this->max_int <= plaintext && plaintext < this->n) {
        long neg_plaintext = this->n - plaintext;
        long neg_ciphertext = (this->n * neg_plaintext + 1) % this->nsquare;
        nude_ciphertext = invert(neg_ciphertext, this->nsquare);
    } else {
        nude_ciphertext = (this->n * plaintext + 1) % this->nsquare;
    }

    long r;
    if (r_value == 0) {
        r = this->get_random_lt_n(); 
    } else {
        r = r_value;
    }

    long obfuscator = powmod(r, this->n, this->nsquare);

    return (nude_ciphertext * obfuscator) % this->nsquare;
}

long PaillierPublicKey::get_random_lt_n() {
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

    long ciphertext = this->raw_encrypt(encoding.encoding, obfuscator);
    EncryptedNumber encrypted_number = EncryptedNumber(this, ciphertext, encoding.encoding);
    if (r_value == 0) {
        encrypted_number.obfuscate();
    }

    return encrypted_number;
}