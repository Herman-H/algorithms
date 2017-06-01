#include "test_rational.h"

void test_gcd()
{
    printf("\nTesting tmp::rational::gcd<A,B>:\n");
    printf("GCD(124,14) = %lld\n",tmp::rational::detail::gcd<124,14>);
    printf("GCD(12412,124441) = %lld\n",tmp::rational::detail::gcd<12412,124441>);
    printf("GCD(-3313,6234) = %lld\n",tmp::rational::detail::gcd<-3313,6234>);
    printf("GCD(124142,-14322) = %lld\n",tmp::rational::detail::gcd<124142,-14322>);
    printf("GCD(-9823,-235241) = %lld\n",tmp::rational::detail::gcd<-9823,-235241>);
}

void test_rational_arithmetic()
{
    printf("\nTesting tmp::rational::rational<N,D>:\n");
    typedef tmp::rational::detail::rational_t<1,2> R1;
    typedef tmp::rational::detail::rational_t<5,3> R2;
    typedef tmp::rational::detail::rational_t<11,7> R3;
    typedef decltype(R1{}+R2{}) addition_expr;
    typedef decltype(R1{}+R2{}+R3{}) addition_expr1;
    typedef decltype((R1{}+R2{})+R3{}) addition_expr2;
    typedef decltype(R1{}+(R2{}+R3{})) addition_expr3;
    
    typedef addition_expr::evaluate addition_evaluation;
    typedef addition_expr1::evaluate addition_evaluation1;
    typedef addition_expr2::evaluate addition_evaluation2;
    typedef addition_expr3::evaluate addition_evaluation3;
    constexpr auto nominator1 = tmp::rational::detail::nominator<addition_evaluation>;
    constexpr auto denominator1 = tmp::rational::detail::denominator<addition_evaluation>;
    constexpr auto nominator2 = tmp::rational::detail::nominator<addition_evaluation1>;
    constexpr auto denominator2 = tmp::rational::detail::denominator<addition_evaluation1>;
    constexpr auto nominator3 = tmp::rational::detail::nominator<addition_evaluation2>;
    constexpr auto denominator3 = tmp::rational::detail::denominator<addition_evaluation2>;
    constexpr auto nominator4 = tmp::rational::detail::nominator<addition_evaluation3>;
    constexpr auto denominator4 = tmp::rational::detail::denominator<addition_evaluation3>;
    
    printf("1/2 + 5/3 = %d/%d\n",nominator1,denominator1);
    printf("1/2 + 5/3 + 11/7 = %d/%d\n",nominator2,denominator2);
    printf("(1/2 + 5/3) + 11/7 = %d/%d\n",nominator3,denominator3);
    printf("1/2 + (5/3 + 11/7) = %d/%d\n",nominator4,denominator4);
    
    typedef decltype(R1{}-R2{})::evaluate sub_expr;
    typedef decltype(R1{}-R2{}+R3{})::evaluate sub_expr1;
    typedef decltype((R1{}-R2{})-R3{})::evaluate sub_expr2;
    typedef decltype(R1{}-(R2{}-R3{}))::evaluate sub_expr3;
    
    constexpr auto nominator5 = tmp::rational::detail::nominator<sub_expr>;
    constexpr auto denominator5 = tmp::rational::detail::denominator<sub_expr>;
    constexpr auto nominator6 = tmp::rational::detail::nominator<sub_expr1>;
    constexpr auto denominator6 = tmp::rational::detail::denominator<sub_expr1>;
    constexpr auto nominator7 = tmp::rational::detail::nominator<sub_expr2>;
    constexpr auto denominator7 = tmp::rational::detail::denominator<sub_expr2>;
    constexpr auto nominator8 = tmp::rational::detail::nominator<sub_expr3>;
    constexpr auto denominator8 = tmp::rational::detail::denominator<sub_expr3>;
    
    printf("1/2 - 5/3 = %d/%d\n",nominator5,denominator5);
    printf("1/2 - 5/3 + 11/7 = %d/%d\n",nominator6,denominator6);
    printf("(1/2 - 5/3) - 11/7 = %d/%d\n",nominator7,denominator7);
    printf("1/2 - (5/3 - 11/7) = %d/%d\n",nominator8,denominator8);
    
    
    typedef decltype(R1{}*(R2{}*R3{} *R1{}*R2{}*R3{} *R1{}*R2{}*R3{} *R1{}*R2{}*R3{}))::evaluate expr;
    constexpr auto nominator9 = tmp::rational::detail::nominator<expr>;
    constexpr auto denominator9 = tmp::rational::detail::denominator<expr>;
    printf("(1/2 * 5/3 * 11/7)^4 = %d/%d\n",nominator9,denominator9);
}

void test_rationald_arithmetic()
{
    printf("\nTesting dynamic tmp::rational::rational:\n");
    using namespace tmp::rational::detail;
    test_all_additions<rational_t<1,1>,rational_t<1,1>>();
    test_all_additions<rational_t<3,2>,rational_t<1,1>>();
    test_all_additions<rational_t<3,2>,rational_t<4,5>>();
    
    
    printf("5.3 + 1/2 = %f\n",5.3 + rational_t<1,2>{});
    printf("5.3 / 1/2 = %f\n",5.3 / rational_t<1,2>{});
    printf("5.3 * 1/2 = %f\n",5.3 * rational_t<1,2>{});
}








