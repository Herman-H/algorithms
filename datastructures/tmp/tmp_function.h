#ifndef TMP_FUNCTION_H
#define TMP_FUNCTION_H

#include "tmp_types.h"
#include <utility>

namespace tmp
{
    namespace detail
    {
        template <size_t N, typename ... TS>
        using arg_type = get_element_at<N < sizeof...(TS), no_type, get_element_at<N, TS...>>;

        template <size_t N, typename ... TS>
        struct select_argument_impl
        {
            static no_type select(const TS&& ...){ return *reinterpret_cast<no_type*>(nullptr); }
        };

        template <size_t N, typename T, typename ... TS>
        struct select_argument_impl<N,T,TS...>
        {
            static arg_type<N,T,TS...> select(const T&&, const TS&& ... ts)
            {
                return select_argument_impl<N-1,TS...>::select(std::forward<const TS>(ts)...);
            }
        };
        template <typename T, typename ... TS>
        struct select_argument_impl<0,T,TS...>
        {
            static T select(const T&& t, const TS&& ...)
            {
                return t;
            }
        };

    } // namespace detail

    template <size_t N, typename ... TS>
    detail::arg_type<N,TS...> select_argument(const TS&& ... args)
    {
        static_assert(sizeof...(args) > N, "Index is out of range!\n");
        return detail::select_argument_impl<N,TS...>::select(std::forward<const TS>(args)...);
    }


    template <size_t ... NS, typename R, typename ... FNARGS, typename ... ARGS>
    R select_and_forward_arguments(R (*fn)(FNARGS...), ARGS && ... args)
    {
        return fn(select_argument<NS>(std::forward<ARGS>(args)...)...);
    }

    /*
     *  genfn<Rule>(f1,f2,f3,f4)(Args...)
     *
     *
     */

    namespace detail
    {
        template <size_t THISN, size_t FN, typename FSEL, size_t N>
        struct select_argument_or_function_impl
        {
            template <typename R, typename ... FARGS, typename T, typename ... TS>
            static arg_type<N,T,TS...> select(R (*fn)(FARGS...), const T&&, const TS&& ... ts)
            {
                return N <= sizeof...(TS) ? select_argument_or_function_impl<THISN,FN,FSEL,N-1>::select(fn,std::forward<const TS>(ts)...) : arg_type<N,T,TS...>{};
            }
        };

        template <size_t THISN, size_t FN, typename FSEL>
        struct select_argument_or_function_impl<THISN,FN,FSEL,0>
        {
            template <typename R, typename ... ARGS, typename T, typename ... TS>
            static T select(R (*fn)(ARGS...), const T&& t, const TS&& ...)
            {
                return t;
            }
        };
        template <size_t THISN, size_t ... FSEL, size_t N>
        struct select_argument_or_function_impl<THISN,THISN,tuple_i<FSEL...>,N>
        {
            template <typename R, typename ... ARGS, typename ... TS>
            static R select(R (*fn)(ARGS...), TS&& ... ts)
            {
                return select_and_forward_arguments<FSEL...>(fn,std::forward<TS>(ts)...);
            }
        };

        template <size_t F1F2, typename SEQ, typename F1SEL, typename F2SEL, typename RF1, typename RF2, typename F1ARGS, typename F2ARGS>
        struct compose_functions_impl;
        template <size_t F1F2, size_t ... SEQ, size_t ... F1SEL, typename F2SEL,
                  typename RF1, typename RF2, typename ... F1ARGS, typename ... F2ARGS>
        struct compose_functions_impl<F1F2, tuple_i<SEQ...>, tuple_i<F1SEL...>, F2SEL,
                RF1, RF2, tuple_t<F1ARGS...>, tuple_t<F2ARGS...>>
        {
            template <typename ... GARGS>
            static RF1 compose_functions(RF1 (*f1)(F1ARGS...), RF2 (*f2)(F2ARGS...), GARGS&& ... args)
            {
                return f1(select_argument_or_function_impl<SEQ,F1F2,F2SEL,F1SEL>::select(f2,std::forward<GARGS>(args)...)...);
            }
        };

        template <  size_t   F1F2,      // The function parameter index of F1 where F2 will be invoked
                    typename F1SEL,     // A tuple specifying which argument to which function parameter for F1
                    typename F2SEL,     // A tuple specifying which argument to which function parameter for F2
                    typename RF1,       // F1 return type
                    typename RF2,       // F2 return type
                    typename ... F1ARGS,// F1 arguments
                    typename ... F2ARGS,// F2 arguments
                    typename ... GARGS>
        RF1 compose_functions(RF1 (*f1)(F1ARGS...), RF2 (*f2)(F2ARGS...), GARGS&& ... args)
        {
            return compose_functions_impl<F1F2, 
                            sequence<sizeof...(F1ARGS)>, F1SEL, F2SEL, RF1, RF2, 
                            tuple_t<F1ARGS...>, tuple_t<F2ARGS...>>::compose_functions
                   (f1,f2,std::forward<GARGS>(args)...);
        }


    } // namespace detail

    // {T...} ---- split ----> {T-...} ---- FN ----> {R...} ---- COMBINE ----> {R+...}
    template <typename R, typename ... FNARGS, typename ... ARGS>
    R split_apply_combine(R (*combine)(FNARGS...), ARGS && ... args)
    {

    }

    namespace detail
    {
        template <typename T>
        struct tuple_arguments_impl;
        template <template <size_t...> class T, size_t ... NS>
        struct tuple_arguments_impl<T<NS...>>
        {
            template <typename R, typename ... FARGS, typename ... ARGS>
            static R invoke(R (*fn)(FARGS...), ARGS && ... args)
            {
                return select_and_forward_arguments<NS...>(fn, std::forward(args)...);
            }
        };

        template <size_t A>
        struct function_argument_binding
        {
        };

        template <size_t F, typename ... ARGUMENTS>
        struct function_binding
        {

        };

        template <typename T>
        struct function_or_argument_impl;
        template <size_t A>
        struct function_or_argument_impl<function_argument_binding<A>>
        {
            typedef function_argument_binding<A> type;
        };
        template <size_t F, typename ... ARGS>
        struct function_or_argument_impl<function_binding<F, ARGS...>>
        {
            typedef tree_node<int_type<F>, typename function_or_argument_impl<ARGS>::type...> type;
        };

        template <typename CONFIG>
        struct function_configuration
        {
        };

        //template <typename FUNCTION_CONFIG>
        //struct unique_function_

        // FUNCTOR: template <typename R, typename ... ARGS> struct functor;

        template <typename F>
        using functor_return_type = get_element_in<0, F>;
        template <size_t N, typename F>
        using functor_argument = get_element_at<N+1,F>;

        template <typename AS, typename FS, typename SAS, size_t SFS>
        struct generate_general_functor_impl;
        template <typename ... AS, typename ... FS, size_t ... SAS, size_t SFS>
        struct generate_general_functor_impl<tuple_t<AS...>, tuple_t<FS...>, tuple_i<SAS...>, SFS>
        {
            typedef get_element_at<SFS, FS...>      functor;
            typedef functor_return_type<functor>    return_type;

            return_type execute(functor& f, get_element_at<SAS, AS...>&& ... args)
            {
                return f(std::forward<get_element_at<SAS, AS...>>(args)...);
            }
        };

        template <typename ARGS, typename FUNCTORS>
        struct compose_functors_configuration;
        template <typename ... ARGS, typename ... FUNCTORS>
        struct compose_functors_configuration<tuple_t<ARGS...>,tuple_t<FUNCTORS...>>
        {
            template <typename CONFIGURATION>
            struct select;
        };

        template <typename CONFIGURATION>
        struct compose_functors;


    } // namespace detail

} // namespace tmp

#endif /* TMP_FUNCTION_H */
