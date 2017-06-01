#include "test_types.h"

void test_signedness()
{
    printf("Test of \"tmp::is_unsigned<TYPE>\" and \"tmp::is_signed<TYPE>\":\n");
    printf("\"unsigned int\" is: %s\n",tmp::types::is_unsigned<unsigned int> ? "unsigned" : "signed");
    printf("\"signed int\" is: %s\n",tmp::types::is_unsigned<signed int> ? "unsigned" : "signed");
    printf("\"char\" is: %s\n",tmp::types::is_signed<char> ? "signed" : "unsigned");
    printf("\"signed char\" is: %s\n",tmp::types::is_signed<signed char> ? "signed" : "unsigned");
    printf("\"unsigned int\" is: %s\n",tmp::types::is_signed<unsigned int> ? "signed" : "unsigned");
    printf("\n");
}

void test_number_of_bits_of_type()
{
    printf("Test of \"tmp::number_of_bits<TYPE>\":\n");
    printf("Number of bits in \"unsigned char\": %d\n",tmp::types::number_of_bits_of_fundamental<unsigned char>);
    printf("Number of bits in \"char\": %d\n",tmp::types::number_of_bits_of_fundamental<char>);
    printf("Number of bits in \"unsigned int\": %d\n",tmp::types::number_of_bits_of_fundamental<unsigned int>);
    printf("Number of bits in \"size_t\": %d\n",tmp::types::number_of_bits_of_fundamental<size_t>);
    printf("Number of bits in \"long long int\": %d\n",tmp::types::number_of_bits_of_fundamental<long long int>);
    printf("Number of bits in \"signed\": %d\n",tmp::types::number_of_bits_of_fundamental<signed>);
    printf("\n");
}

void test_selecting_type_with_bit_size()
{
    printf("Test of \"tmp::get_signed_fundamental_of_bit_size\" and \"tmp::get_unsigned_fundamental_of_bit_size\":\n");
    printf("Selecting a signed type with 8 bits: %d\n", tmp::types::number_of_bits_of_fundamental<tmp::types::get_signed_fundamental_of_bit_size<8>>);
    printf("Selecting a signed type with 16 bits: %d\n", tmp::types::number_of_bits_of_fundamental<tmp::types::get_signed_fundamental_of_bit_size<16>>);
    printf("Selecting a signed type with 32 bits: %d\n", tmp::types::number_of_bits_of_fundamental<tmp::types::get_signed_fundamental_of_bit_size<32>>);
    printf("Selecting a signed type with 64 bits: %d\n", tmp::types::number_of_bits_of_fundamental<tmp::types::get_signed_fundamental_of_bit_size<64>>);
    printf("Selecting an unsigned type with 8 bits: %d\n", tmp::types::number_of_bits_of_fundamental<tmp::types::get_unsigned_fundamental_of_bit_size<8>>);
    printf("Selecting an unsigned type with 16 bits: %d\n", tmp::types::number_of_bits_of_fundamental<tmp::types::get_unsigned_fundamental_of_bit_size<16>>);
    printf("Selecting an unsigned type with 32 bits: %d\n", tmp::types::number_of_bits_of_fundamental<tmp::types::get_unsigned_fundamental_of_bit_size<32>>);
    printf("Selecting an unsigned type with 64 bits: %d\n", tmp::types::number_of_bits_of_fundamental<tmp::types::get_unsigned_fundamental_of_bit_size<64>>);
    printf("\n");
}

void test_explicitly_sized_types()
{
    using namespace tmp::types;
    enum { byte_size = number_of_bits_of_fundamental<char> };
    int v1 = 1;
    int v2 = 2;
    int v3 = 3;
    int v4 = 4;
    unsigned_sized_type<8,8>    t1(v1);
    unsigned_sized_type<16,16>  t2(v2);
    unsigned_sized_type<32,32>  t3(v3);
    unsigned_sized_type<64,64>  t4(v4);

    printf("Test of \"tmp::types::unsigned_sized_type\":\n");
    printf("sizeof(unsigned_sized_type<8,8>): %llu\n", sizeof(t1)*byte_size);
    printf("sizeof(unsigned_sized_type<16,16>): %llu\n", sizeof(t2)*byte_size);
    printf("sizeof(unsigned_sized_type<32,32>): %llu\n", sizeof(t3)*byte_size);
    printf("sizeof(unsigned_sized_type<64,64>): %llu\n", sizeof(t4)*byte_size);
    printf("alignof(unsigned_sized_type<8,8>): %llu\n", alignof(decltype(t1))*byte_size);
    printf("alignof(unsigned_sized_type<16,16>): %llu\n", alignof(decltype(t2))*byte_size);
    printf("alignof(unsigned_sized_type<32,32>): %llu\n", alignof(decltype(t3))*byte_size);
    printf("alignof(unsigned_sized_type<64,64>): %llu\n", alignof(decltype(t4))*byte_size);
    printf("Value of unsigned_sized_type<8,8> should be %d and is %d.\n", v1, fundamental_cast<int>(t1));
    printf("Value of unsigned_sized_type<16,16> should be %d and is %d.\n", v2, fundamental_cast<int>(t2));
    printf("Value of unsigned_sized_type<32,32> should be %d and is %d.\n", v3, fundamental_cast<int>(t3));
    printf("Value of unsigned_sized_type<64,64> should be %d and is %d.\n", v4, fundamental_cast<int>(t4));
    printf("8 bit: 1+1\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(1) + unsigned_sized_type<8,8>(1)));
    printf("8 bit: 15-8\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(15) - unsigned_sized_type<8,8>(8)));
    printf("8 bit: 14*17\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(14) * unsigned_sized_type<8,8>(17)));
    printf("8 bit: 100/25\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(100) / unsigned_sized_type<8,8>(25)));
    printf("8 bit: 8mod3\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(8) % unsigned_sized_type<8,8>(3)));
    unsigned_sized_type<8,8>    t8_1(10);
    unsigned_sized_type<8,8>    t8_2(15);
    printf("8 bit: 10+15\t= %d\n", fundamental_cast<int>(t8_1 += t8_2));
    printf("8 bit: 25-15\t= %d\n", fundamental_cast<int>(t8_1 -= t8_2));
    printf("8 bit: 10-15\t= %d\n", fundamental_cast<int>(t8_1 - t8_2));
    printf("8 bit: 10*15\t= %d\n", fundamental_cast<int>(t8_1 *= t8_2));
    printf("8 bit: 150/15\t= %d\n", fundamental_cast<int>(t8_1 /= t8_2));
    printf("8 bit: 10mod15\t= %d\n", fundamental_cast<int>(t8_1 %= t8_2));
    printf("8 bit: 10+1\t= %d\n", fundamental_cast<int>(t8_1++));
    printf("8 bit: 11+1\t= %d\n", fundamental_cast<int>(++t8_1));
    printf("8 bit: 12-1\t= %d\n", fundamental_cast<int>(t8_1--));
    printf("8 bit: 11-1\t= %d\n", fundamental_cast<int>(--t8_1));
    printf("8 bit: 1=2\t: %s\n", unsigned_sized_type<8,8>(1) == unsigned_sized_type<8,8>(2) ? "true" : "false");
    printf("8 bit: 123=123\t: %s\n", unsigned_sized_type<8,8>(123) == unsigned_sized_type<8,8>(123) ? "true" : "false");
    printf("8 bit: 1!=2\t: %s\n", unsigned_sized_type<8,8>(1) != unsigned_sized_type<8,8>(2) ? "true" : "false");
    printf("8 bit: 123!=123\t: %s\n", unsigned_sized_type<8,8>(123) != unsigned_sized_type<8,8>(123) ? "true" : "false");
    printf("8 bit: 1<2\t: %s\n", unsigned_sized_type<8,8>(1) < unsigned_sized_type<8,8>(2) ? "true" : "false");
    printf("8 bit: 123<123\t: %s\n", unsigned_sized_type<8,8>(123) < unsigned_sized_type<8,8>(123) ? "true" : "false");
    printf("8 bit: 1>2\t: %s\n", unsigned_sized_type<8,8>(1) > unsigned_sized_type<8,8>(2) ? "true" : "false");
    printf("8 bit: 123>123\t: %s\n", unsigned_sized_type<8,8>(123) > unsigned_sized_type<8,8>(123) ? "true" : "false");
    printf("8 bit: 1<=2\t: %s\n", unsigned_sized_type<8,8>(1) <= unsigned_sized_type<8,8>(2) ? "true" : "false");
    printf("8 bit: 123<=123\t: %s\n", unsigned_sized_type<8,8>(123) <= unsigned_sized_type<8,8>(123) ? "true" : "false");
    printf("8 bit: 1>=2\t: %s\n", unsigned_sized_type<8,8>(1) >= unsigned_sized_type<8,8>(2) ? "true" : "false");
    printf("8 bit: 123>=123\t: %s\n", unsigned_sized_type<8,8>(123) >= unsigned_sized_type<8,8>(123) ? "true" : "false");
    printf("8 bit: !0\t= %d\n", fundamental_cast<int>(!unsigned_sized_type<8,8>(0)));
    printf("8 bit: !1\t= %d\n", fundamental_cast<int>(!unsigned_sized_type<8,8>(1)));
    printf("8 bit: 1 OR 0\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(1) || unsigned_sized_type<8,8>(0)));
    printf("8 bit: 0 OR 0\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(0) || unsigned_sized_type<8,8>(0)));
    printf("8 bit: 1 OR 1\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(1) || unsigned_sized_type<8,8>(1)));
    printf("8 bit: 1 AND 0\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(1) && unsigned_sized_type<8,8>(0)));
    printf("8 bit: 0 AND 0\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(0) && unsigned_sized_type<8,8>(0)));
    printf("8 bit: 1 AND 1\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(1) && unsigned_sized_type<8,8>(1)));
    printf("8 bit: ~0\t= %d\n", fundamental_cast<int>(~unsigned_sized_type<8,8>(0)));
    printf("8 bit: 7 & 14\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(7) & unsigned_sized_type<8,8>(14)));
    printf("8 bit: 7 | 14\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(7) | unsigned_sized_type<8,8>(14)));
    printf("8 bit: 7 ^ 14\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(7) ^ unsigned_sized_type<8,8>(14)));
    printf("8 bit: 7 << 2\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(7) << unsigned_sized_type<8,8>(2)));
    printf("8 bit: 7 >> 2\t= %d\n", fundamental_cast<int>(unsigned_sized_type<8,8>(7) >> unsigned_sized_type<8,8>(2)));
    printf("8 bit: 10 & 14\t= %d\n", fundamental_cast<int>(t8_1 &= unsigned_sized_type<8,8>(14)));
    printf("8 bit: 10 | 14\t= %d\n", fundamental_cast<int>(t8_1 |= unsigned_sized_type<8,8>(14)));
    printf("8 bit: 14 ^ 14\t= %d\n", fundamental_cast<int>(t8_1 ^= unsigned_sized_type<8,8>(14)));
    t8_1 = 1;
    printf("8 bit: 1 << 2\t= %d\n", fundamental_cast<int>(t8_1 <<= unsigned_sized_type<8,8>(2)));
    printf("8 bit: 4 >> 2\t= %d\n", fundamental_cast<int>(t8_1 >>= unsigned_sized_type<8,8>(2)));
    printf("\n");
}