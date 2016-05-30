#ifndef PRIME_H
#define PRIME_H

#include <cstddef>
#include "datastructures/tmp/tmp_math.h"
#include "datastructures/tmp/tmp_types.h"
#include "datastructures/tmp/tmp_type_traits.h"
#include <type_traits>
#include <cstdio>

namespace alg
{

    namespace detail
    {
        template <std::size_t N>
        class eratosthenes_sieve_static_prime
        {
        private:
            static constexpr std::size_t mid_point(const std::size_t l, const std::size_t u)
            {
                return (l+u)/2;
            }
            static constexpr std::size_t square(const std::size_t a)
            {
                return a*a;
            }
            static constexpr std::size_t find_sqrt_N(const std::size_t l, const std::size_t u)
            {
                return square(mid_point(l,u)) == N ?
                            mid_point(l,u) :
                            (l == u) ?
                                l-1 :
                                (square(mid_point(l,u)) > N ?
                                     find_sqrt_N(l,mid_point(l,u)) :
                                     find_sqrt_N(mid_point(l,u)+1,u));
            }
            enum { limit = tmp::math::power<2,tmp::number_of_bits_of_fundamental<std::size_t>::value/2>::value-1 };
            enum { sqrt_N = find_sqrt_N(0,limit) };
            bool isnt_prime[sqrt_N+1] = {0};
            bool prime_;
            void init_is_primes()
            {
                for(int i = 2; i <= sqrt_N; ++i)
                {
                    if(!isnt_prime[i])
                    {
                        for(int j = i*i; j <= sqrt_N; j+=i*i)
                        {
                            isnt_prime[j] = true;
                        }
                    }
                }
            }
            bool isprime_() const
            {
                for(int i = 2; i <= sqrt_N; ++i)
                {
                    if(!isnt_prime[i])
                    {
                        if(N % i == 0)
                        {
                            return false;
                        }
                    }
                }
                return true;
            }

        public:
            bool prime() const { return prime_; }
            eratosthenes_sieve_static_prime()
            {
                init_is_primes();
                prime_ = isprime_();
            }
        };
        class eratosthenes_sieve_dynamic_prime
        {
        private:
            static std::size_t mid_point(const std::size_t l, const std::size_t u)
            {
                return (l+u)/2;
            }
            static std::size_t square(const std::size_t a)
            {
                return a*a;
            }
            static std::size_t find_sqrt_N(const std::size_t l, const std::size_t u, const std::size_t N)
            {
                return square(mid_point(l,u)) == N ?
                            mid_point(l,u) :
                            (l == u) ?
                                l-1 :
                                (square(mid_point(l,u)) > N ?
                                     find_sqrt_N(l,mid_point(l,u),N) :
                                     find_sqrt_N(mid_point(l,u)+1,u,N));
            }
            enum { limit = tmp::math::power<2,tmp::number_of_bits_of_fundamental<std::size_t>::value/2>::value-1 };
            std::size_t sqrt_N;
            bool * isnt_prime;
            bool prime_;
            void init_is_primes()
            {
                for(int i = 2; i <= sqrt_N; ++i)
                {
                    if(!isnt_prime[i])
                    {
                        for(int j = i*i; j <= sqrt_N; j+=i*i)
                        {
                            isnt_prime[j] = true;
                        }
                    }
                }
            }
            bool isprime_(std::size_t N) const
            {
                for(int i = 2; i <= sqrt_N; ++i)
                {
                    if(!isnt_prime[i])
                    {
                        if(N % i == 0)
                        {
                            return false;
                        }
                    }
                }
                return true;
            }

        public:
            bool prime() const { return prime_; }
            eratosthenes_sieve_dynamic_prime(std::size_t N)
            {
                sqrt_N = find_sqrt_N(0,limit,N);
                isnt_prime = new bool[sqrt_N+1];
                init_is_primes();
                prime_ = isprime_(N);
                delete [] isnt_prime;
            }
        };
    } // detail

    template <size_t N>
    static bool is_prime()
    {
        return detail::eratosthenes_sieve_static_prime<N>{}.prime();
    }

    static bool is_prime(std::size_t c)
    {
        return detail::eratosthenes_sieve_dynamic_prime{c}.prime();
    }

    namespace detail
    {
        template <std::size_t N>
        class eratosthenes_sieve_static_primes
        {
        private:
            static constexpr std::size_t mid_point(const std::size_t l, const std::size_t u)
            {
                return (l+u)/2;
            }
            static constexpr std::size_t square(const std::size_t a)
            {
                return a*a;
            }
            static constexpr std::size_t find_sqrt_N(const std::size_t l, const std::size_t u)
            {
                return square(mid_point(l,u)) == N ?
                            mid_point(l,u) :
                            (l == u) ?
                                l-1 :
                                (square(mid_point(l,u)) > N ?
                                     find_sqrt_N(l,mid_point(l,u)) :
                                     find_sqrt_N(mid_point(l,u)+1,u));
            }
            enum { limit = tmp::math::power<2,tmp::number_of_bits_of_fundamental<std::size_t>::value/2>::value-1 };
            enum { sqrt_N = find_sqrt_N(0,limit) };
            bool isnt_prime[sqrt_N+1] = {0};

            void init_is_primes()
            {
                for(int i = 2; i <= sqrt_N; ++i)
                {
                    if(!isnt_prime[i])
                    {
                        for(int j = i*i; j <= sqrt_N; j+=i*i)
                        {
                            isnt_prime[j] = true;
                        }
                    }
                }
            }
            bool isprime_() const
            {
                for(int i = 2; i <= sqrt_N; ++i)
                {
                    if(!isnt_prime[i])
                    {
                        if(N % i == 0)
                        {
                            return false;
                        }
                    }
                }
                return true;
            }

        public:
            bool prime() const { return prime_; }
            eratosthenes_sieve_static_prime()
            {
                init_is_primes();
                prime_ = isprime_();
            }
        };
        class eratosthenes_sieve_dynamic_primes
        {

        };
    }
} // alg

















#endif /* PRIME_H */
