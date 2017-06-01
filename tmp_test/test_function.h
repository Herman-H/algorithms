#ifndef TEST_FUNCTION_H
#define TEST_FUNCTION_H

#include <cstdio>
#include "tmp_function.h"

void test_select_argument();
void test_select_and_forward_arguments();
void test_compose_function_tree();

template <typename T, typename ... TS>
int variadic_sum(T t, TS ... ts)
{
    return t + variadic_sum(ts...);
}

#endif // TEST_FUNCTION_H