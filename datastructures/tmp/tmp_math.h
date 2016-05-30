#ifndef TMP_MATH_H
#define TMP_MATH_H

#include <cstddef>
#include "tmp.h"

namespace tmp
{
    namespace math
    {

        namespace detail
        {
            template <typename T>
            struct numbers_to_type_impl;
            template <size_t ... VS>
            struct numbers_to_type_impl<tuple_i<VS...>>
            {
                typedef tuple_t<int_type<VS...>> type;
            };
            template <typename T>
            struct extract_numbers_impl;
            template <size_t ... VS>
            struct extract_numbers_impl<int_type<VS>...>
            {
                typedef tuple_i<VS...> type;
            };
            template <typename T>
            using extract_numbers = typename extract_numbers_impl<T>::type;
            template <typename T>
            using numbers_to_type = typename numbers_to_type_impl<T>::type;

            template <typename,typename>
            struct operation
            {
                struct add;
                struct subtract;
                struct multiply;
                struct divide;
                struct modulus;
            };

            template <size_t A,size_t B>
            struct operation<int_type<A>,int_type<B>>
            {
                using add = int_type<A+B>;
                using subtract = int_type<A-B>;
                using multiply = int_type<A*B>;
                using divide = int_type<A/B>;
                using modulus = int_type<A%B>;
            };
        } // detail
        template <size_t BASE, size_t POWER>
        struct power
        {
            enum { value = power<BASE,POWER-1>::value * BASE };
        };

        template <size_t BASE>
        struct power<BASE,0>
        {
            enum { value = 1 };
        };

        template <size_t ... VS>
        struct sum
        {
            enum { value = 0 };
        };
        template <size_t V, size_t ... VS>
        struct sum<V,VS...>
        {
            enum { value = V + sum<VS...>::value };
        };

        template <size_t N, typename T>
        struct sum_first;
        template <size_t N, template <size_t...> class T>
        struct sum_first<N, T<>>
        {
            enum { value = 0 };
        };
        template <template <size_t...> class T, size_t V, size_t ... VS>
        struct sum_first<0,T<V,VS...>>
        {
            enum { value = 0 };
        };
        template <size_t N, template <size_t...> class T, size_t V, size_t ... VS>
        struct sum_first<N, T<V,VS...>>
        {
            enum { value = V + sum_first<N-1,tuple_i<VS...>>::value };
        };



        namespace detail
        {
            template <size_t N, size_t D, typename ... DIVISORS>
            struct greatest_divisor_impl;
            template <size_t N, size_t D, typename T, typename ... TS>
            struct greatest_divisor_impl<N,D,T,TS...>
            {
                enum { value = ((D < T::value) && (N % T::value == 0)) ? greatest_divisor_impl<N,T::value,TS...>::value : greatest_divisor_impl<N,D,TS...>::value };
            };
            template <size_t N, size_t D>
            struct greatest_divisor_impl<N,D>
            {
                enum { value = D };
            };
        } // namespace detail

        template <size_t N, typename DIVISOR_SET>
        struct greatest_divisor;

        template <size_t N, template <typename ... INTTYPES> class SET, typename ... INTTYPES>
        struct greatest_divisor<N,SET<INTTYPES...>>
        {
            enum { value = detail::greatest_divisor_impl<N,1,INTTYPES...>::value };
        };

        namespace detail
        {
            template <typename C, typename PS>
            struct prime_test
            {
                enum { value = 1 };
            };

            template <size_t C, size_t P, typename ... TS>
            struct prime_test<int_type<C>,tuple_t<int_type<P>,TS...>>
            {
                enum { value = (P*P > C ? 1 : (C%P == 0 ? 0 : prime_test<int_type<C>,tuple_t<TS...>>::value)) };
            };

            template <typename PS, typename N, bool B>
            struct next_prime
            {
                using N_plus_two = typename operation<N,int_type<2>>::add;
                //using prime_test = typename product<is<N>::template divisible_by>::template of<PS>;
                using test = prime_test<N,PS>;
                typedef get_element_at<
                            test::value != 0,
                            typename next_prime<PS, N_plus_two, test::value == 0>::type,
                            N
                       > type;
            };

            template <typename PS, typename N>
            struct next_prime<PS,N,false>
            {
                typedef no_type type;
            };

            template <size_t N>
            struct primes_impl
            {
                using found_primes = typename primes_impl<N-1>::type;
                using previous_prime = typename primes_impl<N-1>::last_prime;
                using previous_prime_added_with_two = typename operation<previous_prime,int_type<2>>::add;

                using last_prime = typename next_prime<found_primes,previous_prime_added_with_two,true>::type;

                using type = typename append<found_primes>::template with<last_prime>;
            };

            template <>
            struct primes_impl<0>
            {
                typedef tuple_t<> type;
            };
            template <>
            struct primes_impl<1>
            {
                typedef int_type<2> last_prime;
                typedef tuple_t<int_type<2>> type;
            };
            template <>
            struct primes_impl<2>
            {
                typedef int_type<3> last_prime;
                typedef tuple_t<int_type<2>,last_prime> type;
            };

            template <size_t N, typename T>
            struct prime_impl;
            template <size_t N, typename ... TS>
            struct prime_impl<N, tuple_t<TS...>>
            {
                typedef get_element_at<N-1,TS...> type;
            };

        } // detail

        template <size_t N>
        using prime = typename detail::prime_impl<N,typename detail::primes_impl<N>::type>::type;

    } // math

} // namespace tmp























#endif // TMP_MATH_H
