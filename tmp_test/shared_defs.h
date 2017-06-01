#ifndef SHARED_DEFS_H
#define SHARED_DEFS_H

template <size_t I>
struct enumerated_type
{
    static void printme()
    {
        printf("%llu",I);
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

#endif // SHARED_DEFS_H