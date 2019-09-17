#include "encoded_number.h"

EncodedNumber::EncodedNumber(PaillierPublicKey* public_key, long encoding,
                             long exponent) {
    this->public_key = public_key;
    this->encoding = encoding;
    this->exponent = exponent;
}

EncodedNumber EncodedNumber::encode(PaillierPublicKey* public_key, long scalar,
                            float precision, long max_exponent) {
    long prec_exponent = 0;
    if (precision != 0) {
        prec_exponent = (long)floorf(logb(precision, BASE));
    }

    long exponent = prec_exponent;
    if (max_exponent != 0) {
        exponent = std::min(max_exponent, prec_exponent);
    }

    long int_rep = roundf(scalar * pow(BASE, -exponent));
    if (abs(int_rep) > public_key->max_int) {
        throw std::runtime_error(std::string("Integer must be within +/- public key max int."));
    }

    mpz_class int_rep_out = int_rep % public_key->n;
    long int_rep_o = mpz_get_si(int_rep_out.get_mpz_t());

    return EncodedNumber(public_key, int_rep_o, exponent);
}

EncodedNumber EncodedNumber::encode(PaillierPublicKey* public_key, float scalar,
                            float precision, long max_exponent) {
    
    long prec_exponent;

    if (precision == 0) {
        int bin_flt_exponent;
        frexp(scalar, &bin_flt_exponent);
        long bin_lsb_exponent = bin_flt_exponent - DBL_MANT_DIG;
        prec_exponent = floor(bin_lsb_exponent / LOG2_BASE);
    } else {
        prec_exponent = (long)floorf(logb(precision, BASE));
    }

    long exponent = prec_exponent;
    if (max_exponent != 0) {
        exponent = std::min(max_exponent, prec_exponent);
    }

    long int_rep = roundf(scalar * pow(BASE, -exponent));
    // printf("%ld, %f, %ld\n", int_rep, scalar, exponent);
    if (abs(int_rep) > public_key->max_int) {
        throw std::runtime_error(std::string("Integer must be within +/- public key max int."));
    }

    mpz_class int_rep_out = int_rep % public_key->n;
    long int_rep_o = mpz_get_si(int_rep_out.get_mpz_t());

    return EncodedNumber(public_key, int_rep_o, exponent);
}

// float (mpfr) by default?
float EncodedNumber::decode() {
    mpz_class mantissa = 0;
    if (this->encoding >= this->public_key->n) {
        throw std::runtime_error(std::string("Attempted to decode corrupted number."));
    } else if (this->encoding <= this->public_key->max_int) {
        mantissa = this->encoding;
    } else if (this->encoding >= this->public_key->n - this->public_key->max_int) {
        mantissa = this->encoding - this->public_key->n;
    } else {
        throw std::runtime_error(std::string("Overflow detected in decrypted number."));
    }
    mpfr_class base = BASE;
    mpfr_class out;
    mpfr_pow_si(out.get_mpfr_t(), base.get_mpfr_t(), this->exponent, MPFR_RNDD);
    mpfr_class output = mantissa * out;
    return mpfr_get_flt(output.get_mpfr_t(), MPFR_RNDD);
}

EncodedNumber EncodedNumber::decrease_exponent_to(long new_exp) {
    if (new_exp > this->exponent) {
        throw std::runtime_error(std::string("New exponent should be smaller than old exponent."));
    }

    long factor = pow(BASE, this->exponent - new_exp);
    mpz_class new_enc = this->encoding * factor % this->public_key->n;
    long n_enc = mpz_get_si(new_enc.get_mpz_t());

    return EncodedNumber(this->public_key, n_enc, new_exp);
}

float logb(float x, long base) { return log(x) / log(base); }