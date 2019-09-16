#include "encrypted_number.h"

EncryptedNumber::EncryptedNumber(PaillierPublicKey* public_key,
                                 mpz_class ciphertext, long exponent) {
    this->public_key = public_key;
    this->_ciphertext = ciphertext;
    this->exponent = exponent;
    this->_is_obfuscated = false;
}

// Adding
EncryptedNumber EncryptedNumber::operator+(EncryptedNumber& other) {
    return this->_add_encrypted(other);
}

EncryptedNumber EncryptedNumber::operator+(EncodedNumber& other) {
    return this->_add_encoded(other);
}

EncryptedNumber EncryptedNumber::operator+(long other) {
    return this->_add_scalar(other);
}

EncryptedNumber EncryptedNumber::operator+(float other) {
    return this->_add_scalar(other);
}

// Multiplying
EncryptedNumber EncryptedNumber::operator*(EncodedNumber& other) {
    mpz_class product = this->_raw_mult(other.encoding);
    long exponent = this->exponent + other.exponent;
    return EncryptedNumber(this->public_key, product, exponent);
}

EncryptedNumber EncryptedNumber::operator*(long other) {
    EncodedNumber encoding = EncodedNumber::encode(this->public_key, other);
    return this->operator*(encoding);
}

EncryptedNumber EncryptedNumber::operator*(float other) {
    EncodedNumber encoding = EncodedNumber::encode(this->public_key, other);
    return this->operator*(encoding);
}

// Division
EncryptedNumber EncryptedNumber::operator/(long scalar) {
    return this->operator*(1.0f / scalar);
}

EncryptedNumber EncryptedNumber::operator/(float scalar) {
    return this->operator*(1 / scalar);
}

mpz_class EncryptedNumber::ciphertext(bool be_secure) {
    if (be_secure && this->_is_obfuscated) {
        this->obfuscate();
    }

    return this->_ciphertext;
}

EncryptedNumber EncryptedNumber::decrease_exponent_to(long new_exp) {
    if (new_exp > this->exponent) {
        throw std::runtime_error(
            std::string("New exponent should be smaller than old exponent."));
    }

    EncryptedNumber multiplied =
        this->operator*((long)pow(BASE, this->exponent - new_exp));
    multiplied.exponent = new_exp;

    return multiplied;
}

void EncryptedNumber::obfuscate() {
    mpz_class r = this->public_key->get_random_lt_n();
    mpz_class r_pow_n;
    mpz_powm(r_pow_n.get_mpz_t(), r.get_mpz_t(), this->public_key->n.get_mpz_t(),
             this->public_key->nsquare.get_mpz_t());
    this->_ciphertext = this->_ciphertext * r_pow_n % this->public_key->nsquare;
    this->_is_obfuscated = true;
}

EncryptedNumber EncryptedNumber::_add_encoded(EncodedNumber& encoded) {
    if (this->public_key != encoded.public_key) {
        throw std::runtime_error(std::string(
            "Attempted to add numbers encoded against different public keys."));
    }

    EncryptedNumber a = *this;
    EncodedNumber b = encoded;
    if (this->exponent > encoded.exponent) {
        a = a.decrease_exponent_to(b.exponent);
    } else if (a.exponent < b.exponent) {
        b = b.decrease_exponent_to(a.exponent);
    }

    mpz_class encrypted_scalar = a.public_key->raw_encrypt(b.encoding, 1);
    mpz_class sum_ciphertext =
        a._raw_add(a.ciphertext(false), encrypted_scalar);

    return EncryptedNumber(a.public_key, sum_ciphertext, a.exponent);
}

EncryptedNumber EncryptedNumber::_add_encrypted(EncryptedNumber& other) {
    if (this->public_key != other.public_key) {
        throw std::runtime_error(std::string(
            "Attempted to add numbers encoded against different public keys."));
    }

    EncryptedNumber a = *this;
    EncryptedNumber b = other;
    if (this->exponent > other.exponent) {
        a = a.decrease_exponent_to(b.exponent);
    } else if (a.exponent < b.exponent) {
        b = b.decrease_exponent_to(a.exponent);
    }

    mpz_class sum_ciphertext =
        a._raw_add(a.ciphertext(false), b.ciphertext(false));

    return EncryptedNumber(a.public_key, sum_ciphertext, a.exponent);
}

EncryptedNumber EncryptedNumber::_add_scalar(long scalar) {
    EncodedNumber encoded =
        EncodedNumber::encode(this->public_key, scalar, this->exponent);
    return this->_add_encoded(encoded);
}
EncryptedNumber EncryptedNumber::_add_scalar(float scalar) {
    EncodedNumber encoded =
        EncodedNumber::encode(this->public_key, scalar, this->exponent);
    return this->_add_encoded(encoded);
}

mpz_class EncryptedNumber::_raw_add(mpz_class e_a, mpz_class e_b) {
    return e_a * e_b % this->public_key->nsquare;
}

mpz_class EncryptedNumber::_raw_mult(mpz_class plaintext) {
    if (plaintext < 0 || plaintext >= this->public_key->max_int) {
        throw std::runtime_error(std::string("Scalar out of bounds."));
    }

    if (this->public_key->n - this->public_key->max_int <= plaintext) {
        mpz_class neg_c;
        mpz_invert(neg_c.get_mpz_t(), this->ciphertext(false).get_mpz_t(), this->public_key->nsquare.get_mpz_t());
        mpz_class neg_scalar = this->public_key->n - plaintext;
        mpz_class ret;
        mpz_powm(ret.get_mpz_t(), neg_c.get_mpz_t(), neg_scalar.get_mpz_t(),
                 this->public_key->nsquare.get_mpz_t());
        return ret;
    }

    mpz_class ret;
    mpz_powm(ret.get_mpz_t(), this->ciphertext(false).get_mpz_t(),
             plaintext.get_mpz_t(), this->public_key->nsquare.get_mpz_t());
    return ret;
}