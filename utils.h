#include <iostream>
#include <stdio.h>
#include "declaration.h"

#ifndef __utils__
#define __utils__

typedef struct {
    long r;
    long s;
    long t;
} ext_gcd;

typedef struct {
    PaillierPublicKey pub_key;
} keypair;

unsigned long powmod(unsigned long base, unsigned long exp, unsigned long p);
ext_gcd egcd(long a, long b);
long invert(long a, long b);
bool miller_rabin(long n, long k);
bool is_prime(long n, long mr_rounds = 25);
long getprimeover(long N);
keypair generate_paillier_keypair(long n_length);

#endif