#define CATCH_CONFIG_MAIN 
#include <catch2/catch.hpp>
#include "declaration.h"


TEST_CASE("Generate prime over length", "[prime]") {
    mpz_class prime = getprimeover(16);
    mpfr_class f_n;
    mpfr_class new_n;
    mpz_class floored;
    int is_prime = mpz_probab_prime_p(prime.get_mpz_t(), 25);
    REQUIRE((is_prime == 2 || is_prime == 1));

    mpfr_set_z(f_n.get_mpfr_t(), prime.get_mpz_t(), MPFR_RNDD);
    mpfr_log2(new_n.get_mpfr_t(), f_n.get_mpfr_t(), MPFR_RNDD);
    mpfr_floor(new_n.get_mpfr_t(), new_n.get_mpfr_t());
    mpfr_get_z(floored.get_mpz_t(), new_n.get_mpfr_t(), MPFR_RNDD);
    long n_len = mpz_get_ui(floored.get_mpz_t()) + 1;

    
    REQUIRE(n_len > 16);
}

TEST_CASE("Encoding / Decoding correctness", "[encoding]") {
    keypair test = generate_paillier_keypair(1024);
    SECTION("float encoding") {
        float plain = 5.12;
        EncodedNumber num = EncodedNumber::encode(test.pub_key, plain);
        REQUIRE(num.decode() == plain);
    }

    SECTION("integer encoding") {
        long plain2 = 10;
    
        EncodedNumber num2 = EncodedNumber::encode(test.pub_key, plain2);
        
        REQUIRE((int)num2.decode() == plain2);
    }
}

TEST_CASE("Encrypting / Decrypting correctness", "[encrypt]") {
    keypair test = generate_paillier_keypair(1024);
    SECTION("float encryption") {
        float plain = 5.12;
        EncryptedNumber num = test.pub_key->encrypt(plain);
        float decoded = test.priv_key->decrypt(num);
        REQUIRE(plain == decoded);
    }
    
    SECTION("integer encryption") {
        long plain2 = 10;
        EncryptedNumber num2 = test.pub_key->encrypt(plain2);
        int decoded2 = (int)test.priv_key->decrypt(num2);
        REQUIRE(plain2 == decoded2);
    }
    
}
