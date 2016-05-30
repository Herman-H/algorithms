#ifndef TEST_H
#define TEST_H

#include <cstdio>
#include "prime.h"

void test_eratosthenes()
{
    //printf("Is 143 prime? %s\n", eratosthenes_sieve<143>().prime() ? "true" : "false");
    //printf("Is 81 prime? %s\n", eratosthenes_sieve<81>().prime() ? "true" : "false");
    //printf("Is 80 prime? %s\n", eratosthenes_sieve<80>().prime() ? "true" : "false");
    //printf("Is 82 prime? %s\n", eratosthenes_sieve<82>().prime() ? "true" : "false");
    printf("Is 2 prime? %s\n", alg::is_prime(2) ? "true" : "false");
    printf("Is 3 prime? %s\n", alg::is_prime(3) ? "true" : "false");
    printf("Is 4 prime? %s\n", alg::is_prime(4) ? "true" : "false");
    printf("Is 5 prime? %s\n", alg::is_prime(5) ? "true" : "false");
    printf("Is 6 prime? %s\n", alg::is_prime(6) ? "true" : "false");
    printf("Is 7 prime? %s\n", alg::is_prime(7) ? "true" : "false");
    printf("Is 8 prime? %s\n", alg::is_prime(8) ? "true" : "false");
    printf("Is 9 prime? %s\n", alg::is_prime(9) ? "true" : "false");
    printf("Is 10 prime? %s\n", alg::is_prime(10) ? "true" : "false");
    printf("Is 11 prime? %s\n", alg::is_prime(11) ? "true" : "false");
    printf("Is 12 prime? %s\n", alg::is_prime(12) ? "true" : "false");
    printf("Is 13 prime? %s\n", alg::is_prime(13) ? "true" : "false");
    printf("Is 14 prime? %s\n", alg::is_prime(14) ? "true" : "false");
    printf("Is 15 prime? %s\n", alg::is_prime(15) ? "true" : "false");
    printf("Is 16 prime? %s\n", alg::is_prime(16) ? "true" : "false");
    printf("Is 17 prime? %s\n", alg::is_prime(17) ? "true" : "false");
    printf("Is 18 prime? %s\n", alg::is_prime(18) ? "true" : "false");
    printf("Is 19 prime? %s\n", alg::is_prime(19) ? "true" : "false");
    printf("Is 20 prime? %s\n", alg::is_prime(20) ? "true" : "false");
    printf("Is 21 prime? %s\n", alg::is_prime(21) ? "true" : "false");
    printf("Is 22 prime? %s\n", alg::is_prime(22) ? "true" : "false");
    printf("Is 23 prime? %s\n", alg::is_prime(23) ? "true" : "false");
    printf("Is 24 prime? %s\n", alg::is_prime(24) ? "true" : "false");
    printf("Is 25 prime? %s\n", alg::is_prime(25) ? "true" : "false");
    printf("Is 26 prime? %s\n", alg::is_prime(26) ? "true" : "false");
    printf("Is 27 prime? %s\n", alg::is_prime(27) ? "true" : "false");
    printf("Is 28 prime? %s\n", alg::is_prime(28) ? "true" : "false");
    printf("Is 29 prime? %s\n", alg::is_prime(29) ? "true" : "false");

    for(std::size_t i = 2; i < 20; ++i)
    {
        printf("Is %d prime? %s\n", i, alg::is_prime(i) ? "true" : "false");
    }
    printf("Is %d prime? %s\n", 1013, alg::is_prime<1013>() ? "true" : "false");
}

#endif /* TEST_H */
