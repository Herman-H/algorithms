#include "test_tmp.h"
#include "test_math.h"
#include "test_types.h"
#include "test_data.h"
#include "test_function.h"
#include "test_rational.h"

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
    test_compose_function_tree();
    test_gcd();
    test_rational_arithmetic();
    test_rationald_arithmetic();
}

int main()
{
    test_all();
    return 0;
}