#include "declaration.h"

#ifndef BASE
#define BASE 16
#endif

#ifndef LOG2_BASE
#define LOG2_BASE log2(BASE)
#endif

#ifndef __EncodedNumber__
#define __EncodedNumber__

class EncodedNumber {
   public:
    
    EncodedNumber(PaillierPublicKey* public_key, long encoding, long exponent);
    static EncodedNumber encode(PaillierPublicKey* public_key, long scalar,
                                float precision = 0,
                                long max_exponent = 0);
    static EncodedNumber encode(PaillierPublicKey* public_key, float scalar,
                                float precision = 0,
                                long max_exponent = 0);
    float decode();
    EncodedNumber decrease_exponent_to(long new_exp);
    PaillierPublicKey* public_key;
    long encoding;
    long exponent;
};

float logb(float x, long base);

#endif