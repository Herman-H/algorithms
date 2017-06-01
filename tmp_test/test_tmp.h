#ifndef TEST_TMP_H
#define TEST_TMP_H

#include <cstdio>
#include "tmp.h"
#include "shared_defs.h"

void test_type_comparation();
void test_sfinae();
void test_summation_with_compare();
void test_prepending_tuple_of_tuples();
void test_tree();

namespace test_detail
{
    template <typename L>
    struct compare
    {
        template <typename R>
        struct with
        {
            enum { value = tmp::detail::types_equal_impl<L,R>::value };
        };
    };
}

#endif // TEST_TMP_H