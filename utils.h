#include <iostream>
#include <stdio.h>
#include "declaration.h"

#ifndef __utils__
#define __utils__

typedef struct {
    int r;
    int s;
    int t;
} ext_gcd;

typedef struct {
    PaillierPublicKey pub_key;
} keypair;

unsigned long powmod(unsigned long base, unsigned long exp, unsigned long p);
ext_gcd egcd(int a, int b);
int invert(int a, int b);
bool miller_rabin(int n, int k);
bool is_prime(int n, int mr_rounds = 25);
int getprimeover(int N);
keypair generate_paillier_keypair(int n_length);

#endif