#ifndef TMP_DATA_H
#define TMP_DATA_H

#include "tmp.h"
#include "tmp_types.h"
#include "tmp_math.h"

namespace tmp
{
    namespace data
    {
        
        namespace detail
        {
            template <typename T, size_t N>
            struct ones
            {
                static constexpr T value = math::power<2,N-1> + ones<T,N-1>::value;
            };
            template <typename T>
            struct ones<T,0>
            {
                static constexpr T value = 0;
            };
        } // namespace detail

        template <size_t S, size_t A, size_t ... FIELDS>
        struct sized_bitfield;
        template <size_t S, size_t A>
        struct sized_bitfield<S,A> : public tmp::types::detail::explicitly_sized_type
        {
        private:
            typedef tmp::types::get_unsigned_fundamental_of_bit_size_and_alignment<S,A> type;
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
            template <size_t I,size_t U, size_t V>
            friend tmp::types::get_unsigned_fundamental_of_bit_size_and_alignment<U,V> get_field(sized_bitfield<U,V> const& a);

            template <size_t I, size_t U, size_t V, typename FUNDAMENTAL>
            friend void set_field(sized_bitfield<U,V> & a, FUNDAMENTAL const& s);
        };

        template <size_t I, size_t S, size_t A>
        tmp::types::get_unsigned_fundamental_of_bit_size_and_alignment<S,A> get_field(sized_bitfield<S,A> const& a)
        {
            static_assert(I == 0, "Index out of range.\n");
            return a.data;
        }

        template <size_t I, size_t S, size_t A, typename FUNDAMENTAL>
        void set_field(sized_bitfield<S,A> & a, FUNDAMENTAL const& s)
        {
            static_assert(I == 0, "Index out of range.\n");
            a.data = s;
        }

        template <size_t S, size_t A, size_t F, size_t ... FIELDS>
        struct sized_bitfield<S,A,F,FIELDS...> : public tmp::types::detail::explicitly_sized_type
        {
        private:
            typedef tmp::types::get_unsigned_fundamental_of_bit_size_and_alignment<S,A> type;
            type data;

        public:
            sized_bitfield(){}
            ~sized_bitfield(){}
            sized_bitfield(sized_bitfield<S,A,F,FIELDS...> const& c) :
                data(c.data)
            { }
            template <size_t I, size_t U, size_t V, size_t G, size_t ... GS>
            friend tmp::types::get_unsigned_fundamental_of_bit_size_and_alignment<U,V> get_field(sized_bitfield<U,V,G,GS...> const& a);

            template <size_t I, size_t U, size_t V, size_t ... FS, typename FUNDAMENTAL>
            friend void set_field(sized_bitfield<U,V,FS...> & a, FUNDAMENTAL const& s);
         private:
            template <size_t N, typename ... TS>
            struct helper
            {
                static void impl(sized_bitfield *, TS const& ...){}
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

        template <size_t I, size_t S, size_t A, size_t F, size_t ... FIELDS>
        tmp::types::get_unsigned_fundamental_of_bit_size_and_alignment<S,A> get_field(sized_bitfield<S,A,F,FIELDS...> const& a)
        {
            static_assert(I >= 0 && I < sizeof...(FIELDS)+1, "Index out of range.\n");
            enum { offset = math::sum_first<I,tuple_i<F,FIELDS...>> };
            enum { size = get_element_at_i<I,F,FIELDS...>::value };
            return (a.data >> offset) & detail::ones<tmp::types::get_unsigned_fundamental_of_bit_size_and_alignment<S,A>,size>::value;
        }
        template <size_t I, size_t S, size_t A, size_t ... FIELDS, typename FUNDAMENTAL>
        void set_field(sized_bitfield<S,A,FIELDS...> & a, FUNDAMENTAL const& s)
        {
            static_assert(I >= 0 && I < sizeof...(FIELDS), "Index out of range.\n");
            enum { offset = math::sum_first<I,tuple_i<FIELDS...>> };
            enum { size = get_element_at_i<I,FIELDS...>::value };
            typedef tmp::types::get_unsigned_fundamental_of_bit_size_and_alignment<S,A> type;
            constexpr type mask = (~detail::ones<type,size>::value << offset)| detail::ones<type,offset>::value;
            a.data = (a.data & mask) | ((s << offset) & ~mask);
        }

        namespace detail
        {
            template <typename SEQ, typename ... TS>
            struct tpl_gen_leafs;
            template <size_t ... SEQ, typename ... TS>
            struct tpl_gen_leafs<tuple_i<SEQ...>,TS...> : protected ::tmp::detail::leaf_node<SEQ,TS>...
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
                friend leaf_type<N,PS...>& tmp::data::get(tuple<PS...> & t);

                tuple(){ }
                ~tuple(){ }
                tuple(T const&& t, TS const&& ... ts)
                {
                    for_all<0,T,TS...>::init(*this, std::forward<const T>(t), std::forward<const TS>(ts)...);
                }

            private:
                template <size_t I, typename ... PS>
                struct for_all
                {
                    static void init(tuple &, PS const&& ...){ }
                };
                template <size_t I, typename P, typename ... PS>
                struct for_all<I,P,PS...>
                {
                    static void init(tuple & tup, P const&& p, PS const&& ... ps)
                    {
                        tmp::data::get<I>(tup) = std::forward<const P>(p);
                        for_all<I+1,PS...>::init(tup,std::forward<const PS>(ps)...);
                    }
                };
            };

        }// namespace detail

        template <size_t N, typename ... TS>
        get_element_at<N,TS...>& get(detail::tuple<TS...> & t){ return get_leaf_data(static_cast<::tmp::detail::leaf_node<N,get_element_at<N,TS...>>&>(t),int_type<std::is_class<get_element_at<N,TS...>>::value>()); }
        template <typename ... TS>
        using tuple = detail::tuple<TS...>;

        namespace detail{

            template <typename T>
            struct int_type_of_bit_size
            {
                typedef int_type<tmp::types::detail::number_of_bits_of_fundamental_<T>::value> type;
            };

            typedef map<int_type_of_bit_size,tmp::types::detail::unsigned_fundamentals_set> unsigned_fundamentals_sizes;
            typedef unsigned_fundamentals_sizes signed_fundamentals_sizes;

            template <size_t S, size_t A>
            struct composed_sized_type : tmp::types::detail::explicitly_sized_type
            {
            private:
                /* Select the type such that bitsoftype(type)*N=size where N is an integer*/
                enum { greatest_divisor = math::greatest_divisor<S, unsigned_fundamentals_sizes> };
            public:
            };

        } // namespace detail
    }
}



















#endif // TMP_DATA_H