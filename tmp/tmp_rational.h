#ifndef TMP_RATIONAL_H
#define TMP_RATIONAL_H

#include "tmp.h"
#include "tmp_math.h"
#include "tmp_types.h"
#include <utility>

/*
 *      Q = Rs*Rd. Here Rs is the static factor of the rational number, and Rd is the dynamic factor.
 *
 *      We need a mapping f(Q) that computes the floating point number for Q. We call this the projection of Q.
 *
 *      Rs = SN/SD where both integers SN and SD has underlying type "signed_int_max".
 *
 *      Rd = (<X,Y>,<TN,TD>) where X is the nominator component and Y is the denominator component. TN is the type of the nominator component and TD
 *      is the type of the denominator component. For example (<1,0>,<int,N/A>) means that Rd can only store one int which is the nominator. (<0,1>,<N/A,int>)
 *      will specify the denominator component stored as an int and last we have (<1,1>,<long,int>) we have a rational number with nominator being a long
 *      and denominator being an int. A (<0,0>,<N/A,N/A>) will mean that the dynamic factor is 1 and thus Q is a fully static rational number.
 *
 *      We need operators / * + - so that we have Q1/Q2, Q1*Q2, Q1+Q2, Q1-Q2
 *      
 *      For each operator we have a corresponding resulting type operator, T/ T* T+ T- so that we have (Q1 T/ Q2), (Q1 T* Q2), (Q1 T+ Q2), (Q1 T- Q2)
 *      For all operators we have (Q1 T+ Q2) = (Q1 T- Q2) = (Q2 T+ Q1) = (Q2 T- Q1). Why? First we have commutative addition which
 *      must equalize (Q1 T+ Q2) and (Q2 T+ Q1). The rest depends on the fact that the type is simply the "best fitting type" which
 *      must be the same since addition and subtraction can be defined by each other by multiplying one of the terms by -1. We can also break out a static
 *      constant factor from both terms which will be the same term in all cases. We call + and - "term binding".
 *      For * we have (Q1 T* Q2) = (Q2 T* Q1)
 *
 *      "Term binding" means that we have to first equalize the denominators of each term so that we then can sum the equalized nominators. We have to
 *      cross multiply static factors here.
 *
 *      For Rd: (<0,0>,...) T+ (<0,0>,...) = (<0,0>,...) T (<0,0>)
 *              (<0,0>,...) T+ (<1,0>,...) = (<1,0>,...) T (<YN-(XSD-YSD)-XSN-YSN,0>)
 *              (<0,0>,...) T+ (<0,1>,...) = (<1,1>,...) T (<
 *              (<0,0>,...) T+ (<1,1>,...) = (<1,1>,...)
 *              (<1,0>,...) T+ (<1,0>,...) = (<1,0>,...)
 *              (<1,0>,...) T+ (<0,1>,...) = (<1,1>,...)
 *              (<1,0>,...) T+ (<1,1>,...) = (<1,1>,...)
 *              (<0,1>,...) T+ (<0,1>,...) = (<1,1>,...)
 *              (<0,1>,...) T+ (<1,1>,...) = (<1,1>,...)
 *              (<1,1>,...) T+ (<1,1>,...) = (<1,1>,...)
 *
 *              (<0,0>,...) T* (<0,0>,...) = (<0,0>,...)
 *              (<0,0>,...) T* (<1,0>,...) = (<1,0>,...)
 *              (<0,0>,...) T* (<0,1>,...) = (<0,1>,...)
 *              (<0,0>,...) T* (<1,1>,...) = (<1,1>,...)
 *              (<1,0>,...) T* (<1,0>,...) = (<1,0>,...)
 *              (<1,0>,...) T* (<0,1>,...) = (<1,1>,...)
 *              (<1,0>,...) T* (<1,1>,...) = (<1,1>,...)
 *              (<0,1>,...) T* (<0,1>,...) = (<0,1>,...)
 *              (<0,1>,...) T* (<1,1>,...) = (<1,1>,...)
 *              (<1,1>,...) T* (<1,1>,...) = (<1,1>,...)
 *
 *              (<0,0>,...) T/ (<0,0>,...) = (<0,0>,...)
 *              (<0,0>,...) T/ (<1,0>,...) = (<0,1>,...)
 *              (<0,0>,...) T/ (<0,1>,...) = (<1,0>,...)
 *              (<0,0>,...) T/ (<1,1>,...) = (<1,1>,...)
 *              (<1,0>,...) T/ (<1,0>,...) = (<1,1>,...)
 *              (<1,0>,...) T/ (<0,1>,...) = (<1,0>,...)
 *              (<1,0>,...) T/ (<1,1>,...) = (<1,1>,...)
 *              (<0,1>,...) T/ (<1,0>,...) = (<0,1>,...)
 *              (<0,1>,...) T/ (<0,1>,...) = (<1,1>,...)
 *              (<0,1>,...) T/ (<1,1>,...) = (<1,1>,...)
 *              (<1,1>,...) T/ (<1,1>,...) = (<1,1>,...)
 *              (<1,1>,...) T/ (<1,0>,...) = (<1,1>,...)
 *              (<1,1>,...) T/ (<0,1>,...) = (<1,1>,...)
 *
 *      We can easily spot patterns here. For example, for T+ and T- we have (<X1,Y1>) OP (<X2,Y2>) = (<(X1 | Y1 | X2 | Y2),!(Y1 and Y2)>)
 *                                                     for T* we have        (<X1,Y1>) T* (<X2,Y2>) = (<(X1 or X2), (Y1 or Y2)>)
 *                                                     for T/ we have        (<X1,Y1>) T/ (<X2,Y2>) = (<(X1 or Y2), (X2 or Y1)>)
 *
 *
 */

namespace tmp
{
    namespace rational
    {
        namespace detail
        {
            template <typename T>
            struct type_to_size_impl
            {
                enum { value = tmp::types::number_of_bits_of_fundamental<T> };
            };
            
            template <typename T>
            constexpr int type_to_size = type_to_size_impl<T>::value;
            
            /* Use this to dodge compiler warnings using enums in conditional expressions, int is arbitrary but it can easily hold the type sizes */
            template <typename T>
            constexpr int type_size_value = T::value;
            
            template <typename T1, typename T2>
            struct max_op
            {
                enum { value = (type_size_value<T1> > type_size_value<T2>) ? type_size_value<T1> : type_size_value<T2> };
            };
            
            template <int N>
            struct find_min_containing_type
            {
            };
            
            constexpr int signed_int_max_size = fold<max_op,type_to_size_impl,int_type<0>>::on<tmp::types::detail::signed_fundamentals>::value;
            template <int N>
            struct min_type_size_impl
            {
                template <typename T1, typename T2>
                struct op
                {
                    enum { value = (type_size_value<T1> < type_size_value<T2> && type_size_value<T1> >= N+1) ? type_size_value<T1> : type_size_value<T2> };
                };
                
                enum { value = fold<op,type_to_size_impl,int_type<signed_int_max_size>>::template on<tmp::types::detail::signed_fundamentals>::value };
            };
            template <int N>
            constexpr int min_type_size = min_type_size_impl<N>::value;
            
            using signed_int_max = tmp::types::get_signed_fundamental_of_bit_size<signed_int_max_size>;
                                            
            template <signed_int_max Z>
            constexpr signed_int_max absolute_value = Z >= 0 ? Z : -Z;
            
            template <signed_int_max P1, signed_int_max P2>
            struct gcd_impl
            {
                enum { value = gcd_impl<(P1 > P2 ? P2 : P1), (P1 > P2 ? P1%P2 : P2%P1)>::value };
            };
            
            template <signed_int_max P1>
            struct gcd_impl<P1,0>
            {
                enum { value = P1 };
            };
            
            template <signed_int_max P1, signed_int_max P2>
            constexpr signed_int_max gcd = gcd_impl<absolute_value<P1>,absolute_value<P2>>::value;
            
            
            template <signed_int_max N, size_t BITS>
            struct significant_bits_impl
            {
                enum { value = significant_bits_impl<(N >> 1),BITS+1>::value };
            };
            template <size_t BITS>
            struct significant_bits_impl<0,BITS>
            {
                enum { value = BITS };
            };
            template <size_t BITS>
            struct significant_bits_impl<1,BITS>
            {
                enum { value = BITS };
            };
            
            template <signed_int_max P>
            constexpr int significant_bits = significant_bits_impl<P,0>::value;
            
            template <signed_int_max P>
            using min_type = tmp::types::get_signed_fundamental_of_bit_size<min_type_size<significant_bits<absolute_value<P>>>>;
            
            /*
            template <int A, int B>
            constexpr bool is_less_than = A < B;

            template <signed_int_max P1, signed_int_max P2>
            struct overflow_impl
            {
                enum { N = signed_int_max_size };
                enum { Nxy = significant_bits<P1> + significant_bits<P2> };
                enum { C1 = is_less_than<Nxy, N> };
                enum { C2 = (Nxy == N-1) };
                enum { X1 = (static_cast<signed_int_max>(1) << C2*significant_bits<P1>) };
                enum { Y1 = (static_cast<signed_int_max>(1) << C2*significant_bits<P2>) };
                enum { Y2 = P2-(static_cast<signed_int_max>(1) << C2*significant_bits<P2>) };
                enum { R1 = (C2*P1)*Y1 };
                enum { R1_ = C2*(R1-(static_cast<signed_int_max>(1) << C2*static_cast<int>(Nxy))) };
                enum { R2 = P1*(C2*Y2) };
                enum { Nxy_ = significant_bits<R1_+R2> };
                enum { C3 = (Nxy_ == N-1) };
                
                enum { value = !C1 + (C2 && C3) };
            };
            
            template <signed_int_max P1, signed_int_max P2>
            constexpr bool overflow = overflow_impl<absolute_value<P1>,absolute_value<P2>>::value;*/
            
            template <signed_int_max N, signed_int_max D>
            struct rational_t;
            
            template <typename T>
            struct rational_nominator_impl;
            template <signed_int_max N, signed_int_max D>
            struct rational_nominator_impl<rational_t<N,D>> { enum { value = N }; };
            template <typename T>
            struct rational_denominator_impl;
            template <signed_int_max N, signed_int_max D>
            struct rational_denominator_impl<rational_t<N,D>> { enum { value = D }; };
            
            template <typename T>
            constexpr auto nominator = rational_nominator_impl<T>::value;
            template <typename T>
            constexpr auto denominator = rational_denominator_impl<T>::value;
            
            template <typename E1, typename E2>
            struct rational_addition_expr;
            template <typename E1, typename E2>
            struct rational_subtraction_expr;
            template <typename E1, typename E2>
            struct rational_multiplication_expr;
            template <typename E1, typename E2>
            struct rational_division_expr;
            
            template <typename T>
            struct rational_expr_base
            {
            };
            
            template <typename E1, typename E2>
            struct rational_addition_expr_impl
            {
                typedef typename E1::evaluate L;
                typedef typename E2::evaluate R;
                typedef rational_t<   nominator<L>*(denominator<R>/gcd<denominator<L>,denominator<R>>) + 
                                    nominator<R>*(denominator<L>/gcd<denominator<L>,denominator<R>>)
                                    ,
                                    (denominator<L>/gcd<denominator<L>,denominator<R>>) * denominator<R>> evaluate;
            };
            
            template <typename E1, typename E2>
            struct rational_addition_expr : public rational_expr_base<rational_addition_expr<E1,E2>>
            {
                template <typename T>
                auto operator+(const T &&) const { return rational_addition_expr<rational_addition_expr<E1,E2>,T>{}; }
                template <typename T>
                auto operator-(const T &&) const { return rational_subtraction_expr<rational_addition_expr<E1,E2>,T>{}; }
                template <typename T>
                auto operator*(const T &&) const { return rational_multiplication_expr<rational_addition_expr<E1,E2>,T>{}; }
                template <typename T>
                auto operator/(const T &&) const { return rational_division_expr<rational_addition_expr<E1,E2>,T>{}; }
                
             private:
                typedef typename rational_addition_expr_impl<E1,E2>::evaluate res;
             public:
                typedef rational_t<   nominator<res>/gcd<nominator<res>,denominator<res>>,
                                    denominator<res>/gcd<nominator<res>,denominator<res>> > evaluate;
            };
            
            template <typename E1, typename E2>
            struct rational_subtraction_expr_impl
            {
                typedef typename E1::evaluate L;
                typedef typename E2::evaluate R;
                typedef rational_t<   nominator<L>*(denominator<R>/gcd<denominator<L>,denominator<R>>) - 
                                    nominator<R>*(denominator<L>/gcd<denominator<L>,denominator<R>>)
                                    ,
                                    (denominator<L>/gcd<denominator<L>,denominator<R>>) * denominator<R>> evaluate;
            };
            
            template <typename E1, typename E2>
            struct rational_subtraction_expr : public rational_expr_base<rational_subtraction_expr<E1,E2>>
            {
                template <typename T>
                auto operator+(const T &&) const { return rational_addition_expr<rational_subtraction_expr<E1,E2>,T>{}; }
                template <typename T>
                auto operator-(const T &&) const { return rational_subtraction_expr<rational_subtraction_expr<E1,E2>,T>{}; }
                template <typename T>
                auto operator*(const T &&) const { return rational_multiplication_expr<rational_subtraction_expr<E1,E2>,T>{}; }
                template <typename T>
                auto operator/(const T &&) const { return rational_division_expr<rational_subtraction_expr<E1,E2>,T>{}; }
                
             private:
                typedef typename rational_subtraction_expr_impl<E1,E2>::evaluate res;
             public:
                typedef rational_t<   nominator<res>/gcd<nominator<res>,denominator<res>>,
                                    denominator<res>/gcd<nominator<res>,denominator<res>> > evaluate;
            };
            
            template <typename E1, typename E2>
            struct rational_multiplication_expr_impl
            {
                typedef typename E1::evaluate L;
                typedef typename E2::evaluate R;
                typedef rational_t<   (nominator<L>/gcd<nominator<L>,denominator<R>>) * (nominator<R>/gcd<nominator<R>,denominator<L>>)
                                    ,
                                    (denominator<L>/gcd<nominator<R>,denominator<L>>) * (denominator<R>/gcd<nominator<L>,denominator<R>>)> evaluate;
            };
            template <typename E1, typename E2>
            struct rational_multiplication_expr : public rational_expr_base<rational_multiplication_expr<E1,E2>>
            {
                template <typename T>
                auto operator+(const T &&) const { return rational_addition_expr<rational_multiplication_expr<E1,E2>,T>{}; }
                template <typename T>
                auto operator-(const T &&) const { return rational_subtraction_expr<rational_multiplication_expr<E1,E2>,T>{}; }
                template <typename T>
                auto operator*(const T &&) const { return rational_multiplication_expr<rational_multiplication_expr<E1,E2>,T>{}; }
                template <typename T>
                auto operator/(const T &&) const { return rational_division_expr<rational_multiplication_expr<E1,E2>,T>{}; }
                
             private:
                typedef typename rational_multiplication_expr_impl<E1,E2>::evaluate res;
             public:
                typedef rational_t<   nominator<res>/gcd<nominator<res>,denominator<res>>,
                                    denominator<res>/gcd<nominator<res>,denominator<res>> > evaluate;
            };
            
            template <typename E1, typename E2>
            struct rational_division_expr_impl
            {
                typedef typename E1::evaluate L;
                typedef typename E2::evaluate R;
                typedef rational_t<   (nominator<L>/gcd<nominator<L>,nominator<R>>) * (denominator<R>/gcd<denominator<L>,denominator<R>>)
                                    ,
                                    (denominator<L>/gcd<denominator<L>,denominator<R>>) * (nominator<R>/gcd<nominator<L>,nominator<R>>)> evaluate;
            };
            template <typename E1, typename E2>
            struct rational_division_expr : public rational_expr_base<rational_division_expr<E1,E2>>
            {
                template <typename T>
                auto operator+(const T &&) const { return rational_addition_expr<rational_division_expr<E1,E2>,T>{}; }
                template <typename T>
                auto operator-(const T &&) const { return rational_subtraction_expr<rational_division_expr<E1,E2>,T>{}; }
                template <typename T>
                auto operator*(const T &&) const { return rational_multiplication_expr<rational_division_expr<E1,E2>,T>{}; }
                template <typename T>
                auto operator/(const T &&) const { return rational_division_expr<rational_division_expr<E1,E2>,T>{}; }
                
             private:
                typedef typename rational_division_expr_impl<E1,E2>::evaluate res;
             public:
                typedef rational_t< nominator<res>/gcd<nominator<res>,denominator<res>>,
                                    denominator<res>/gcd<nominator<res>,denominator<res>> > evaluate;
            };
            
            /* (SN*DN) / (SD*DD) :
             *
             * (1) SN/SD            (rational)                     
             *          (Z*rational -> rational + DN)
             *          (Z/rational -> rational + DD)
             *          (Z+rational -> rational + 
             * (2) SN*DN/SD         (rational + DN)
             * (3) SN/(SD*DD)       (rational + DD)
             * (4) (SN*DN)/(SD*DD)  (rational + DN + DD)
             */
             
            template <typename T>
            struct rational_base
            {
            };
            
            template <typename T>
            struct rational_float
            {
            };
            
            template <typename T, typename R>
            struct rational_nominator : public rational_base<rational_nominator<T,R>>, rational_float<rational_nominator<T,R>>
            {
                T n;
                rational_nominator() = delete;
                explicit rational_nominator(T n) : n(n) {}
            };
            template <typename T, typename R>
            struct rational_denominator : public rational_base<rational_denominator<T,R>>, rational_float<rational_denominator<T,R>>
            {
                T d;
                rational_denominator() = delete;
                explicit rational_denominator(T d) : d(d) {}
            };
            template <typename S,typename T, typename R>
            struct rational : public rational_base<rational<S,T,R>>, rational_float<rational<S,T,R>>
            {
                S n;
                T d;
                rational() = delete;
                explicit rational(S n, T d) : n(n),d(d) {}
            };
            
            template <signed_int_max N, signed_int_max D>
            struct rational_t : public rational_expr_base<rational_t<N,D>>, rational_float<rational_t<N,D>>
            {
                template <typename T>
                auto operator+(const T &&) const { return rational_addition_expr<rational_t<N,D>,T>{}; }
                template <typename T>
                auto operator-(const T &&) const { return rational_subtraction_expr<rational_t<N,D>,T>{}; }
                template <typename T>
                auto operator*(const T &&) const { return rational_multiplication_expr<rational_t<N,D>,T>{}; }
                template <typename T>
                auto operator/(const T &&) const { return rational_division_expr<rational_t<N,D>,T>{}; }
                
                typedef rational_t<N,D> evaluate;
            };
            
            template <typename R>
            struct rational_t_flip
            {
                typedef rational_t<denominator<R>, nominator<R>> type;
            };
            
            template <typename R>
            using flip = typename rational_t_flip<R>::type;
            
            template <typename U, typename V>
            using biggest = get_element_at<type_to_size<U> < type_to_size<V>, U, V>;
            template <typename U, typename V>
            using best_impl = get_element_at<((tmp::types::is_signed<U> || tmp::types::is_signed<V>) && tmp::types::is_unsigned<biggest<U,V>>),
                            biggest<U,V>,
                            tmp::types::to_unsigned<biggest<U,V>> >;
                            
            template <typename ... TS>
            struct best_var_impl;
            template <typename T, typename V, typename ... TS>
            struct best_var_impl<T,V,TS...>
            {
                typedef typename best_var_impl<best_impl<T,V>,TS...>::type type;
            };
            template <typename T>
            struct best_var_impl<T>
            {
                typedef T type;
            };
            template <typename ... TS>
            using best = typename best_var_impl<TS...>::type;
             
            namespace operations
            {
                template <typename A>
                struct operand;
                template <typename T>
                struct operand<rational_expr_base<T>>
                {
                    typedef typename T::evaluate     r;
                    typedef min_type<nominator<r>>   up_type;
                    typedef min_type<denominator<r>> down_type;
                    static constexpr up_type   n(const rational_expr_base<T> &) { return 1; }
                    static constexpr down_type d(const rational_expr_base<T> &) { return 1; }
                };
                template <typename U, typename R>
                struct operand<rational_nominator<U,R>>
                {
                    typedef R                  r;
                    typedef U                  up_type;
                    typedef min_type<1>        down_type;
                    static constexpr up_type   n(const rational_nominator<U,R> & t) { return t.n; }
                    static constexpr down_type d(const rational_nominator<U,R>&)    { return 1; }
                };
                template <typename U, typename R>
                struct operand<rational_denominator<U,R>>
                {
                    typedef R                  r;
                    typedef min_type<1>        up_type;
                    typedef U                  down_type;
                    static constexpr up_type   n(const rational_denominator<U,R>&)    { return 1; }
                    static constexpr down_type d(const rational_denominator<U,R> & t) { return t.d; }
                };
                template <typename U, typename V, typename R>
                struct operand<rational<U,V,R>>
                {
                    typedef R                  r;
                    typedef U                  up_type;
                    typedef V                  down_type;
                    static constexpr up_type   n(const rational<U,V,R> & t) { return t.n; }
                    static constexpr down_type d(const rational<U,V,R> & t) { return t.d; }
                };

                template <typename R1, typename R2>
                constexpr signed_int_max _13gcd = gcd<nominator<R1>,nominator<R2>>; 

                template <typename R1, typename R2>
                constexpr signed_int_max _24 = (denominator<R1> == denominator<R2> ? 1 : denominator<R1>)*denominator<R2>;

                template <typename R1, typename R2>
                constexpr signed_int_max _14 = nominator<R1>*(denominator<R1> == denominator<R2> ? 1 : denominator<R2>)/_13gcd<R1,R2>;

                template <typename R1, typename R2>
                constexpr signed_int_max _23 = nominator<R2>*(denominator<R1> == denominator<R2> ? 1 : denominator<R1>)/_13gcd<R1,R2>;

                /* 
                    Branching will be performed at compile time. It is done this way to guarantee that rounding errors are minimized by circumventing
                    as many sources for errors as possible. It is possible that the compiler would return the exact same result with the
                    "naive" implementation.
                */
                template <typename R>
                constexpr float multiply_float_impl(float f) { return   ((nominator<R> == 1 && denominator<R> == 1) ? f :
                            ((nominator<R> == 1)                        ? f/float(denominator<R>) :
                            ((denominator<R> == 1) ? f*float(nominator<R>) : 
                              f*float(nominator<R>)/float(denominator<R>)))); }
                template <typename R>
                constexpr double multiply_double_impl(double d){ return ((nominator<R> == 1 && denominator<R> == 1) ? d :
                            ((nominator<R> == 1)                        ? d/double(denominator<R>) :
                            ((denominator<R> == 1) ? d*double(nominator<R>) : 
                              d*double(nominator<R>)/double(denominator<R>)))); }
                template <typename R>
                constexpr float multiply_float_impl_d(float f) { return ((nominator<R> == 1 && denominator<R> == 1) ? 1.0f/f :
                            ((nominator<R> == 1)                        ? 1.0f/(float(denominator<R>)*f) :
                            ((denominator<R> == 1) ? float(nominator<R>)/f : 
                              float(nominator<R>)/(float(denominator<R>)*f)))); }
                template <typename R>
                constexpr double multiply_double_impl_d(double d){ return   ((nominator<R> == 1 && denominator<R> == 1) ? 1.0/d :
                                ((nominator<R> == 1)                        ? 1.0/(double(denominator<R>)*d) :
                                ((denominator<R> == 1) ? double(nominator<R>)/d : 
                                  double(nominator<R>)/(double(denominator<R>)*d)))); }
                template <typename R>
                constexpr float multiply_float_impl_r(float n, float d) { return    ((nominator<R> == 1 && denominator<R> == 1) ? n/d :
                                        ((nominator<R> == 1)                        ? n/(float(denominator<R>)*d) :
                                        ((denominator<R> == 1) ? n*float(nominator<R>)/d : 
                                          (n*float(nominator<R>))/(float(denominator<R>)*d)))); }
                template <typename R>
                constexpr float multiply_double_impl_r(double n, double d) { return ((nominator<R> == 1 && denominator<R> == 1) ? n/d :
                                        ((nominator<R> == 1)                        ? n/(double(denominator<R>)*d) :
                                        ((denominator<R> == 1) ? n*double(nominator<R>)/d : 
                                          (n*double(nominator<R>))/(double(denominator<R>)*d)))); }

                template <typename A, typename B>
                struct result_type_impl;
                template <typename U, typename R1, typename T>
                struct result_type_impl<rational_nominator<U,R1>,rational_expr_base<T>>
                {
                    typedef typename T::evaluate R2;
                    typedef rational_nominator< best<U, min_type<_14<R1,R2>>, min_type<_23<R1,R2>>>,
                                                rational_t<_13gcd<R1,R2>,_24<R1,R2>>> addition;
                    typedef addition subtraction;
                    typedef rational_nominator< U,
                                                rational_t<nominator<R1>*nominator<R2>,denominator<R1>*denominator<R2>>> multiplication;
                    typedef rational_nominator< U,
                                                rational_t<nominator<R1>*denominator<R2>,denominator<R1>*nominator<R2>>> division;
                };
                template <typename T, typename V, typename R2>
                struct result_type_impl<rational_expr_base<T>,rational_nominator<V,R2>>
                {
                    typedef typename T::evaluate R1;
                    typedef typename result_type_impl<rational_nominator<V,R2>,rational_expr_base<T>>::addition addition;
                    typedef addition subtraction;
                    typedef typename result_type_impl<rational_nominator<V,R2>,rational_expr_base<T>>::multiplication multiplication;
                    typedef rational_denominator<V,
                                     rational_t<nominator<R1>*denominator<R2>,denominator<R1>*nominator<R2>>> division;
                };
                template <typename U, typename R1, typename T>
                struct result_type_impl<rational_denominator<U,R1>,rational_expr_base<T>>
                {
                    typedef typename T::evaluate R2;
                    typedef rational<   best<U, min_type<_14<R1,R2>>, min_type<_23<R1,R2>>>,
                                        U,
                                        rational_t<_13gcd<R1,R2>,_24<R1,R2>>> addition;
                    typedef addition subtraction;
                    typedef rational_denominator<   U,
                                                    rational_t<nominator<R1>*nominator<R2>,denominator<R1>*denominator<R2>>> multiplication;
                    typedef rational_denominator<   U,
                                                    rational_t<nominator<R1>*denominator<R2>,denominator<R1>*nominator<R2>>> division;
                };
                template <typename T, typename V, typename R2>
                struct result_type_impl<rational_expr_base<T>,rational_denominator<V,R2>>
                {
                    typedef typename T::evaluate R1;
                    typedef typename result_type_impl<rational_denominator<V,R2>,rational_expr_base<T>>::addition addition;
                    typedef addition subtraction;
                    typedef typename result_type_impl<rational_denominator<V,R2>,rational_expr_base<T>>::multiplication multiplication;
                    typedef rational_nominator<V,
                                    rational_t<nominator<R1>*denominator<R2>,denominator<R1>*nominator<R2>>> division;
                };
                template <typename U, typename V, typename R1, typename T>
                struct result_type_impl<rational<U,V,R1>,rational_expr_base<T>>
                {
                    typedef typename T::evaluate R2;
                    typedef rational<   best<U,V, min_type<_14<R1,R2>>, min_type<_23<R1,R2>>>,
                                        V,
                                        rational_t<_13gcd<R1,R2>,_24<R1,R2>>> addition;
                    typedef addition subtraction;
                    typedef rational<   U,V,
                                        rational_t<nominator<R1>*nominator<R2>,denominator<R1>*denominator<R2>>> multiplication;
                    typedef rational<   U,V,
                                        rational_t<nominator<R1>*denominator<R2>,denominator<R1>*nominator<R2>>> division;
                };
                template <typename T, typename V, typename W, typename R2>
                struct result_type_impl<rational_expr_base<T>,rational<V,W,R2>>
                {
                    typedef typename T::evaluate R1;
                    typedef typename result_type_impl<rational<V,W,R2>,rational_expr_base<T>>::addition addition;
                    typedef addition subtraction;
                    typedef typename result_type_impl<rational<V,W,R2>,rational_expr_base<T>>::multiplication multiplication;
                    typedef rational<   W,V,
                                        rational_t<nominator<R1>*denominator<R2>,denominator<R1>*nominator<R2>>> division;
                };
                template <typename U, typename R1, typename V, typename R2>
                struct result_type_impl<rational_nominator<U,R1>,rational_nominator<V,R2>>
                {
                    typedef rational_nominator< best<U, V, min_type<_14<R1,R2>>, min_type<_23<R1,R2>>>,
                                                rational_t<_13gcd<R1,R2>,_24<R1,R2>>> addition;
                    typedef addition subtraction;
                    typedef rational_nominator< best<U,V>,
                                                rational_t<nominator<R1>*nominator<R2>,denominator<R1>*denominator<R2>>> multiplication;
                    typedef rational<U,V,
                                     rational_t<nominator<R1>*denominator<R2>,denominator<R1>*nominator<R2>>> division;
                };
                template <typename U, typename R1, typename V, typename R2>
                struct result_type_impl<rational_nominator<U,R1>,rational_denominator<V,R2>>
                {
                    typedef rational<   best<U, V, min_type<_14<R1,R2>>, min_type<_23<R1,R2>>>,
                                        V,
                                        rational_t<_13gcd<R1,R2>,_24<R1,R2>>> addition;
                    typedef addition subtraction;
                    typedef rational<   U,V,
                                        rational_t<nominator<R1>*nominator<R2>,denominator<R1>*denominator<R2>>> multiplication;
                    typedef rational_nominator< best<U,V>,
                                                rational_t<nominator<R1>*denominator<R2>,denominator<R1>*nominator<R2>>> division;
                };
                template <typename U, typename R1, typename V, typename R2>
                struct result_type_impl<rational_denominator<U,R1>,rational_nominator<V,R2>>
                {
                    typedef typename result_type_impl<rational_nominator<V,R2>, rational_denominator<U,R1>>::addition addition;
                    typedef addition subtraction;
                    typedef rational<   V,U,
                                        rational_t<nominator<R1>*nominator<R2>,denominator<R1>*denominator<R2>>> multiplication;
                    typedef rational_nominator< best<U,V>,
                                                rational_t<nominator<R1>*denominator<R2>,denominator<R1>*nominator<R2>>> division;
                };
                template <typename U, typename R1, typename V, typename W, typename R2>
                struct result_type_impl<rational_nominator<U,R1>,rational<V,W,R2>>
                {
                    typedef rational<   best<U, V, W, min_type<_14<R1,R2>>, min_type<_23<R1,R2>>>,
                                        W,
                                        rational_t<_13gcd<R1,R2>,_24<R1,R2>>> addition;
                    typedef addition subtraction;
                    typedef rational<   best<U,V>,W,
                                        rational_t<nominator<R1>*nominator<R2>,denominator<R1>*denominator<R2>>> multiplication;
                    typedef rational<   best<U,W>,V,
                                        rational_t<nominator<R1>*denominator<R2>,denominator<R1>*nominator<R2>>> division;
                };
                template <typename U, typename R1, typename V, typename W, typename R2>
                struct result_type_impl<rational<U,V,R1>,rational_nominator<W,R2>>
                {
                    typedef typename result_type_impl<rational_nominator<W,R2>, rational<U,V,R1>>::addition addition;
                    typedef addition subtraction;
                    typedef rational<   best<U,W>,V,
                                        rational_t<nominator<R1>*nominator<R2>,denominator<R1>*denominator<R2>>> multiplication;
                    typedef rational<   U,best<V,W>,
                                        rational_t<nominator<R1>*denominator<R2>,denominator<R1>*nominator<R2>>> division;
                };
                template <typename U, typename R1, typename V, typename R2>
                struct result_type_impl<rational_denominator<U,R1>,rational_denominator<V,R2>>
                {
                    typedef rational<   best<U, V, min_type<_14<R1,R2>>, min_type<_23<R1,R2>>>,
                                        best<U,V>,
                                        rational_t<_13gcd<R1,R2>,_24<R1,R2>>> addition;
                    typedef addition subtraction;
                    typedef rational_denominator<   best<U,V>,
                                                    rational_t<nominator<R1>*nominator<R2>,denominator<R1>*denominator<R2>>> multiplication;
                    typedef rational<   V,U,
                                        rational_t<nominator<R1>*denominator<R2>,denominator<R1>*nominator<R2>>> division;
                };
                template <typename U, typename R1, typename V, typename W, typename R2>
                struct result_type_impl<rational_denominator<U,R1>,rational<V,W,R2>>
                {
                    typedef rational<   best<U, V, W, min_type<_14<R1,R2>>, min_type<_23<R1,R2>>>,
                                        best<U,W>,
                                        rational_t<_13gcd<R1,R2>,_24<R1,R2>>> addition;
                    typedef addition subtraction;
                    typedef rational<   V, best<U,W>,
                                        rational_t<nominator<R1>*nominator<R2>,denominator<R1>*denominator<R2>>> multiplication;
                    typedef rational<   W,best<U,V>,
                                        rational_t<nominator<R1>*denominator<R2>,denominator<R1>*nominator<R2>>> division;
                };
                template <typename U, typename V, typename R1, typename W, typename R2>
                struct result_type_impl<rational<U,V,R1>,rational_denominator<W,R2>>
                {
                    typedef typename result_type_impl<rational_denominator<W,R2>, rational<U,V,R1>>::addition addition;
                    typedef addition subtraction;
                    typedef rational<   U, best<V,W>,
                                        rational_t<nominator<R1>*nominator<R2>,denominator<R1>*denominator<R2>>> multiplication;
                    typedef rational<   best<U,W>,V,
                                        rational_t<nominator<R1>*denominator<R2>,denominator<R1>*nominator<R2>>> division;
                };
                template <typename U, typename V, typename R1, typename W, typename X, typename R2>
                struct result_type_impl<rational<U,V,R1>,rational<W,X,R2>>
                {
                    typedef rational<   best<U, V, W, X, min_type<_14<R1,R2>>, min_type<_23<R1,R2>>>,
                                        best<V,W>,
                                        rational_t<_13gcd<R1,R2>,_24<R1,R2>>> addition;
                    typedef addition subtraction;
                    typedef rational<   best<U,W>, best<V,X>,
                                        rational_t<nominator<R1>*nominator<R2>,denominator<R1>*denominator<R2>>> multiplication;
                    typedef rational<   best<U,X>,best<V,W>,
                                        rational_t<nominator<R1>*denominator<R2>,denominator<R1>*nominator<R2>>> division;
                };

                template <typename T>
                struct produce_result_impl;
                template <signed_int_max N, signed_int_max D>
                struct produce_result_impl<rational_t<N,D>>
                {
                    typedef rational_t<N,D> X;
                    static constexpr float to_float(const X &&) { return multiply_float_impl<X>(float(1.0)); }
                    static constexpr double to_double(const X &&) { return multiply_double_impl<X>(double(1.0)); }
                    static constexpr float multiply_float(const X &&,float h) { return multiply_float_impl<X>(h); }
                    static constexpr double multiply_double(const X &&,double h) { return multiply_double_impl<X>(h); }
                    static constexpr float divide_float(const X &&,float h) { return multiply_float_impl_d<X>(h); }
                    static constexpr float divide_float(float h,const X &&) { return multiply_float_impl<flip<X>>(h); }
                    static constexpr double divide_double(const X &&,double h) { return multiply_double_impl_d<X>(h); }
                    static constexpr double divide_double(double h,const X &&) { return multiply_double_impl<flip<X>>(h); }
                };
                template <typename U, typename X>
                struct produce_result_impl<rational_nominator<U,X>>
                {
                    template <typename A, typename B>
                    static constexpr rational_nominator<U,X> add(const A&& a, const B&& b)
                    {
                        typedef operand<A> L;
                        typedef operand<B> R;
                        typedef typename L::r LR;
                        typedef typename R::r RR;

                        return rational_nominator<U,X>{ static_cast<min_type<_14<LR,RR>>>(_14<LR,RR>)
                                                        *L::n(a)*R::d(b)+
                                                        static_cast<min_type<_23<LR,RR>>>(_23<LR,RR>)
                                                        *L::d(a)*R::n(b)};
                    }
                    template <typename A, typename B>
                    static constexpr rational_nominator<U,X> subtract(const A&& a, const B&& b)
                    {
                        typedef operand<A> L;
                        typedef operand<B> R;
                        typedef typename L::r LR;
                        typedef typename R::r RR;

                        return rational_nominator<U,X>{ static_cast<min_type<_14<LR,RR>>>(_14<LR,RR>)
                                                        *L::n(a)*R::d(b)-
                                                        static_cast<min_type<_23<LR,RR>>>(_23<LR,RR>)
                                                        *L::d(a)*R::n(b)};
                    }
                    template <typename A, typename B>
                    static constexpr rational_nominator<U,X> multiply(const A&& a, const B&& b)
                    {
                        typedef operand<A> L;
                        typedef operand<B> R;

                        return rational_nominator<U,X>{L::n(a)*R::n(b)};
                    }
                    template <typename A, typename B>
                    static constexpr rational_nominator<U,X> divide(const A&& a, const B&& b)
                    {
                        typedef operand<A> L;
                        typedef operand<B> R;

                        return rational_nominator<U,X>{L::n(a)*R::d(b)};
                    }
                    static constexpr float to_float(const rational_nominator<U,X> && r) { return multiply_float_impl<X>(float(r.n)); }
                    static constexpr double to_double(const rational_nominator<U,X> && r) { return multiply_double_impl<X>(double(r.n)); }
                    static constexpr float multiply_float(const rational_nominator<U,X> && r,float h) { return multiply_float_impl<X>(h*float(r.n)); }
                    static constexpr double multiply_double(const rational_nominator<U,X> && r,double h) { return multiply_double_impl<X>(h*double(r.n)); }
                    static constexpr float divide_float(const rational_nominator<U,X> && r,float h) { return multiply_float_impl_r<X>(float(r.n),h); }
                    static constexpr float divide_float(float h,const rational_nominator<U,X> && r) { return multiply_float_impl_r<flip<X>>(h,float(r.n)); }
                    static constexpr double divide_double(const rational_nominator<U,X> && r,double h) { return multiply_double_impl_r<X>(double(r.n),h); }
                    static constexpr double divide_double(double h,const rational_nominator<U,X> && r) { return multiply_double_impl_r<flip<X>>(h,double(r.n)); }
                };


                template <typename U, typename X>
                struct produce_result_impl<rational_denominator<U,X>>
                {
                    template <typename A, typename B>
                    static constexpr rational_denominator<U,X> multiply(const A&& a, const B&& b)
                    {
                        typedef operand<A> L;
                        typedef operand<B> R;

                        return rational_denominator<U,X>{L::d(a)*R::d(b)};
                    }
                    template <typename A, typename B>
                    static constexpr rational_denominator<U,X> divide(const A&& a, const B&& b)
                    {
                        typedef operand<A> L;
                        typedef operand<B> R;

                        return rational_denominator<U,X>{L::d(a)*R::n(b)};
                    }
                    static constexpr float to_float(const rational_denominator<U,X> && r) { return multiply_float_impl_d<X>(float(r.d)); }
                    static constexpr double to_double(const rational_denominator<U,X> && r) { return multiply_double_impl_d<X>(double(r.d)); }
                    static constexpr float multiply_float(const rational_denominator<U,X> && r,float h) { return multiply_float_impl_r<X>(h,float(r.d)); }
                    static constexpr double multiply_double(const rational_denominator<U,X> && r,double h) { return multiply_double_impl_r<X>(h,double(r.d)); }
                    static constexpr float divide_float(const rational_denominator<U,X> && r,float h) { return multiply_float_impl_d<X>(float(r.d)*h); }
                    static constexpr float divide_float(float h,const rational_denominator<U,X> && r) { return multiply_float_impl<flip<X>>(h*float(r.d)); }
                    static constexpr double divide_double(const rational_denominator<U,X> && r,double h) { return multiply_double_impl_d<X>(double(r.d)*h); }
                    static constexpr double divide_double(double h,const rational_denominator<U,X> && r) { return multiply_double_impl<flip<X>>(h*double(r.d)); }
                };

                template <typename U, typename V, typename X>
                struct produce_result_impl<rational<U,V,X>>
                {
                    template <typename A, typename B>
                    static constexpr rational<U,V,X> add(const A&& a, const B&& b)
                    {
                        typedef operand<A> L;
                        typedef operand<B> R;
                        typedef typename L::r LR;
                        typedef typename R::r RR;
                        return rational<U,V,X>{ static_cast<min_type<_14<LR,RR>>>(_14<LR,RR>)
                                                *L::n(a)*R::d(b)+
                                                static_cast<min_type<_23<LR,RR>>>(_23<LR,RR>)
                                                *L::d(a)*R::n(b),L::d(a)*R::d(b)};
                    }
                    template <typename A, typename B>
                    static constexpr rational<U,V,X> subtract(const A&& a, const B&& b)
                    {
                        typedef operand<A> L;
                        typedef operand<B> R;
                        typedef typename L::r LR;
                        typedef typename R::r RR;
                        return rational<U,V,X>{ static_cast<min_type<_14<LR,RR>>>(_14<LR,RR>)
                                                *L::n(a)*R::d(b)-
                                                static_cast<min_type<_23<LR,RR>>>(_23<LR,RR>)
                                                *L::d(a)*R::n(b),L::d(a)*R::d(b)};
                    }
                    template <typename A, typename B>
                    static constexpr rational<U,V,X> multiply(const A&& a, const B&& b)
                    {
                        typedef operand<A> L;
                        typedef operand<B> R;

                        return rational<U,V,X>{L::n(a)*L::n(b),L::d(a)*R::d(b)};
                    }
                    template <typename A, typename B>
                    static constexpr rational<U,V,X> divide(const A&& a, const B&& b)
                    {
                        typedef operand<A> L;
                        typedef operand<B> R;

                        return rational<U,V,X>{L::n(a)*L::d(b),L::d(a)*R::n(b)};
                    }
                    static constexpr float to_float(const rational<U,V,X> && r) { return multiply_float_impl_r<X>(float(r.n),float(r.d)); }
                    static constexpr double to_double(const rational<U,V,X>  && r) { return multiply_double_impl_r<X>(double(r.n),double(r.d)); }
                    static constexpr float multiply_float(const rational<U,V,X>  && r,float h) { return multiply_float_impl_r<X>(h*float(r.n),float(r.d),h); }
                    static constexpr double multiply_double(const rational<U,V,X>  && r,double h) { return multiply_double_impl_r<X>(h*double(r.n),double(r.d)); }
                    static constexpr float divide_float(const rational<U,V,X>  && r,float h) { return multiply_float_impl_r<X>(float(r.n),float(r.d)*h); }
                    static constexpr float divide_float(float h,const rational<U,V,X>  && r) { return multiply_float_impl_r<flip<X>>(h*float(r.d),float(r.n)); }
                    static constexpr double divide_double(const rational<U,V,X>  && r,double h) { return multiply_double_impl_r<X>(double(r.n),double(r.d)*h); }
                    static constexpr double divide_double(double h,const rational<U,V,X> && r) { return multiply_double_impl_r<flip<X>>(h*double(r.d),double(r.n)); }
                };

            } // namespace operations
             
            
            
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////                                      ADDITION                                          /////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            template <typename L, typename R>
            inline auto operator+(const rational_base<L> && l, const rational_base<R> && r)
            {
                using namespace operations;
                return produce_result_impl<typename result_type_impl<L,R>::addition>::
                        add(std::forward<const L>(*static_cast<const L*>(&l)),std::forward<const R>(*static_cast<const R*>(&r)));
            }
            template <typename L, typename R>
            inline auto operator+(const rational_base<L> && l, const rational_expr_base<R> &&)
            {
                using namespace operations;
                return produce_result_impl<typename result_type_impl<L,rational_expr_base<R>>::addition>::
                        add(std::forward<const L>(*static_cast<const L*>(&l)),rational_expr_base<R>{});
            }
            template <typename L, typename R>
            inline auto operator+(const rational_expr_base<L> &&, const rational_base<R> && r)
            {
                using namespace operations;
                return produce_result_impl<typename result_type_impl<rational_expr_base<L>,R>::addition>::
                        add(rational_expr_base<L>{},std::forward<const R>(*static_cast<const R*>(&r)));
            }
            
            template <typename L>
            inline float operator+(const rational_float<L> && l, float r)
            { 
                return operations::produce_result_impl<L>::to_float(std::forward<const L>(*static_cast<const L*>(&l))) + r;
            }
            template <typename L>
            inline double operator+(const rational_float<L> && l, double r)
            { 
                return operations::produce_result_impl<L>::to_double(std::forward<const L>(*static_cast<const L*>(&l))) + r;
            }
            template <typename R>
            inline float operator+(float l, const rational_float<R> && r)
            { 
                return l + operations::produce_result_impl<R>::to_float(std::forward<const R>(*static_cast<const R*>(&r)));
            }
            template <typename R>
            inline double operator+(double l, const rational_float<R> && r)
            { 
                return l + operations::produce_result_impl<R>::to_double(std::forward<const R>(*static_cast<const R*>(&r)));
            }
            
            template <typename L>
            inline auto operator+(const rational_float<L> && l, int r)
            {
                typedef rational_nominator<int,rational_t<1,1>> R;
                return std::forward<const L>(*static_cast<const L*>(&l)) + static_cast<const R>(R{r});
            }
            template <typename R>
            inline auto operator+(int l, const rational_float<R> && r)
            {
                typedef rational_nominator<int,rational_t<1,1>> L;
                return static_cast<const L>(L{l}) + std::forward<const R>(*static_cast<const R*>(&r));
            }
            
            template <typename L>
            inline auto operator+(const rational_float<L> && l, long int r)
            {
                typedef rational_nominator<long int,rational_t<1,1>> R;
                return std::forward<const L>(*static_cast<const L*>(&l)) + static_cast<const R>(R{r});
            }
            template <typename R>
            inline auto operator+(long int l, const rational_float<R> && r)
            {
                typedef rational_nominator<long int,rational_t<1,1>> L;
                return static_cast<const L>(L{l}) + std::forward<const R>(*static_cast<const R*>(&r));
            }
            
            template <typename L>
            inline auto operator+(const rational_float<L> && l, long long int r)
            {
                typedef rational_nominator<long long int,rational_t<1,1>> R;
                return std::forward<const L>(*static_cast<const L*>(&l)) + static_cast<const R>(R{r});
            }
            template <typename R>
            inline auto operator+(long long int l, const rational_float<R> && r)
            {
                typedef rational_nominator<long long int,rational_t<1,1>> L;
                return static_cast<const L>(L{l}) + std::forward<const R>(*static_cast<const R*>(&r));
            }
            
            
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////                                     SUBTRACTION                                        /////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            template <typename L, typename R>
            inline auto operator-(const rational_base<L> && l, const rational_base<R> && r)
            { 
                using namespace operations;
                return produce_result_impl<typename result_type_impl<L,R>::subtraction>::
                        subtract(std::forward<const L>(*static_cast<const L*>(&l)),std::forward<const R>(*static_cast<const R*>(&r)));
            }
            template <typename L, typename R>
            inline auto operator-(const rational_base<L> && l, const rational_expr_base<R> &&)
            {
                using namespace operations;
                return produce_result_impl<typename result_type_impl<L,rational_expr_base<R>>::subtraction>::
                        subtract(std::forward<const L>(*static_cast<const L*>(&l)),rational_expr_base<R>{});
            }
            template <typename L, typename R>
            inline auto operator-(const rational_expr_base<L> &&, const rational_base<R> && r)
            {
                using namespace operations;
                return produce_result_impl<typename result_type_impl<rational_expr_base<L>,R>::subtraction>::
                        subtract(rational_expr_base<L>{},std::forward<const R>(*static_cast<const R*>(&r)));
            }
            
            template <typename L>
            inline float operator-(const rational_float<L> && l, float r)
            { 
                return operations::produce_result_impl<L>::to_float(std::forward<const L>(*static_cast<const L*>(&l))) - r;
            }
            template <typename L>
            inline double operator-(const rational_float<L> && l, double r)
            { 
                return operations::produce_result_impl<L>::to_double(std::forward<const L>(*static_cast<const L*>(&l))) - r;
            }
            template <typename R>
            inline float operator-(float l, const rational_float<R> && r)
            { 
                return l - operations::produce_result_impl<R>::to_float(std::forward<const R>(*static_cast<const R*>(&r)));
            }
            template <typename R>
            inline double operator-(double l, const rational_float<R> && r)
            { 
                return l - operations::produce_result_impl<R>::to_double(std::forward<const R>(*static_cast<const R*>(&r)));
            }
            
            template <typename L>
            inline auto operator-(const rational_float<L> && l, int r)
            {
                typedef rational_nominator<int,rational_t<1,1>> R;
                return std::forward<const L>(*static_cast<const L*>(&l)) - static_cast<const R>(R{r});
            }
            template <typename R>
            inline auto operator-(int l, const rational_float<R> && r)
            {
                typedef rational_nominator<int,rational_t<1,1>> L;
                return static_cast<const L>(L{l}) - std::forward<const R>(*static_cast<const R*>(&r));
            }
            
            template <typename L>
            inline auto operator-(const rational_float<L> && l, long int r)
            {
                typedef rational_nominator<long int,rational_t<1,1>> R;
                return std::forward<const L>(*static_cast<const L*>(&l)) - static_cast<const R>(R{r});
            }
            template <typename R>
            inline auto operator-(long int l, const rational_float<R> && r)
            {
                typedef rational_nominator<long int,rational_t<1,1>> L;
                return static_cast<const L>(L{l}) - std::forward<const R>(*static_cast<const R*>(&r));
            }
            
            template <typename L>
            inline auto operator-(const rational_float<L> && l, long long int r)
            {
                typedef rational_nominator<long long int,rational_t<1,1>> R;
                return std::forward<const L>(*static_cast<const L*>(&l)) - static_cast<const R>(R{r});
            }
            template <typename R>
            inline auto operator-(long long int l, const rational_float<R> && r)
            {
                typedef rational_nominator<long long int,rational_t<1,1>> L;
                return static_cast<const L>(L{l}) - std::forward<const R>(*static_cast<const R*>(&r));
            }
            
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////                                     MULTIPLICATION                                     /////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            template <typename L, typename R>
            inline auto operator*(const rational_base<L> && l, const rational_base<R> && r)
            { 
                using namespace operations;
                return produce_result_impl<typename result_type_impl<L,R>::multiplication>::
                        multiply(std::forward<const L>(*static_cast<const L*>(&l)),std::forward<const R>(*static_cast<const R*>(&r)));
            }
            template <typename L, typename R>
            inline auto operator*(const rational_base<L> && l, const rational_expr_base<R> &&)
            {
                using namespace operations;
                return produce_result_impl<typename result_type_impl<L,rational_expr_base<R>>::multiplication>::
                        multiply(std::forward<const L>(*static_cast<const L*>(&l)),rational_expr_base<R>{});
            }
            template <typename L, typename R>
            inline auto operator*(const rational_expr_base<L> &&, const rational_base<R> && r)
            {
                using namespace operations;
                return produce_result_impl<typename result_type_impl<rational_expr_base<L>,R>::multiplication>::
                        multiply(rational_expr_base<L>{},std::forward<const R>(*static_cast<const R*>(&r)));
            }
            
            template <typename L>
            inline float operator*(const rational_float<L> && l, float r)
            { 
                return operations::produce_result_impl<L>::multiply_float(std::forward<const L>(*static_cast<const L*>(&l)),r);
            }
            template <typename L>
            inline double operator*(const rational_float<L> && l, double r)
            { 
                return operations::produce_result_impl<L>::multiply_double(std::forward<const L>(*static_cast<const L*>(&l)),r);
            }
            template <typename R>
            inline float operator*(float l, const rational_float<R> && r)
            { 
                return operations::produce_result_impl<R>::multiply_float(std::forward<const R>(*static_cast<const R*>(&r)),l);
            }
            template <typename R>
            inline double operator*(double l, const rational_float<R> && r)
            { 
                return operations::produce_result_impl<R>::multiply_double(std::forward<const R>(*static_cast<const R*>(&r)),l);
            }
            
            template <typename L>
            inline auto operator*(const rational_float<L> && l, int r)
            {
                typedef rational_nominator<int,rational_t<1,1>> R;
                return std::forward<const L>(*static_cast<const L*>(&l)) * static_cast<const R>(R{r});
            }
            template <typename R>
            inline auto operator*(int l, const rational_float<R> && r)
            {
                typedef rational_nominator<int,rational_t<1,1>> L;
                return static_cast<const L>(L{l}) * std::forward<const R>(*static_cast<const R*>(&r));
            }
            
            template <typename L>
            inline auto operator*(const rational_float<L> && l, long int r)
            {
                typedef rational_nominator<long int,rational_t<1,1>> R;
                return std::forward<const L>(*static_cast<const L*>(&l)) * static_cast<const R>(R{r});
            }
            template <typename R>
            inline auto operator*(long int l, const rational_float<R> && r)
            {
                typedef rational_nominator<long int,rational_t<1,1>> L;
                return static_cast<const L>(L{l}) * std::forward<const R>(*static_cast<const R*>(&r));
            }
            
            template <typename L>
            inline auto operator*(const rational_float<L> && l, long long int r)
            {
                typedef rational_nominator<long long int,rational_t<1,1>> R;
                return std::forward<const L>(*static_cast<const L*>(&l)) * static_cast<const R>(R{r});
            }
            template <typename R>
            inline auto operator*(long long int l, const rational_float<R> && r)
            {
                typedef rational_nominator<long long int,rational_t<1,1>> L;
                return static_cast<const L>(L{l}) * std::forward<const R>(*static_cast<const R*>(&r));
            }
            
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////                                        DIVISION                                        /////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            template <typename L, typename R>
            inline auto operator/(const rational_base<L> && l, const rational_base<R> && r)
            { 
                using namespace operations;
                return produce_result_impl<typename result_type_impl<L,R>::division>::
                        divide(std::forward<const L>(*static_cast<const L*>(&l)),std::forward<const R>(*static_cast<const R*>(&r)));
            }
            template <typename L, typename R>
            inline auto operator/(const rational_base<L> && l, const rational_expr_base<R> &&)
            {
                using namespace operations;
                return produce_result_impl<typename result_type_impl<L,rational_expr_base<R>>::division>::
                        divide(std::forward<const L>(*static_cast<const L*>(&l)),rational_expr_base<R>{});
            }
            template <typename L, typename R>
            inline auto operator/(const rational_expr_base<L> &&, const rational_base<R> && r)
            {
                using namespace operations;
                return produce_result_impl<typename result_type_impl<rational_expr_base<L>,R>::division>::
                        divide(rational_expr_base<L>{},std::forward<const R>(*static_cast<const R*>(&r)));
            }
            
            template <typename L>
            inline float operator/(const rational_float<L> && l, float r)
            { 
                return operations::produce_result_impl<L>::divide_float(std::forward<const L>(*static_cast<const L*>(&l)),r);
            }
            template <typename L>
            inline double operator/(const rational_float<L> && l, double r)
            { 
                return operations::produce_result_impl<L>::divide_double(std::forward<const L>(*static_cast<const L*>(&l)),r);
            }
            template <typename R>
            inline float operator/(float l, const rational_float<R> && r)
            { 
                return operations::produce_result_impl<R>::divide_float(l,std::forward<const R>(*static_cast<const R*>(&r)));
            }
            template <typename R>
            inline double operator/(double l, const rational_float<R> && r)
            { 
                return operations::produce_result_impl<R>::divide_double(l,std::forward<const R>(*static_cast<const R*>(&r)));
            }
            
            template <typename L>
            inline auto operator/(const rational_float<L> && l, int r)
            {
                typedef rational_nominator<int,rational_t<1,1>> R;
                return std::forward<const L>(*static_cast<const L*>(&l)) / static_cast<const R>(R{r});
            }
            template <typename R>
            inline auto operator/(int l, const rational_float<R> && r)
            {
                typedef rational_nominator<int,rational_t<1,1>> L;
                return static_cast<const L>(L{l}) / std::forward<const R>(*static_cast<const R*>(&r));
            }
            
            template <typename L>
            inline auto operator/(const rational_float<L> && l, long int r)
            {
                typedef rational_nominator<long int,rational_t<1,1>> R;
                return std::forward<const L>(*static_cast<const L*>(&l)) / static_cast<const R>(R{r});
            }
            template <typename R>
            inline auto operator/(long int l, const rational_float<R> && r)
            {
                typedef rational_nominator<long int,rational_t<1,1>> L;
                return static_cast<const L>(L{l}) / std::forward<const R>(*static_cast<const R*>(&r));
            }
            
            template <typename L>
            inline auto operator/(const rational_float<L> && l, long long int r)
            {
                typedef rational_nominator<long long int,rational_t<1,1>> R;
                return std::forward<const L>(*static_cast<const L*>(&l)) / static_cast<const R>(R{r});
            }
            template <typename R>
            inline auto operator/(long long int l, const rational_float<R> && r)
            {
                typedef rational_nominator<long long int,rational_t<1,1>> L;
                return static_cast<const L>(L{l}) / std::forward<const R>(*static_cast<const R*>(&r));
            }
            
        } // namespace detail
        
        template <detail::signed_int_max N, detail::signed_int_max D>
        using rational = detail::rational_t<N/detail::gcd<N,D>,D/detail::gcd<N,D>>;
        template <typename T>
        constexpr auto nominator = detail::nominator<T>;
        template <typename T>
        constexpr auto denominator = detail::denominator<T>;
        
    } // namespace rational
} // namespace tmp



#endif // TMP_RATIONAL_