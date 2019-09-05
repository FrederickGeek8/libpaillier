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
    
    EncodedNumber(PaillierPublicKey* public_key, int encoding, int exponent);
    static EncodedNumber encode(PaillierPublicKey* public_key, int scalar,
                                float precision = 0,
                                int max_exponent = 0);
    static EncodedNumber encode(PaillierPublicKey* public_key, float scalar,
                                float precision = 0,
                                int max_exponent = 0);
    int decode();
    EncodedNumber decrease_exponent_to(int new_exp);
    PaillierPublicKey* public_key;
    int encoding;
    int exponent;
};

float logb(float x, int base);

#endif