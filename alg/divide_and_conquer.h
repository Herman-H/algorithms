#ifndef DIVIDE_AND_CONQUER_H
#define DIVIDE_AND_CONQUER_H

#include <cstddef>
#include <vector>
#include <utility>
#include "../datastructures/priority_queue.h"
#include "../tmp/tmp_types.h"
#include "../tmp/tmp_function.h"

namespace alg
{
    namespace detail
    {
        template <typename ... TS>
        struct for_each_impl;
        template <>
        struct for_each_impl<>
        {
            template <typename L>
            static void eval(L && l){}
        };
        template <typename T, typename ... TS>
        struct for_each_impl<T,TS...>
        {
            template <typename L>
            static auto eval(L && l, const T && t, const TS && ... ts)
            {
                l(t);
                for_each_impl<TS...>::eval(std::forward<L>(l),std::forward<const TS>(ts)...);
            }
        };

        template <typename ... TS>
        auto for_each(const TS && ... ts)
        {
            return [&ts...](auto && l)
            {
                detail::for_each_impl<TS...>::eval(l,std::forward<const TS>(ts)...);
            };
        }
    } // namespace detail

    template <typename ... TS>
    std::vector<int> combine_sorted(const TS&& ... vs)
    {
        unsigned int is[sizeof...(vs)] = {0};
        const int mv = tmp::detail::ones<int,tmp::number_of_bits_of_fundamental<int>-1>::value;
        int sz = 0;
        std::vector<int> v;
        v.reserve(sz);
        detail::for_each(std::forward<const TS>(vs)...)([&sz](const std::vector<int,std::allocator<int>> v_)
        {
            sz += v_.size();
        });
        int min_value = mv;
        for(int i = 0; i < sz; ++i)
        {
            int min_idx = 0;
            int tmp_idx = 0;
            min_value = mv;
            detail::for_each(std::forward<const TS>(vs)...)([&is,&min_value,&min_idx,&tmp_idx,&i](const std::vector<int,std::allocator<int>> v_)
            {
                if(is[tmp_idx] < v_.size())
                {
                    if(v_.at(is[tmp_idx]) < min_value)
                    {
                        min_value = v_.at(is[tmp_idx]);
                        min_idx = tmp_idx;
                    }
                }
                ++tmp_idx;
            });
            v.push_back(min_value);
            is[min_idx] = is[min_idx]+1;
        }
        return v;
    }

    template <typename ... TS>
    std::vector<int> sort(const TS&& ... vs)
    {
        ds::priority_queue<int,sizeof...(vs)> pq;
        std::vector<int> v;
        v.reserve(sizeof...(vs));
        detail::for_each(std::forward<const TS>(vs)...)([&pq](const int value)
        {
            pq.insert(value);
        });
        while(!pq.is_empty())
        {
            v.push_back(pq.element());
            pq.decrement();
        }
        return v;
    }

    namespace detail
    {
        template <typename IDX>
        struct gen_inner_function;
        template <std::size_t ... IDX>
        struct gen_inner_function<tmp::tuple_i<IDX...>>
        {
            typedef tmp::select_fun<1>::with_arguments<tmp::select_arg<IDX>...> type;
        };
        template <typename SPLITS>
        struct splits_to_function_tree;
        template <typename ... SPLITS>
        struct splits_to_function_tree<tmp::tuple_t<SPLITS...>>
        {
            typedef tmp::select_fun<0>::with_arguments<
                        typename gen_inner_function<SPLITS>::type ...
                        > type;
        };
        /*template <typename SEQ, typename T>
        struct gen_args;
        template <std::size_t ... SEQ, typename T>
        struct gen_args<tmp::tuple_i<SEQ...>, T>
        {
            typedef
        };
        template <typename ARGS>
        struct gen_lambda;
        template <typename ... ARGS>
        struct gen_lambda
        {
            template <typename L>
            auto lambda(L && l)
            {
                return [&l](const ARGS && ... args){l(std::forward<const ARGS>(args)...)};
            }
        };*/
    } // namespace detail


    template <std::size_t N, std::size_t M, typename ... T>
    auto divide_and_conquer(const T&& ... ts)
    {
        enum { input_size = sizeof...(ts) };
        typedef tmp::split_tuple<tmp::sequence<sizeof...(ts)>, N> splits;
        typedef detail::splits_to_function_tree<splits> CT;
        return [&ts...](auto && combine, auto && func)
        {
            return (input_size / N) < M ? func(std::forward<const T>(ts)...) :
                                          tmp::compose_function_tree_lambda<CT>(
                                                [&combine](auto && ... vs){ return combine(std::forward<decltype(vs)>(vs)...); },
                                                [&combine,&func](auto && ... vs){
                                                    return divide_and_conquer<N,M>(std::forward<decltype(vs)>(vs)...)(combine,func);
                                                })(std::forward<const T>(ts)...);
        };
    }

//    template <typename ITERATOR, std::size_t N>
//    void divide_and_conquer(ITERATOR & begin, const ITERATOR & end)
//    {

//    }


} // namespace alg



#endif /* DIVIDE_AND_CONQUER_H */
