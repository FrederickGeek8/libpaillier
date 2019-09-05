#include "encoded_number.h"

EncodedNumber::EncodedNumber(PaillierPublicKey* public_key, int encoding,
                             int exponent) {
    this->public_key = public_key;
    this->encoding = encoding;
    this->exponent = exponent;
}

EncodedNumber EncodedNumber::encode(PaillierPublicKey* public_key, int scalar,
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
    if (abs(int_rep) > public_key->n) {
        throw std::runtime_error(std::string("Integer must be within +/- public key max int."));
    }

    return EncodedNumber(public_key, int_rep % public_key->n, exponent);
}

EncodedNumber EncodedNumber::encode(PaillierPublicKey* public_key, float scalar,
                            float precision, int max_exponent) {
    
    int prec_exponent;

    if (precision == 0) {
        int bin_flt_exponent;
        frexp(scalar, &bin_flt_exponent);
        int bin_lsb_exponent = bin_flt_exponent - FLT_MANT_DIG;
        prec_exponent = floor(bin_lsb_exponent / LOG2_BASE);
    } else {
        prec_exponent = (int)floorf(logb(precision, BASE));
    }

    int exponent = prec_exponent;
    if (max_exponent != 0) {
        exponent = std::min(max_exponent, prec_exponent);
    }

    int int_rep = roundf(scalar * pow(BASE, -exponent));
    if (abs(int_rep) > public_key->n) {
        throw std::runtime_error(std::string("Integer must be within +/- public key max int."));
    }

    return EncodedNumber(public_key, int_rep % public_key->n, exponent);
}

int EncodedNumber::decode() {
    int mantissa = 0;
    if (this->encoding >= this->public_key->n) {
        throw std::runtime_error(std::string("Attempted to decode corrupted number."));
    } else if (this->encoding <= this->public_key->max_int) {
        mantissa = this->encoding;
    } else if (this->encoding >= this->public_key->n - this->public_key->max_int) {
        mantissa = this->encoding - this->public_key->n;
    } else {
        throw std::runtime_error(std::string("Overflow detected in decrypted number."));
    }

    return mantissa * pow(BASE, this->exponent);
}

EncodedNumber EncodedNumber::decrease_exponent_to(int new_exp) {
    if (new_exp > this->exponent) {
        throw std::runtime_error(std::string("New exponent should be smaller than old exponent."));
    }

    int factor = pow(BASE, this->exponent - new_exp);
    int new_enc = this->encoding * factor % this->public_key->n;

    return EncodedNumber(this->public_key, new_enc, new_exp);
}

float logb(float x, int base) { return log(x) / log(base); }