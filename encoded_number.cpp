#include "encoded_number.h"

EncodedNumber::EncodedNumber(PaillierPublicKey public_key, int encoding,
                             int exponent) {
    this->public_key = public_key;
    this->encoding = encoding;
    this->exponent = exponent;
    this->LOG2_BASE = log2(BASE);
}

static EncodedNumber encode(PaillierPublicKey public_key, int scalar,
                            float precision, int max_exponent) {
    int prec_exponent = 0;
    if (precision != 0) {
        prec_exponent = (int)floorf(logb(precision, BASE));
    }

    int exponent = prec_exponent;
    if (max_exponent != 0) {
        exponent = std::min(max_exponent, prec_exponent);
    }

    int int_rep = roundf(scalar * pow(BASE, -exponent));
    if (abs(int_rep) > public_key.n) {
        throw std::string("Integer must be within +/- public key max int.");
    }

    return EncodedNumber(public_key, int_rep % public_key.n, exponent);
}

int EncodedNumber::decode() {
    int mantissa = 0;
    if (this->encoding >= this->public_key.n) {
        throw std::string("Attempted to decode corrupted number.");
    } else if (this->encoding <= this->public_key.max_int) {
        mantissa = this->encoding;
    } else if (this->encoding >= this->public_key.n - this->public_key.max_int) {
        mantissa = this->encoding - this->public_key.n;
    } else {
        throw std::string("Overflow detected in decrypted number.");
    }

    return mantissa * pow(BASE, this->exponent);
}

EncodedNumber EncodedNumber::decrease_exponent_to(int new_exp) {
    if (new_exp > this->exponent) {
        throw std::string("New exponent should be smaller than old exponent.");
    }

    int factor = pow(BASE, this->exponent - new_exp);
    int new_enc = this->encoding * factor % this->public_key.n;

    return EncodedNumber(this->public_key, new_enc, new_exp);
}

float logb(float x, int base) { return log(x) / log(base); }