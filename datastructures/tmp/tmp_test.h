#ifndef TMP_TEST_H
#define TMP_TEST_H

#include "tmp.h"
#include "tmp_math.h"
#include "tmp_types.h"
#include "tmp_function.h"

template <size_t I>
struct enumerated_type
{
    static void printme()
    {
        printf("%d",I);
    }
};

/* Expects the tuple to be populated by either enumerated_types or other tuples */
template <typename>
struct print_tuple;

template <typename ... ES>
struct print_tuple<tmp::tuple_t<tmp::tuple_t<ES...>>>
{
    static void printme()
    {
        printf("{");
        print_tuple<tmp::tuple_t<ES...>>::printme();
        printf("}");
    }
};

template <typename ... ES, typename T, typename ... TS>
struct print_tuple<tmp::tuple_t<tmp::tuple_t<ES...>,T,TS...>>
{
    static void printme()
    {
        printf("{");
        print_tuple<tmp::tuple_t<ES...>>::printme();
        printf("},");
        print_tuple<tmp::tuple_t<T,TS...>>::printme();
    }
};

template <typename T1, typename T2, typename ... TS>
struct print_tuple<tmp::tuple_t<T1,T2,TS...>>
{
    static void printme()
    {
        T1::printme();
        printf(",");
        print_tuple<tmp::tuple_t<T2,TS...>>::printme();
    }
};

template <typename T>
struct print_tuple<tmp::tuple_t<T>>
{
    static void printme()
    {
        T::printme();
    }
};

template <>
struct print_tuple<tmp::tuple_t<>>
{
    static void printme(){}
};

template <typename T>
struct to_print_tuple;
template <size_t ... SEQ>
struct to_print_tuple<tmp::tuple_t<tmp::int_type<SEQ>...>>
{
    typedef print_tuple<tmp::tuple_t<enumerated_type<SEQ>...>> type;
};

void test_type_comparation()
{
    printf("Test of \"tmp::types_equal<T1,T2>\":\n");
    bool types_should_equal = tmp::types_equal<int,int>::value;
    bool types_shouldnt_equal = tmp::types_equal<bool,int>::value;
    printf("int = int: %s\n",types_should_equal ? "true" : "false");
    printf("bool = int: %s\n", types_shouldnt_equal ? "true" : "false");
    printf("\n");
}

void test_summation_with_compare()
{
    printf("Test of \"tmp::sum<PREDICATE>::of<TUPLE>\":\n");
    typedef tmp::tuple_t<int,int,bool,char,int,long,bool,float> tup1;
    int num1 = tmp::sum<tmp::compare<int>::with>::of<tup1>::value;
    int num2 = tmp::sum<tmp::compare<bool>::with>::of<tup1>::value;
    printf("Number of ints in tuple_t<int,int,bool,char,int,long,bool,float>: %d\n", num1);
    printf("Number of bools in tuple_t<int,int,bool,char,int,long,bool,float>: %d\n", num2);
    printf("\n");
}

void test_prepending_tuple_of_tuples()
{
    typedef enumerated_type<0> t0;typedef enumerated_type<1> t1;typedef enumerated_type<2> t2;typedef enumerated_type<3> t3;
    typedef enumerated_type<4> t4;typedef enumerated_type<5> t5;typedef enumerated_type<6> t6;typedef enumerated_type<7> t7;
    typedef tmp::tuple_t<tmp::tuple_t<t0,t1,t2,t3,t4>,tmp::tuple_t<t5>,tmp::tuple_t<t6,t7,t3>,tmp::tuple_t<t6,t2,t3>> tuple_of_tuples;

    printf("Test of \"tmp::detail::prepend_all_of<C,TUPLEOFTUPLE>\":\n");
    printf("Tuples to prepend:\n");
    print_tuple<tuple_of_tuples>::printme();
    printf("\nTuples prepended with ");
    t2::printme();
    printf(":\n");
    print_tuple<tmp::detail::prepend_all_of<t2,tuple_of_tuples>::type>::printme();
    printf("\n\n");
}

/*
 *  Set up all metas for the tree
 */

typedef enumerated_type<0> m0;
typedef enumerated_type<1> m1;
typedef enumerated_type<2> m2;
typedef enumerated_type<3> m3;
typedef enumerated_type<4> m4;
typedef enumerated_type<5> m5;
typedef enumerated_type<6> m6;
typedef enumerated_type<7> m7;
typedef enumerated_type<8> m8;
typedef enumerated_type<9> m9;
typedef enumerated_type<10> m10;
typedef enumerated_type<11> m11;
typedef enumerated_type<12> m12;
typedef enumerated_type<13> m13;
typedef enumerated_type<14> m14;
typedef enumerated_type<15> m15;

typedef tmp::tree_node<m2,m0,m1> l0;
typedef tmp::tree_node<m5,m3,m4> l1;
typedef tmp::tree_node<m7,m6> l2;
typedef tmp::tree_node<m9,m8> l3;
typedef tmp::tree_node<m11,m10> l4;
typedef tmp::tree_node<m12,l0,l1,l2> l5;
typedef tmp::tree_node<m14,l3,l4,m13> l6;
typedef tmp::tree_node<m15,l5,l6> l7;

void test_tree()
{
    printf("Test of \"tmp::tree<TREE>\":\n");
    print_tuple<tmp::tree<l7>>::printme();
    printf("\n\n");
}

void test_power()
{
    printf("Test of \"tmp::power<BASE,POWER>\":\n");
    printf("2^0 = %d\n", tmp::math::power<2,0>::value);
    printf("2^1 = %d\n", tmp::math::power<2,1>::value);
    printf("2^2 = %d\n", tmp::math::power<2,2>::value);
    printf("4^0 = %d\n", tmp::math::power<4,0>::value);
    printf("4^2 = %d\n", tmp::math::power<4,2>::value);
    printf("4^5 = %d\n", tmp::math::power<4,5>::value);
    printf("\n");
}

void test_sum()
{
     printf("Test of \"tmp::sum<VS...> and tmp::sum_first<N,T<VS...>>\":\n");
     printf("1+2+3+4+5 = %d\n", tmp::math::sum<1,2,3,4,5>::value);
     printf("1+2+3 = %d\n", tmp::math::sum_first<3,tmp::tuple_i<1,2,3,4,5>>::value);
     printf("\n");
}

void test_signedness()
{
    printf("Test of \"tmp::is_unsigned<TYPE>\" and \"tmp::is_signed<TYPE>\":\n");
    printf("\"unsigned int\" is: %s\n",tmp::is_unsigned<unsigned int>::value ? "unsigned" : "signed");
    printf("\"signed int\" is: %s\n",tmp::is_unsigned<signed int>::value ? "unsigned" : "signed");
    printf("\"char\" is: %s\n",tmp::is_signed<char>::value ? "signed" : "unsigned");
    printf("\"signed char\" is: %s\n",tmp::is_signed<signed char>::value ? "signed" : "unsigned");
    printf("\"unsigned size_t\" is: %s\n",tmp::is_signed<unsigned int>::value ? "signed" : "unsigned");
    printf("\n");
}

void test_number_of_bits_of_type()
{
    printf("Test of \"tmp::number_of_bits<TYPE>\":\n");
    printf("Number of bits in \"unsigned char\": %d\n",tmp::number_of_bits_of_fundamental<unsigned char>::value);
    printf("Number of bits in \"char\": %d\n",tmp::number_of_bits_of_fundamental<char>::value);
    printf("Number of bits in \"unsigned int\": %d\n",tmp::number_of_bits_of_fundamental<unsigned int>::value);
    printf("Number of bits in \"size_t\": %d\n",tmp::number_of_bits_of_fundamental<size_t>::value);
    printf("Number of bits in \"long long int\": %d\n",tmp::number_of_bits_of_fundamental<long long int>::value);
    printf("Number of bits in \"signed\": %d\n",tmp::number_of_bits_of_fundamental<signed>::value);
    printf("\n");
}

void test_selecting_type_with_bit_size()
{
    printf("Test of \"tmp::get_signed_fundamental_of_bit_size\" and \"tmp::get_unsigned_fundamental_of_bit_size\":\n");
    printf("Selecting a signed type with 8 bits: %d\n", tmp::number_of_bits_of_fundamental<tmp::get_signed_fundamental_of_bit_size<8>>::value);
    printf("Selecting a signed type with 16 bits: %d\n", tmp::number_of_bits_of_fundamental<tmp::get_signed_fundamental_of_bit_size<16>>::value);
    printf("Selecting a signed type with 32 bits: %d\n", tmp::number_of_bits_of_fundamental<tmp::get_signed_fundamental_of_bit_size<32>>::value);
    printf("Selecting a signed type with 64 bits: %d\n", tmp::number_of_bits_of_fundamental<tmp::get_signed_fundamental_of_bit_size<64>>::value);
    printf("Selecting an unsigned type with 8 bits: %d\n", tmp::number_of_bits_of_fundamental<tmp::get_unsigned_fundamental_of_bit_size<8>>::value);
    printf("Selecting an unsigned type with 16 bits: %d\n", tmp::number_of_bits_of_fundamental<tmp::get_unsigned_fundamental_of_bit_size<16>>::value);
    printf("Selecting an unsigned type with 32 bits: %d\n", tmp::number_of_bits_of_fundamental<tmp::get_unsigned_fundamental_of_bit_size<32>>::value);
    printf("Selecting an unsigned type with 64 bits: %d\n", tmp::number_of_bits_of_fundamental<tmp::get_unsigned_fundamental_of_bit_size<64>>::value);
    printf("\n");
}

void test_explicitly_sized_types()
{
    enum { byte_size = tmp::number_of_bits_of_fundamental<char>::value };
    int v1 = 1;
    int v2 = 2;
    int v3 = 3;
    int v4 = 4;
    tmp::unsigned_sized_type<8,8>    t1(v1);
    tmp::unsigned_sized_type<16,16>  t2(v2);
    tmp::unsigned_sized_type<32,32>  t3(v3);
    tmp::unsigned_sized_type<64,64>  t4(v4);

    printf("Test of \"tmp::unsigned_sized_type\":\n");
    printf("sizeof(unsigned_sized_type<8,8>): %d\n", sizeof(t1)*byte_size);
    printf("sizeof(unsigned_sized_type<16,16>): %d\n", sizeof(t2)*byte_size);
    printf("sizeof(unsigned_sized_type<32,32>): %d\n", sizeof(t3)*byte_size);
    printf("sizeof(unsigned_sized_type<64,64>): %d\n", sizeof(t4)*byte_size);
    printf("alignof(unsigned_sized_type<8,8>): %d\n", alignof(t1)*byte_size);
    printf("alignof(unsigned_sized_type<16,16>): %d\n", alignof(t2)*byte_size);
    printf("alignof(unsigned_sized_type<32,32>): %d\n", alignof(t3)*byte_size);
    printf("alignof(unsigned_sized_type<64,64>): %d\n", alignof(t4)*byte_size);
    printf("Value of unsigned_sized_type<8,8> should be %d and is %d.\n", v1, tmp::fundamental_cast<int>(t1));
    printf("Value of unsigned_sized_type<16,16> should be %d and is %d.\n", v2, tmp::fundamental_cast<int>(t2));
    printf("Value of unsigned_sized_type<32,32> should be %d and is %d.\n", v3, tmp::fundamental_cast<int>(t3));
    printf("Value of unsigned_sized_type<64,64> should be %d and is %d.\n", v4, tmp::fundamental_cast<int>(t4));
    printf("8 bit: 1+1\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(1) + tmp::unsigned_sized_type<8,8>(1)));
    printf("8 bit: 15-8\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(15) - tmp::unsigned_sized_type<8,8>(8)));
    printf("8 bit: 14*17\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(14) * tmp::unsigned_sized_type<8,8>(17)));
    printf("8 bit: 100/25\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(100) / tmp::unsigned_sized_type<8,8>(25)));
    printf("8 bit: 8%3\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(8) % tmp::unsigned_sized_type<8,8>(3)));
    tmp::unsigned_sized_type<8,8>    t8_1(10);
    tmp::unsigned_sized_type<8,8>    t8_2(15);
    printf("8 bit: 10+15\t= %d\n", tmp::fundamental_cast<int>(t8_1 += t8_2));
    printf("8 bit: 25-15\t= %d\n", tmp::fundamental_cast<int>(t8_1 -= t8_2));
    printf("8 bit: 10-15\t= %d\n", tmp::fundamental_cast<int>(t8_1 - t8_2));
    printf("8 bit: 10*15\t= %d\n", tmp::fundamental_cast<int>(t8_1 *= t8_2));
    printf("8 bit: 150/15\t= %d\n", tmp::fundamental_cast<int>(t8_1 /= t8_2));
    printf("8 bit: 10%15\t= %d\n", tmp::fundamental_cast<int>(t8_1 %= t8_2));
    printf("8 bit: 10+1\t= %d\n", tmp::fundamental_cast<int>(t8_1++));
    printf("8 bit: 11+1\t= %d\n", tmp::fundamental_cast<int>(++t8_1));
    printf("8 bit: 12-1\t= %d\n", tmp::fundamental_cast<int>(t8_1--));
    printf("8 bit: 11-1\t= %d\n", tmp::fundamental_cast<int>(--t8_1));
    printf("8 bit: 1=2\t: %s\n", tmp::unsigned_sized_type<8,8>(1) == tmp::unsigned_sized_type<8,8>(2) ? "true" : "false");
    printf("8 bit: 123=123\t: %s\n", tmp::unsigned_sized_type<8,8>(123) == tmp::unsigned_sized_type<8,8>(123) ? "true" : "false");
    printf("8 bit: 1!=2\t: %s\n", tmp::unsigned_sized_type<8,8>(1) != tmp::unsigned_sized_type<8,8>(2) ? "true" : "false");
    printf("8 bit: 123!=123\t: %s\n", tmp::unsigned_sized_type<8,8>(123) != tmp::unsigned_sized_type<8,8>(123) ? "true" : "false");
    printf("8 bit: 1<2\t: %s\n", tmp::unsigned_sized_type<8,8>(1) < tmp::unsigned_sized_type<8,8>(2) ? "true" : "false");
    printf("8 bit: 123<123\t: %s\n", tmp::unsigned_sized_type<8,8>(123) < tmp::unsigned_sized_type<8,8>(123) ? "true" : "false");
    printf("8 bit: 1>2\t: %s\n", tmp::unsigned_sized_type<8,8>(1) > tmp::unsigned_sized_type<8,8>(2) ? "true" : "false");
    printf("8 bit: 123>123\t: %s\n", tmp::unsigned_sized_type<8,8>(123) > tmp::unsigned_sized_type<8,8>(123) ? "true" : "false");
    printf("8 bit: 1<=2\t: %s\n", tmp::unsigned_sized_type<8,8>(1) <= tmp::unsigned_sized_type<8,8>(2) ? "true" : "false");
    printf("8 bit: 123<=123\t: %s\n", tmp::unsigned_sized_type<8,8>(123) <= tmp::unsigned_sized_type<8,8>(123) ? "true" : "false");
    printf("8 bit: 1>=2\t: %s\n", tmp::unsigned_sized_type<8,8>(1) >= tmp::unsigned_sized_type<8,8>(2) ? "true" : "false");
    printf("8 bit: 123>=123\t: %s\n", tmp::unsigned_sized_type<8,8>(123) >= tmp::unsigned_sized_type<8,8>(123) ? "true" : "false");
    printf("8 bit: !0\t= %d\n", tmp::fundamental_cast<int>(!tmp::unsigned_sized_type<8,8>(0)));
    printf("8 bit: !1\t= %d\n", tmp::fundamental_cast<int>(!tmp::unsigned_sized_type<8,8>(1)));
    printf("8 bit: 1 OR 0\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(1) || tmp::unsigned_sized_type<8,8>(0)));
    printf("8 bit: 0 OR 0\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(0) || tmp::unsigned_sized_type<8,8>(0)));
    printf("8 bit: 1 OR 1\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(1) || tmp::unsigned_sized_type<8,8>(1)));
    printf("8 bit: 1 AND 0\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(1) && tmp::unsigned_sized_type<8,8>(0)));
    printf("8 bit: 0 AND 0\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(0) && tmp::unsigned_sized_type<8,8>(0)));
    printf("8 bit: 1 AND 1\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(1) && tmp::unsigned_sized_type<8,8>(1)));
    printf("8 bit: ~0\t= %d\n", tmp::fundamental_cast<int>(~tmp::unsigned_sized_type<8,8>(0)));
    printf("8 bit: 7 & 14\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(7) & tmp::unsigned_sized_type<8,8>(14)));
    printf("8 bit: 7 | 14\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(7) | tmp::unsigned_sized_type<8,8>(14)));
    printf("8 bit: 7 ^ 14\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(7) ^ tmp::unsigned_sized_type<8,8>(14)));
    printf("8 bit: 7 << 2\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(7) << tmp::unsigned_sized_type<8,8>(2)));
    printf("8 bit: 7 >> 2\t= %d\n", tmp::fundamental_cast<int>(tmp::unsigned_sized_type<8,8>(7) >> tmp::unsigned_sized_type<8,8>(2)));
    printf("8 bit: 10 & 14\t= %d\n", tmp::fundamental_cast<int>(t8_1 &= tmp::unsigned_sized_type<8,8>(14)));
    printf("8 bit: 10 | 14\t= %d\n", tmp::fundamental_cast<int>(t8_1 |= tmp::unsigned_sized_type<8,8>(14)));
    printf("8 bit: 14 ^ 14\t= %d\n", tmp::fundamental_cast<int>(t8_1 ^= tmp::unsigned_sized_type<8,8>(14)));
    t8_1 = 1;
    printf("8 bit: 1 << 2\t= %d\n", tmp::fundamental_cast<int>(t8_1 <<= tmp::unsigned_sized_type<8,8>(2)));
    printf("8 bit: 4 >> 2\t= %d\n", tmp::fundamental_cast<int>(t8_1 >>= tmp::unsigned_sized_type<8,8>(2)));
    printf("\n");
}

void test_sized_bitfield()
{
    tmp::sized_bitfield<32,32,10,7,4,4,7> bf1(413,101,11,3,110);
    printf("Test of \"tmp::sized_bitfield<S,A,FIELDS...>\":\n");
    printf("First element should be 413: %d\n",tmp::get_field<0>(bf1));
    printf("Second element should be 101: %d\n",tmp::get_field<1>(bf1));
    printf("Third element should be 11: %d\n",tmp::get_field<2>(bf1));
    printf("Fourth element should be 3: %d\n",tmp::get_field<3>(bf1));
    printf("Fifth element should be 110: %d\n",tmp::get_field<4>(bf1));
    printf("\n");
}

void test_tuple()
{
    tmp::detail::tuple<int,double,bool> t0{1,2.0f,false};
    printf("Test of \"tmp::detail::tuple<TS...>\":\n");
    printf("Current values of t0 should be (1,2.0,false) and are: (%d,%f,%s)\n",tmp::get<0>(t0),tmp::get<1>(t0),tmp::get<2>(t0) ? "true" : "false");
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
    printf("\nThirtieth prime:\t%d",tmp::math::prime<30>::value);

    printf("\n");

}

void test_select_argument()
{
    printf("\nTesting tmp::select_argument<N>(args...):\n");
    printf("tmp::select_argument<0>(13,281,22,9,0,112) = %d\n", tmp::select_argument<0>(13,281,22,9,0,112));
    printf("tmp::select_argument<1>(13,281,22,9,0,112) = %d\n", tmp::select_argument<1>(13,281,22,9,0,112));
    printf("tmp::select_argument<2>(13,281,22,9,0,112) = %d\n", tmp::select_argument<2>(13,281,22,9,0,112));
    printf("tmp::select_argument<3>(13,281,22,9,0,112) = %d\n", tmp::select_argument<3>(13,281,22,9,0,112));
    printf("tmp::select_argument<4>(13,281,22,9,0,112) = %d\n", tmp::select_argument<4>(13,281,22,9,0,112));
    printf("tmp::select_argument<5>(13,281,22,9,0,112) = %d\n", tmp::select_argument<5>(13,281,22,9,0,112));
    printf("\nTesting tmp::select_argument_lambda<N>(args...):\n");
    auto l1 = tmp::select_argument_lambda<0>();
    auto l2 = tmp::select_argument_lambda<1>();
    auto l3 = tmp::select_argument_lambda<2>();
    auto l4 = tmp::select_argument_lambda<3>();
    printf("lambda<0>(13,281,22,9,0,112,14) = %d\n", l1(13,281,22,9,0,112,14));
    printf("lambda<1>(13,281,22,9,0,112,14) = %d\n", l2(13,281,22,9,0,112,14));
    printf("lambda<2>(13,281,22,9,0,112,14) = %d\n", l3(13,281,22,9,0,112,14));
    printf("lambda<3>(13,281,22,9,0,112,14) = %d\n", l4(13,281,22,9,0,112,14));
}

int fwdfn1(int a, int b, int c)                 { return a+b+c; }
int fwdfn2(int a, int b)                        { return a-b; }
int fwdfn3(int a, int b, int c, int d)          { return a+b-c*d; }
int fwdfn4(int a, int b, int c)                 { return a+b*c; }
int fwdfn5(int a, int b, int c, int d, int e)   { return a*b+c*d*e; }

void test_select_and_forward_arguments()
{
    printf("\nTesting tmp::select_and_forward_arguments<NS...>(fn,args...):\n");
    printf("tmp::select_and_forward_arguments<0,2,4>(fwdfn1,13,281,22,9,0,112) = %d\n", tmp::select_and_forward_arguments<0,2,4>(fwdfn1,13,281,22,9,0,112));
    printf("tmp::select_and_forward_arguments<0,1>(fwdfn2,13,281,22,9,0,112) = %d\n", tmp::select_and_forward_arguments<0,1>(fwdfn2,13,281,22,9,0,112));
    printf("tmp::select_and_forward_arguments<0,2,4,1>(fwdfn3,13,281,22,9,0,112) = %d\n", tmp::select_and_forward_arguments<0,2,4,1>(fwdfn3,13,281,22,9,0,112));
    printf("tmp::select_and_forward_arguments<5,3,2>(fwdfn4,13,281,22,9,0,112) = %d\n", tmp::select_and_forward_arguments<5,3,2>(fwdfn4,13,281,22,9,0,112));
    printf("tmp::select_and_forward_arguments<0,0,1,3,0>(fwdfn5,13,281,22,9,0,112) = %d\n", tmp::select_and_forward_arguments<0,0,1,3,0>(fwdfn5,13,281,22,9,0,112));
}

int add(int a, int b) { return a + b; }
int square(int a, int b, int c, int d) { return a*a + b*b + c*c + d*d; }

void test_compose_functions()
{
    printf("\nTesting tmp::detail::compose_functions<...>(...):\n");
    
    // 2*2 + 5*5 + 8*8 + 3*3 - 2*2 + (5+6)(5+6) = 25 + 64 + 9 + 121 = 89 + 130 = 219
    printf("compose(square,add,0,1,2,3,4,5,6,7,8) = %d\n",tmp::detail::compose_functions<0,tmp::tuple_i<2,5,8,3>,tmp::tuple_i<5,6>>(square,add,0,1,2,3,4,5,6,7,8));
}

void test_all()
{
    test_type_comparation();
    test_summation_with_compare();
    test_prepending_tuple_of_tuples();
    test_tree();
    test_power();
    test_sum();
    test_signedness();
    test_number_of_bits_of_type();
    test_selecting_type_with_bit_size();
    test_explicitly_sized_types();
    test_sized_bitfield();
    test_tuple();
    test_primes();
    test_select_argument();
    test_select_and_forward_arguments();
    test_compose_functions();
}













#endif // TMP_TEST_H
