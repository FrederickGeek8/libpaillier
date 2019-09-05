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

int PaillierPublicKey::raw_encrypt(int plaintext, int r_value) {
    int nude_ciphertext;
    if (this->n - this->max_int <= plaintext && plaintext < this->n) {
        int neg_plaintext = this->n - plaintext;
        int neg_ciphertext = (this->n * neg_plaintext + 1) % this->nsquare;
        nude_ciphertext = invert(neg_ciphertext, this->nsquare);
    } else {
        nude_ciphertext = (this->n * plaintext + 1) % this->nsquare;
    }

    int r;
    if (r_value == 0) {
        r = this->get_random_lt_n(); 
    } else {
        r = r_value;
    }

    int obfuscator = powmod(r, this->n, this->nsquare);

    return (nude_ciphertext * obfuscator) % this->nsquare;
}

int PaillierPublicKey::get_random_lt_n() {
    return rand() % this->n + 1;
}

EncryptedNumber PaillierPublicKey::encrypt(EncodedNumber value, float precision, int r_value) {
    return this->encrypt_encoded(value, r_value);
}

EncryptedNumber PaillierPublicKey::encrypt(int value, float precision, int r_value) {
    return this->encrypt_encoded(EncodedNumber::encode(this, value, precision), r_value);
}

EncryptedNumber PaillierPublicKey::encrypt(float value, float precision, int r_value) {
    return this->encrypt_encoded(EncodedNumber::encode(this, value, precision), r_value);
}

EncryptedNumber PaillierPublicKey::encrypt_encoded(EncodedNumber encoding, int r_value) {
    int obfuscator;
    if (r_value == 0) {
        obfuscator = 1;
    } else {
        obfuscator = r_value;
    }

    int ciphertext = this->raw_encrypt(encoding.encoding, obfuscator);
    EncryptedNumber encrypted_number = EncryptedNumber(this, ciphertext, encoding.encoding);
    if (r_value == 0) {
        encrypted_number.obfuscate();
    }

    return encrypted_number;
}