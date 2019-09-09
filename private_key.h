#include "declaration.h"

#ifndef __PaillierPrivateKey__
#define __PaillierPrivateKey__

class PaillierPrivateKey {
    public:
        PaillierPrivateKey(PaillierPublicKey* public_key, long p, long q);
        PaillierPublicKey* public_key;
        long p;
        long q;
        long psquare;
        long qsquare;
        long p_inverse;
        long hp;
        long hq;

        static PaillierPrivateKey from_totient(PaillierPublicKey* public_key, long totient);
}

#endif