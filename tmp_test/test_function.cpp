#include "test_function.h"

void test_select_argument()
{
    printf("\nTesting tmp::fn::select_argument<N>(args...):\n");
    printf("tmp::fn::select_argument<0>(13,281,22,9,0,112) = %d\n", tmp::fn::select_argument<0>(13,281,22,9,0,112));
    printf("tmp::fn::select_argument<1>(13,281,22,9,0,112) = %d\n", tmp::fn::select_argument<1>(13,281,22,9,0,112));
    printf("tmp::fn::select_argument<2>(13,281,22,9,0,112) = %d\n", tmp::fn::select_argument<2>(13,281,22,9,0,112));
    printf("tmp::fn::select_argument<3>(13,281,22,9,0,112) = %d\n", tmp::fn::select_argument<3>(13,281,22,9,0,112));
    printf("tmp::fn::select_argument<4>(13,281,22,9,0,112) = %d\n", tmp::fn::select_argument<4>(13,281,22,9,0,112));
    printf("tmp::fn::select_argument<5>(13,281,22,9,0,112) = %d\n", tmp::fn::select_argument<5>(13,281,22,9,0,112));
    printf("\nTesting tmp::fn::select_argument_lambda<N>(args...):\n");
    auto l1 = tmp::fn::select_argument_lambda<0>();
    auto l2 = tmp::fn::select_argument_lambda<1>();
    auto l3 = tmp::fn::select_argument_lambda<2>();
    auto l4 = tmp::fn::select_argument_lambda<3>();
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
    printf("\nTesting tmp::fn::select_and_forward_arguments<NS...>(fn,args...):\n");
    printf("tmp::fn::select_and_forward_arguments<0,2,4>(fwdfn1,13,281,22,9,0,112) = %d\n", tmp::fn::select_and_forward_arguments<0,2,4>(fwdfn1,13,281,22,9,0,112));
    printf("tmp::fn::select_and_forward_arguments<0,1>(fwdfn2,13,281,22,9,0,112) = %d\n", tmp::fn::select_and_forward_arguments<0,1>(fwdfn2,13,281,22,9,0,112));
    printf("tmp::fn::select_and_forward_arguments<0,2,4,1>(fwdfn3,13,281,22,9,0,112) = %d\n", tmp::fn::select_and_forward_arguments<0,2,4,1>(fwdfn3,13,281,22,9,0,112));
    printf("tmp::fn::select_and_forward_arguments<5,3,2>(fwdfn4,13,281,22,9,0,112) = %d\n", tmp::fn::select_and_forward_arguments<5,3,2>(fwdfn4,13,281,22,9,0,112));
    printf("tmp::fn::select_and_forward_arguments<0,0,1,3,0>(fwdfn5,13,281,22,9,0,112) = %d\n", tmp::fn::select_and_forward_arguments<0,0,1,3,0>(fwdfn5,13,281,22,9,0,112));
    printf("\nTesting tmp::fn::select_and_forward_arguments_lambda<NS...>(fn,args...):\n");
    auto l1 = tmp::fn::select_and_forward_arguments_lambda<0,2,4>();
    auto l2 = tmp::fn::select_and_forward_arguments_lambda<0,1>();
    auto l3 = tmp::fn::select_and_forward_arguments_lambda<0,2,4,1>();
    auto l4 = tmp::fn::select_and_forward_arguments_lambda<5,3,2>();
    auto l5 = tmp::fn::select_and_forward_arguments_lambda<0,0,1,3,0>();
    printf("lambda<0,2,4>(fwdfn1,13,281,22,9,0,112) = %d\n", l1(fwdfn1,13,281,22,9,0,112));
    printf("lambda<0,1>(fwdfn2,13,281,22,9,0,112) = %d\n", l2(fwdfn2,13,281,22,9,0,112));
    printf("lambda<0,2,4,1>(fwdfn3,13,281,22,9,0,112) = %d\n", l3(fwdfn3,13,281,22,9,0,112));
    printf("lambda<5,3,2>(fwdfn4,13,281,22,9,0,112) = %d\n", l4(fwdfn4,13,281,22,9,0,112));
    printf("lambda<0,0,1,3,0>(fwdfn5,13,281,22,9,0,112) = %d\n", l5(fwdfn5,13,281,22,9,0,112));
}

int tcftd_f0(int a,double b,int c,int d,int e)
{
    return a + b + c + d + e;
}

int tcftd_f1(double a, double b, double c)
{
    return a*b*c;
}

int tcftd_f2(int a,int b,double c)
{
    return b < c ? a : b;
}

int tcftd_f3(int a,double b,double c)
{
    return a < c ? a : b;
}

/*
 *  arg0 -> int,int,int,int
 *  arg1 -> double,double,double
 *  arg2 -> double,double
 *  arg3 -> int
 *  arg4 -> double,double
 *  arg5 -> int
 *  fun1 -> int
 *  fun2 -> int
 *  fun3 -> int
 *
 *
 */

void test_compose_function_tree()
{
    typedef tmp::fn::fn<0>::
                with_args<  tmp::fn::arg<0>,
                            tmp::fn::arg<1>,
                            tmp::fn::fn<3>::with_args<  tmp::fn::arg<0>,
                                                        tmp::fn::arg<2>,
                                                        tmp::fn::arg<4>>,
                            tmp::fn::fn<2>::with_args<  tmp::fn::fn<0>::with_args<  tmp::fn::arg<0>,
                                                                                    tmp::fn::arg<1>,
                                                                                    tmp::fn::arg<5>,
                                                                                    tmp::fn::arg<0>,
                                                                                    tmp::fn::arg<3>>,
                                                        tmp::fn::fn<1>::with_args<  tmp::fn::arg<4>,
                                                                                    tmp::fn::arg<4>,
                                                                                    tmp::fn::arg<2>>,
                                                        tmp::fn::arg<1>>,
                            tmp::fn::arg<5>>
            CT;
    /*typedef tmp::fn::fn<0>::
                with_args<  tmp::fn::arg<0>,
                            tmp::fn::arg<1>,
                            tmp::fn::fn<0>::with_args<  tmp::fn::arg<0>,
                                                        tmp::fn::arg<2>,
                                                        tmp::fn::arg<4>>,
                            tmp::fn::fn<0>::with_args<  tmp::fn::fn<0>::with_args<  tmp::fn::arg<0>,
                                                                                    tmp::fn::arg<1>,
                                                                                    tmp::fn::arg<5>,
                                                                                    tmp::fn::arg<0>,
                                                                                    tmp::fn::arg<3>>,
                                                        tmp::fn::fn<0>::with_args<  tmp::fn::arg<4>,
                                                                                    tmp::fn::arg<4>,
                                                                                    tmp::fn::arg<2>>,
                                                        tmp::fn::arg<1>>,
                            tmp::fn::arg<5>>
            CTGL;*/

    auto l = tmp::fn::compose_function_tree_lambda<CT>(tcftd_f0,tcftd_f1,tcftd_f2,tcftd_f3);
    int lr = l(4, 7.3, 3.14, 1, 0.4, -14);
    int b =
        tcftd_f0(4,
                 7.3,
                 tcftd_f3(4,
                          3.14,
                          0.4),
                 tcftd_f2(tcftd_f0(4,
                                   7.3,
                                   -14,
                                   4,
                                   1),
                          tcftd_f1(0.4,
                                   0.4,
                                   3.14),
                          7.3),
                 -14);

    printf("\nTesting tmp::detail::compose_function_tree<...>(...):\n");

    printf("b = %d\n",b);
    printf("l(4, 7.3, 3.14, 1, 0.4, -14) = %d\n",lr);

    /*auto glambda = [](auto ... vs)
    {
        return variadic_sum(vs...);
    };*/

    //auto sumlambda = tmp::compose_function_tree_lambda_with_interface<CTGL, int, tmp::tuple_t<int,int,int,int,int,int>>(glambda);

    //int a = sumlambda(0,1,2,3,4,5);
    //printf("a = %d\n",lr);

}