#include <iostream>
#include <stdio.h>
#include "declaration.h"

#ifndef __utils__
#define __utils__

typedef struct {
    mpz_class r;
    mpz_class s;
    mpz_class t;
} ext_gcd;

typedef struct {
    PaillierPublicKey* pub_key;
    PaillierPrivateKey* priv_key;
} keypair;

// mpz_class powmod(mpz_class base, mpz_class exp, mpz_class p);
// ext_gcd egcd(mpz_class a, mpz_class b);
// mpz_class invert(mpz_class a, mpz_class b);
// bool miller_rabin(long n, long k);
// bool is_prime(long n, long mr_rounds = 25);
mpz_class getprimeover(long N);
keypair generate_paillier_keypair(long n_length);

#endif