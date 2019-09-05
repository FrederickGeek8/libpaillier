#include <iostream>
#include <float.h>
#include <math.h>
#include "public_key.h"

#ifndef BASE
#define BASE 16
#endif

class EncodedNumber {
   public:
    PaillierPublicKey public_key;
    int encoding;
    int exponent;
    int LOG2_BASE;
    EncodedNumber(PaillierPublicKey public_key, int encoding, int exponent);
    static EncodedNumber encode(PaillierPublicKey public_key, int scalar,
                                float precision = 0,
                                int max_exponent = 0);
    static EncodedNumber encode(PaillierPublicKey public_key, float scalar,
                                float precision = 0,
                                int max_exponent = 0);
    int decode();
    EncodedNumber decrease_exponent_to(int new_exp);
};

float logb(float x, int base);