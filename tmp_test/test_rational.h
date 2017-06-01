#ifndef TEST_RATIONAL_H
#define TEST_RATIONAL_H

#include <cstdio>
#include <cmath>
#include "tmp_rational.h"

void test_gcd();
void test_rational_arithmetic();
void test_rationald_arithmetic();

template <typename T>
struct print_impl
{
    static void a(T i)
    {
        printf("%d", i);
    }
};
template <>
struct print_impl<int>
{
    static void a(int i)
    {
        printf("%d", i);
    }
};
template <>
struct print_impl<long int>
{
    static void a(long int i)
    {
        printf("%ld", i);
    }
};
template <>
struct print_impl<long long int>
{
    static void a(long long int i)
    {
        printf("%lld", i);
    }
};

template <typename T>
void print_rational_num(T i) { print_impl<T>::a(i); }

template <typename T>
void print_rational(T r)
{
    using namespace tmp::rational::detail;
    typedef operations::operand<T> type;
    typedef typename type::r R;
    constexpr auto nom = nominator<R>;
    constexpr auto denom = denominator<R>;
    int a = 0;
    bool m = !((nom == 1 && denom == 1) || (type::n(r) == 1 && type::d(r) == 1));
    if(nom == 1 && denom == 1){printf("         ");a=9;}
    else if(nom != 1 && denom == 1) { printf("%d",nom);a=a+1+int(log(nom)); for(int i = 0; i < 9-a; i++){printf(" ");} a=9;}
    else { printf("(%d/%d)",nom,denom);a=a+4+log(nom)+log(denom);if(nom==1){a+=1;} for(int i = 0; i < 9-a; i++){printf(" ");}a=9; }
    if(m) printf("*"); else{printf(" ");}
    printf(" ");
    if(type::n(r) == 1 && type::d(r) == 1){printf("        ");a=18;}
    else if(type::n(r) != 1 && type::d(r) == 1) { print_rational_num(type::n(r));a=a+1+int(log(type::n(r))); for(int i = 0; i < 18-a; i++){printf(" ");}a=18; }
    else { printf("("); print_rational_num(type::n(r)); printf("/"); print_rational_num(type::d(r));printf(")");
    a=a+4+int(log(type::n(r)))+int(log(type::d(r)));if(type::n(r)==1){a+=1;}for(int i = 0; i < 18-a; i++){printf(" ");}a=18;}
}

template <typename T, typename L, typename R>
void test_and_print_addition(T t, L l, R r)
{
    using namespace tmp::rational::detail;
    typedef operations::operand<T> type;
    typedef typename type::r TR;
    constexpr int nom = nominator<TR>;
    constexpr int denom = denominator<TR>;
    
    typedef operations::operand<L> ltype;
    typedef typename ltype::r LR;
    constexpr int sln = nominator<LR>;
    constexpr int sld = denominator<LR>;
    int ln = ltype::n(l);
    int ld = ltype::d(l);
    ln = sln*ln;
    ld = sld*ld;
    
    typedef operations::operand<R> rtype;
    typedef typename rtype::r RR;
    constexpr int srn = nominator<RR>;
    constexpr int srd = denominator<RR>;
    int rn = rtype::n(r);
    int rd = rtype::d(r);
    rn = srn*rn;
    rd = srd*rd;
    
    int resn = ln*rd+rn*ld;
    int resd = ld*rd;
    if(resn == nom*type::n(t) && resd == denom*type::d(t))  { printf("PASSED  "); }
    else                                                    { printf("FAILED  "); }
      
    print_rational(l); printf("+ "); print_rational(r);
    printf(" = ");
    print_rational(t);
    printf("\n");
}

template <typename R1, typename R2>
void test_all_additions()
{
    using namespace tmp::rational::detail;
    typedef rational_nominator<int,rational_t<1,1>> n;
    typedef rational_nominator<int,R1> nom;
    typedef rational_denominator<int,R1> denom;
    typedef rational<int,int,R1> r;
    typedef rational_nominator<int,R2> nom1;
    typedef rational_denominator<int,R2> denom1;
    typedef rational<int,int,R2> r1;
    
    test_and_print_addition(nom{5} + nom1{6}, nom{5}, nom1{6});
    test_and_print_addition(nom{5} + denom1{6}, nom{5}, denom1{6});
    test_and_print_addition(nom{5} + r1{6,5}, nom{5}, r1{6,5});
    
    test_and_print_addition(denom{5} + nom1{6}, denom{5}, nom1{6});
    test_and_print_addition(denom{5} + denom1{6}, denom{5}, denom1{6});
    test_and_print_addition(denom{5} + r1{6,5}, denom{5}, r1{6,5});
    
    test_and_print_addition(r{6,5} + nom1{6}, r{6,5}, nom1{6});
    test_and_print_addition(r{6,5} + denom1{6}, r{6,5}, denom1{6});
    test_and_print_addition(r{6,5} + r1{6,5}, r{6,5}, r1{6,5});
    
    test_and_print_addition(5LL + nom1{6}, n{5},nom1{6});
    test_and_print_addition(5LL + denom1{6}, n{5},denom1{6});
    test_and_print_addition(5LL + r1{6,5}, n{5},r1{6,5});
    
    test_and_print_addition(nom{5} + 5LL, nom{5}, n{5});
    test_and_print_addition(denom{5} + 5LL, denom{5}, n{5});
    test_and_print_addition(r{6,5} + 5LL, r{6,5}, n{5});
    
    test_and_print_addition(5L + nom1{6}, n{5},nom1{6});
    test_and_print_addition(5L + denom1{6}, n{5},denom1{6});
    test_and_print_addition(5L + r1{6,5}, n{5},r1{6,5});
    
    test_and_print_addition(nom{5} + 5L, nom{5}, n{5});
    test_and_print_addition(denom{5} + 5L, denom{5}, n{5});
    test_and_print_addition(r{6,5} + 5L, r{6,5}, n{5});
    
    test_and_print_addition(5 + nom1{6}, n{5},nom1{6});
    test_and_print_addition(5 + denom1{6}, n{5},denom1{6});
    test_and_print_addition(5 + r1{6,5}, n{5},r1{6,5});
    
    test_and_print_addition(nom{5} + 5, nom{5}, n{5});
    test_and_print_addition(denom{5} + 5, denom{5}, n{5});
    test_and_print_addition(r{6,5} + 5, r{6,5}, n{5});
}

#endif // TEST_RATIONAL_H
