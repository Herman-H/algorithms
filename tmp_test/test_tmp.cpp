#include "test_tmp.h"

void test_type_comparation()
{
    printf("Test of \"tmp::types_equal<T1,T2>\":\n");
    bool types_should_equal = tmp::types_equal<int,int>;
    bool types_shouldnt_equal = tmp::types_equal<bool,int>;
    printf("int = int: %s\n",types_should_equal ? "true" : "false");
    printf("bool = int: %s\n", types_shouldnt_equal ? "true" : "false");
    printf("\n");
}

void test_sfinae()
{
    printf("Test of \"tmp::types_equal<T1,T2>\":\n");
    bool types_should_equal = tmp::types_equal<int,int>;
    bool types_shouldnt_equal = tmp::types_equal<bool,int>;
    printf("int = int: %s\n",types_should_equal ? "true" : "false");
    printf("bool = int: %s\n", types_shouldnt_equal ? "true" : "false");
    printf("\n");
}

void test_summation_with_compare()
{
    printf("Test of \"tmp::sum<PREDICATE>::of<TUPLE>\":\n");
    typedef tmp::tuple_t<int,int,bool,char,int,long,bool,float> tup1;
    size_t num1 = tmp::sum<test_detail::compare<int>::with>::of<tup1>::value;
    size_t num2 = tmp::sum<test_detail::compare<bool>::with>::of<tup1>::value;
    printf("Number of ints in tuple_t<int,int,bool,char,int,long,bool,float>: %llu\n", num1);
    printf("Number of bools in tuple_t<int,int,bool,char,int,long,bool,float>: %llu\n", num2);
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
