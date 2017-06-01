#include "test_data.h"

void test_sized_bitfield()
{
    tmp::data::sized_bitfield<32,32,10,7,4,4,7> bf1(413,101,11,3,110);
    printf("Test of \"tmp::sized_bitfield<S,A,FIELDS...>\":\n");
    printf("First element should be 413: %d\n",tmp::data::get_field<0>(bf1));
    printf("Second element should be 101: %d\n",tmp::data::get_field<1>(bf1));
    printf("Third element should be 11: %d\n",tmp::data::get_field<2>(bf1));
    printf("Fourth element should be 3: %d\n",tmp::data::get_field<3>(bf1));
    printf("Fifth element should be 110: %d\n",tmp::data::get_field<4>(bf1));

    tmp::data::set_field<2>(bf1,14);
    tmp::data::set_field<3>(bf1,29);
    printf("First element should be 413: %d\n",tmp::data::get_field<0>(bf1));
    printf("Second element should be 101: %d\n",tmp::data::get_field<1>(bf1));
    printf("Third element should be 14: %d\n",tmp::data::get_field<2>(bf1));
    printf("Fourth element should be 13: %d\n",tmp::data::get_field<3>(bf1));
    printf("Fifth element should be 110: %d\n",tmp::data::get_field<4>(bf1));

    tmp::data::sized_bitfield<64,64> bf2(123);
    printf("First element should be 123: %llu\n",tmp::data::get_field<0>(bf2));
    printf("\n");
}

void test_tuple()
{
    tmp::data::detail::tuple<int,double,bool> t0{1,2.0f,false};
    printf("Test of \"tmp::detail::tuple<TS...>\":\n");
    printf("Current values of t0 should be (1,2.0,false) and are: (%d,%f,%s)\n",tmp::data::get<0>(t0),tmp::data::get<1>(t0),tmp::data::get<2>(t0) ? "true" : "false");
    printf("\n");
}