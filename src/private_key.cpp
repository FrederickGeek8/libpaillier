#include "private_key.h"

PaillierPrivateKey::PaillierPrivateKey(PaillierPublicKey* public_key,
                                       mpz_class p, mpz_class q) {
    if (p * q != public_key->n) {
        throw std::runtime_error(std::string("Given public key does not match the given p and q."));
    }

    if (p == q) {
        throw std::runtime_error(std::string("p and q must be different."));
    }
    
    this->public_key = public_key;
    if (q < p) {
        this->p = q;
        this->q = p;
    } else {
        this->p = p;
        this->q = q;
    }

    this->psquare = this->p * this->p;
    this->qsquare = this->q * this->q;
    mpz_invert(this->p_inverse.get_mpz_t(), this->p.get_mpz_t(), this->q.get_mpz_t());
    this->hp = this->h_function(this->p, this->psquare);
    this->hq = this->h_function(this->q, this->qsquare);
}

PaillierPrivateKey PaillierPrivateKey::from_totient(PaillierPublicKey* public_key, mpz_class totient) {
    mpz_class p_plus_q = public_key->n - totient + 1;
    mpz_class p_minus_q;
    mpz_class tmp = p_plus_q * p_plus_q - public_key->n * 4;
    mpz_sqrt(p_minus_q.get_mpz_t(), tmp.get_mpz_t());
    mpz_class q = (p_plus_q - p_minus_q) / 2;
    mpz_class p = p_plus_q - q;

    if (p * q != public_key->n) {
        throw std::runtime_error(std::string("Given public key and totient do not match."));
    }

    return PaillierPrivateKey(public_key, p, q);
}

float PaillierPrivateKey::decrypt(EncryptedNumber& encrypted_number) {
    EncodedNumber encoded = this->decrypt_encoded(encrypted_number);
    return encoded.decode();
}

EncodedNumber PaillierPrivateKey::decrypt_encoded(
    EncryptedNumber& encrypted_number) {

    if (this->public_key != encrypted_number.public_key) {
        throw std::runtime_error(std::string(
            "Encrypted number was encrypted against a different key!"));
    }

    long encoded = this->raw_decrypt(encrypted_number.ciphertext(false));

    return EncodedNumber(this->public_key, encoded, encrypted_number.exponent);
}

long PaillierPrivateKey::raw_decrypt(mpz_class ciphertext) {
    mpz_class p;
    mpz_class q;
    mpz_class p_m_1 = this->p - 1;
    mpz_class q_m_1 = this->q - 1;

    mpz_powm(p.get_mpz_t(), ciphertext.get_mpz_t(), p_m_1.get_mpz_t(), this->psquare.get_mpz_t());
    mpz_powm(q.get_mpz_t(), ciphertext.get_mpz_t(), q_m_1.get_mpz_t(), this->qsquare.get_mpz_t());
    
    mpz_class decrypt_to_p = this->l_function(p, this->p) * this->hp % this->p;
    mpz_class decrypt_to_q = this->l_function(q, this->q) * this->hq % this->q;
    
    return this->crt(decrypt_to_p, decrypt_to_q);
}

long PaillierPrivateKey::crt(mpz_class mp, mpz_class mq) {
    mpz_class u = (mq - mp) * this->p_inverse % this->q;
    mpz_class ret = mp + (u * this->p);
    return mpz_get_si(ret.get_mpz_t());
}

mpz_class PaillierPrivateKey::l_function(mpz_class x, mpz_class p) {
    return (x - 1) / p;
}

mpz_class PaillierPrivateKey::h_function(mpz_class x, mpz_class x_square) {
    mpz_class ret;
    mpz_class pmod;
    mpz_class x_m_1 = x - 1;
    mpz_powm(pmod.get_mpz_t(), this->public_key->g.get_mpz_t(), x_m_1.get_mpz_t(), x_square.get_mpz_t());
    mpz_invert(ret.get_mpz_t(), this->l_function(pmod, x).get_mpz_t(), x.get_mpz_t());

    return ret;
}