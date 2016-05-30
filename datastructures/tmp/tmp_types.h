#ifndef TMP_TYPES_H
#define TMP_TYPES_H

#include <stddef.h>
#include <type_traits>
#include "tmp.h"
#include "tmp_math.h"
#include "tmp_predicates.h"

namespace tmp
{
namespace detail
{
    typedef tuple_t<signed char,
                    signed wchar_t,
                    signed short,
                    signed short int,
                    signed,
                    signed int,
                    signed long,
                    signed long int,
                    signed long long,
                    signed long long int,
                    signed size_t> signed_fundamentals;
    typedef tuple_t<unsigned char,
                    unsigned wchar_t,
                    unsigned short,
                    unsigned short int,
                    unsigned,
                    unsigned int,
                    unsigned long,
                    unsigned long int,
                    unsigned long long,
                    unsigned long long int,
                    unsigned size_t> unsigned_fundamentals;

    typedef set_of<signed_fundamentals> signed_fundamentals_set;
    typedef set_of<unsigned_fundamentals> unsigned_fundamentals_set;
} // namespace detail

    template <typename T>
    struct is_unsigned
    {
        enum { value = (static_cast<T>(0)-1 > static_cast<T>(0)) };
    };
    template <typename T>
    struct is_signed
    {
        enum { value = !is_unsigned<T>::value };
    };

namespace detail
{
    template <typename T, T VAL, bool IS_ZERO>
    struct number_of_bits_of_value_impl
    {
        static const T shifted_val = static_cast<T>(VAL << 1);
        enum { value = 1 + number_of_bits_of_value_impl<T, shifted_val, shifted_val == 0>::value };
    };
    template <typename T, T VAL>
    struct number_of_bits_of_value_impl<T,VAL,true>
    {
        enum { value = 0 };
    };
} // namespace detail

    template <typename T>
    struct number_of_bits_of_fundamental
    {
        enum { value = detail::number_of_bits_of_value_impl<T, static_cast<T>(1), (static_cast<T>(1) << 1) == 0>::value };
    };

namespace detail
{
    template <size_t B>
    struct compare_alignment_size
    {
        template <typename R>
        struct with_alignment_of
        {
            enum { value = (alignof(R)*number_of_bits_of_fundamental<char>::value == B) };
        };
    };
    template <size_t S, size_t A>
    struct compare_size_and_alignment
    {
        template <typename R>
        struct with_size_and_alignment_of
        {
            enum { value = (
            (number_of_bits_of_fundamental<R>::value == S) &&
            (alignof(R)*number_of_bits_of_fundamental<char>::value == A)) };
        };
    };
} // namespace detail

    template <size_t S>
    using get_signed_fundamental_of_bit_size = get_element_of<detail::signed_fundamentals_set>::template that_satisfies<compare_type_size<S>::template with_size_of>;
    template <size_t S>
    using get_unsigned_fundamental_of_bit_size = get_element_of<detail::unsigned_fundamentals_set>::template that_satisfies<compare_type_size<S>::template with_size_of>;
    template <size_t A>
    using get_signed_fundamental_of_alignment = get_element_of<detail::signed_fundamentals_set>::template that_satisfies<detail::compare_alignment_size<A>::template with_alignment_of>;
    template <size_t A>
    using get_unsigned_fundamental_of_alignment = get_element_of<detail::unsigned_fundamentals_set>::template that_satisfies<detail::compare_alignment_size<A>::template with_alignment_of>;
    template <size_t S, size_t A>
    using get_signed_fundamental_of_bit_size_and_alignment = get_element_of<detail::signed_fundamentals_set>::template that_satisfies<detail::compare_size_and_alignment<S,A>::template with_size_and_alignment_of>;
    template <size_t S, size_t A>
    using get_unsigned_fundamental_of_bit_size_and_alignment = get_element_of<detail::unsigned_fundamentals_set>::template that_satisfies<detail::compare_size_and_alignment<S,A>::template with_size_and_alignment_of>;

namespace detail
{
    /* These definitions are required in order to select types with "smallest type that has at least x number of bits", etc */
    struct explicitly_sized_type
    {
    };
    /*Forward declaration*/
    template <typename T>
    struct sized_type;
} // namespace detail

    template <typename R, typename T>
    R fundamental_cast(detail::sized_type<T> const& a);

namespace detail
{
    template <typename T>
    struct sized_type : public explicitly_sized_type
    {
    private:
        T data;
    public:
        sized_type()
        { }
        ~sized_type()
        { }
        sized_type(const sized_type & d) :
            data(d.data)
        { }
        template <typename FUNDAMENTAL>
        sized_type(const FUNDAMENTAL& c) :
            data(c)
        { }
        sized_type& operator=(const sized_type& c)
        {
            data = c.data;
            return *this;
        }
        /* This usually works but can incur compiler warnings */
        template <typename FUNDAMENTAL>
        sized_type& operator=(const FUNDAMENTAL& c)
        {
            data = c;
            return *this;
        }

        /* "Inherit" all operations that can be used on fundamental types here */
        // Addition
        friend sized_type operator+(sized_type const& l, sized_type const& r)
        {
            return sized_type(l.data + r.data);
        }
        sized_type& operator+=(sized_type& r)
        {
            data += r.data;
            return *this;
        }
        // Subtraction
        friend sized_type<T> operator-(sized_type<T> const& l, sized_type<T> const& r)
        {
            return sized_type<T>(l.data - r.data);
        }
        sized_type& operator-=(sized_type& r)
        {
            data -= r.data;
            return *this;
        }
        // Additive inverse
        friend sized_type<T> operator -(sized_type<T> const& l)
        {
            return sized_type<T>(-l.data);
        }
        // Multiplication
        friend sized_type<T> operator*(sized_type<T> const& l, sized_type<T> const& r)
        {
            return sized_type<T>(l.data * r.data);
        }
        sized_type& operator*=(sized_type const& r)
        {
            data *= r.data;
            return *this;
        }
        // Division
        friend sized_type<T> operator/(sized_type<T> const& l, sized_type<T> const& r)
        {
            return sized_type<T>(l.data / r.data);
        }
        sized_type& operator/=(sized_type const& r)
        {
            data /= r.data;
            return *this;
        }
        // Modulo
        friend sized_type<T> operator%(sized_type<T> const& l, sized_type<T> const& r)
        {
            return sized_type<T>(l.data % r.data);
        }
        sized_type& operator%=(sized_type const& r)
        {
            data %= r.data;
            return *this;
        }
        // Prefix increment
        sized_type& operator++()
        {
            data += 1;
            return *this;
        }
        // Postfix increment
        sized_type operator++(int)
        {
            data += 1;
            return *this;
        }
        // Prefix decrement
        sized_type& operator--()
        {
            data -= 1;
            return *this;
        }
        // Postfix decrement
        sized_type operator--(int)
        {
            data -= 1;
            return *this;
        }
        // Equal to
        friend bool operator ==(sized_type const& l, sized_type const& r)
        {
            return l.data == r.data;
        }
        // Not equal to
        friend bool operator !=(sized_type const& l, sized_type const& r)
        {
            return l.data != r.data;
        }
        // Greater than
        friend bool operator >(sized_type const& l, sized_type const& r)
        {
            return l.data > r.data;
        }
        // Less than
        friend bool operator <(sized_type const& l, sized_type const& r)
        {
            return l.data < r.data;
        }
        // Greater than or equal to
        friend bool operator >=(sized_type const& l, sized_type const& r)
        {
            return l.data >= r.data;
        }
        // Less than or equal to
        friend bool operator <=(sized_type const& l, sized_type const& r)
        {
            return l.data <= r.data;
        }
        // Logical negation
        friend sized_type<T> operator !(sized_type<T> l)
        {
            return sized_type<T>(!l.data);
        }
        // Logical AND
        friend sized_type<T> operator &&(sized_type<T> l, sized_type<T> r)
        {
            return sized_type<T>(l.data && r.data);
        }
        // Logical OR
        friend sized_type<T> operator ||(sized_type<T> l, sized_type<T> r)
        {
            return sized_type<T>(l.data || r.data);
        }
        // Bitwise NOT
        friend sized_type<T> operator ~(sized_type<T> l)
        {
            return sized_type<T>(~l.data);
        }
        // Bitwise AND
        friend sized_type<T> operator &(sized_type<T> l, sized_type<T> r)
        {
            return sized_type<T>(l.data & r.data);
        }
        sized_type& operator &=(sized_type r)
        {
            data &= r.data;
            return *this;
        }
        // Bitwise OR
        friend sized_type<T> operator |(sized_type<T> l, sized_type<T> r)
        {
            return sized_type<T>(l.data | r.data);
        }
        sized_type& operator |=(sized_type r)
        {
            data |= r.data;
            return *this;
        }
        // Bitwise XOR
        friend sized_type<T> operator ^(sized_type<T> l, sized_type<T> r)
        {
            return sized_type<T>(l.data ^ r.data);
        }
        sized_type& operator ^=(sized_type r)
        {
            data ^= r.data;
            return *this;
        }
        // Bitwise left shift
        friend sized_type<T> operator <<(sized_type<T> l, sized_type<T> r)
        {
            return sized_type<T>(l.data << r.data);
        }
        template <typename FUNDAMENTAL>
        friend sized_type<T> operator <<(sized_type<T> l, FUNDAMENTAL r)
        {
            return sized_type<T>(l.data << r);
        }
        sized_type& operator <<=(sized_type r)
        {
            data <<= r.data;
            return *this;
        }
        template <typename FUNDAMENTAL>
        sized_type& operator <<=(FUNDAMENTAL r)
        {
            data <<= r;
        }
        // Bitwise right shift
        friend sized_type<T> operator >>(sized_type<T> l, sized_type<T> r)
        {
            return sized_type<T>(l.data >> r.data);
        }
        template <typename FUNDAMENTAL>
        friend sized_type<T> operator >>(sized_type<T> l, FUNDAMENTAL r)
        {
            return sized_type<T>(l.data >> r);
        }
        sized_type& operator >>=(sized_type r)
        {
            data >>= r.data;
            return *this;
        }
        template <typename FUNDAMENTAL>
        sized_type& operator >>=(FUNDAMENTAL r)
        {
            data >>= r;
        }

        template <typename R>
        friend R tmp::fundamental_cast(sized_type<T> const& a){ return static_cast<R>(a.data); }
    };
} // namespace detail

    template <size_t S, size_t A> using unsigned_sized_type = detail::sized_type<get_unsigned_fundamental_of_bit_size_and_alignment<S,A>>;
    template <size_t S, size_t A> using signed_sized_type = detail::sized_type<get_signed_fundamental_of_bit_size_and_alignment<S,A>>;

namespace detail
{
    template <typename T, size_t N>
    struct ones
    {
        static const T value = math::power<2,N-1>::value + ones<T,N-1>::value;
    };
    template <typename T>
    struct ones<T,0>
    {
        static const T value = 0;
    };
} // namespace detail

    template <size_t S, size_t A, size_t ... FIELDS>
    struct sized_bitfield;
    template <size_t I, typename FUNDAMENTAL, size_t S, size_t A, size_t ... FIELDS>
    void set_field(sized_bitfield<S,A,FIELDS...> & a, FUNDAMENTAL const& b);
    template <size_t S, size_t A>
    struct sized_bitfield<S,A> : public detail::explicitly_sized_type
    {
    private:
        typedef typename get_unsigned_fundamental_of_bit_size_and_alignment<S,A>::type type;
        type data;
    public:
        sized_bitfield(){}
        ~sized_bitfield(){}
        sized_bitfield(sized_bitfield<S,A> const& a) :
            data(a.data)
        { }
        template <typename FUNDAMENTAL>
        sized_bitfield(FUNDAMENTAL const& a) :
            data(a)
        { }
        template <size_t I>
        friend type get_field(sized_bitfield<S,A> const& a)
        {
            static_assert(I == 0, "Index out of range.\n");
            return a.data;
        }
        template <size_t I, typename FUNDAMENTAL>
        friend void set_field(sized_bitfield<S,A> & a, FUNDAMENTAL const& s)
        {
            static_assert(I == 0, "Index out of range.\n");
            a.data = s;
        }
    };

    template <size_t S, size_t A, size_t F, size_t ... FIELDS>
    struct sized_bitfield<S,A,F,FIELDS...> : public detail::explicitly_sized_type
    {
    private:
        typedef get_unsigned_fundamental_of_bit_size_and_alignment<S,A> type;
        type data;

    public:
        sized_bitfield(){}
        ~sized_bitfield(){}
        sized_bitfield(sized_bitfield<S,A,F,FIELDS...> const& c) :
            data(c.data)
        { }
        template <size_t I>
        friend type get_field(sized_bitfield<S,A,F,FIELDS...> const& a)
        {
            static_assert(I >= 0 && I < sizeof...(FIELDS)+1, "Index out of range.\n");
            enum { offset = math::sum_first<I,tuple_i<F,FIELDS...>>::value };
            enum { size = get_element_at_i<I,F,FIELDS...>::value };
            return (a.data >> offset) & detail::ones<type,size>::value;
        }
        template <size_t I, typename FUNDAMENTAL>
        friend void set_field(sized_bitfield<S,A,F,FIELDS...> & a, FUNDAMENTAL const& s)
        {
            static_assert(I >= 0 && I < sizeof...(FIELDS)+1, "Index out of range.\n");
            enum { offset = math::sum_first<I,tuple_i<F,FIELDS...>>::value };
            enum { size = get_element_at_i<I,F,FIELDS...>::value };
            a.data |= ((s & detail::ones<type,size>::value) << offset);
        }
     private:
        template <size_t N, typename ... TS>
        struct helper
        {
            static void impl(sized_bitfield * c, TS const& ... ts){}
        };
        template <size_t N, typename T, typename ... TS>
        struct helper<N,T,TS...>
        {
            static void impl(sized_bitfield * c, T const& t, TS const& ... ts)
            {
                set_field<N>(*c, t);
                helper<N+1,TS...>::impl(c, ts...);
            }
        };
     public:
        template <typename ... TS>
        sized_bitfield(TS ... ts) : data(0)
        {
            static_assert(sizeof...(TS) <= sizeof...(FIELDS)+1, "Too many arguments provided.\n");
            helper<0, TS...>::impl(this,ts...);
        }
    };

namespace detail
{
    template <typename SEQ, typename ... TS>
    struct tpl_gen_leafs;
    template <size_t ... SEQ, typename ... TS>
    struct tpl_gen_leafs<tuple_i<SEQ...>,TS...> : protected detail::leaf_node<SEQ,TS>...
    {

    };
    template <typename ... TS>
    struct tuple;
} // namespace detail

template <size_t N, typename ... TS>
get_element_at<N,TS...>& get(detail::tuple<TS...> & t);

namespace detail
{
    template <typename T, typename ... TS>
    struct tuple<T,TS...> : public tpl_gen_leafs<sequence<sizeof...(TS)+1>,T,TS...>
    {
        template <size_t N, typename ... PS> using leaf_type = get_element_at<N,PS...>;
        template <size_t N, typename ... PS>
        friend leaf_type<N,PS...>& tmp::get(tuple<PS...> & t){ return get_leaf_data(static_cast<leaf_node<N,leaf_type<N,PS...>>&>(t),int_type<std::is_class<leaf_type<N,PS...>>::value>()); }

        tuple(){ }
        ~tuple(){ }
        tuple(T const& t, TS const& ... ts)
        {
            for_all<0,T,TS...>::init(*this, t, ts...);
        }

    private:
        template <size_t I, typename ... PS>
        struct for_all
        {
            static void init(tuple & tup, PS const& ... ps){ }
        };
        template <size_t I, typename P, typename ... PS>
        struct for_all<I,P,PS...>
        {
            static void init(tuple & tup, P const& p, PS const& ... ps)
            {
                tmp::get<I>(tup) = p;
                for_all<I+1,PS...>::init(tup,ps...);
            }
        };
    };

    typedef map<int_type_of_bit_size,signed_fundamentals_set> signed_fundamentals_sizes;
    typedef map<int_type_of_bit_size,unsigned_fundamentals_set> unsigned_fundamentals_sizes;

    template <size_t S, size_t A>
    struct composed_sized_type : explicitly_sized_type
    {
    private:
        /* Select the type such that bitsoftype(type)*N=size where N is an integer*/
        enum { greatest_divisor = math::greatest_divisor<S, unsigned_fundamentals_sizes>::value };
    public:
    };

} // namespace detail

} // namespace tmp


























#endif // TMP_TYPES_H
