#include "test_math.h"

void test_power()
{
    printf("Test of \"tmp::power<BASE,POWER>\":\n");
    printf("2^0 = %d\n", tmp::math::power<2,0>);
    printf("2^1 = %llu\n", tmp::math::power<2,1>);
    printf("2^2 = %llu\n", tmp::math::power<2,2>);
    printf("4^0 = %d\n", tmp::math::power<4,0>);
    printf("4^2 = %llu\n", tmp::math::power<4,2>);
    printf("4^5 = %llu\n", tmp::math::power<4,5>);
    printf("\n");
}

void test_sum()
{
     printf("Test of \"tmp::sum<VS...> and tmp::sum_first<N,T<VS...>>\":\n");
     printf("1+2+3+4+5 = %llu\n", tmp::math::sum<1,2,3,4,5>);
     printf("1+2+3 = %d\n", tmp::math::sum_first<3,tmp::tuple_i<1,2,3,4,5>>);
     printf("\n");
}

void test_primes()
{
    printf("Test of \"tmp::detail::primes_impl<N>\":\n");
    typedef tmp::math::detail::primes_impl<1>::type V1_;
    typedef to_print_tuple<V1_>::type V1;
    typedef tmp::math::detail::primes_impl<2>::type V2_;
    typedef to_print_tuple<V2_>::type V2;
    typedef tmp::math::detail::primes_impl<3>::type V3_;
    typedef to_print_tuple<V3_>::type V3;
    typedef tmp::math::detail::primes_impl<23>::type V23_;
    typedef to_print_tuple<V23_>::type V23;
    printf("First prime is:\t");
    V1::printme();
    printf("\nThe first two primes are:\t");
    V2::printme();
    printf("\nThe first three primes are:\t");
    V3::printme();
    printf("\nThe first twentythree primes are:\t");
    V23::printme();
    printf("\nThirtieth prime:\t%d",tmp::math::prime<30>);

    printf("\n");
}