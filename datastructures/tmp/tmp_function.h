#ifndef TMP_FUNCTION_H
#define TMP_FUNCTION_H

#include "tmp_types.h"
#include <utility>

namespace tmp
{
    namespace detail
    {
        struct is_functor;
        struct is_function;
        struct is_member_function;

        template <typename F>
        struct callable_type_impl
        {
            typedef is_functor type;
            typedef typename callable_type_impl<decltype(F::operator())>::arguments    arguments;
            typedef typename callable_type_impl<decltype(F::operator())>::return_type  return_type;
        };
        template <typename R, typename ... ARGS>
        struct callable_type_impl<R (*)(ARGS...)>
        {
            typedef is_function         type;
            typedef tuple_t<ARGS...>    arguments;
            typedef R                   return_type;
        };
        template <typename R, typename ... ARGS>
        struct callable_type_impl<R (&)(ARGS...)>
        {
            typedef is_function         type;
            typedef tuple_t<ARGS...>    arguments;
            typedef R                   return_type;
        };
        template <typename R, typename ... ARGS>
        struct callable_type_impl<R (*&)(ARGS...)>
        {
            typedef is_function         type;
            typedef tuple_t<ARGS...>    arguments;
            typedef R                   return_type;
        };
        template <typename C, typename R, typename ... ARGS>
        struct callable_type_impl<R (C::*)(ARGS...)>
        {
            typedef is_member_function  type;
            typedef tuple_t<ARGS...>    arguments;
            typedef R                   return_type;
        };
        template <typename C, typename R, typename ... ARGS>
        struct callable_type_impl<R (C::*)(ARGS...) const>
        {
            typedef is_member_function  type;
            typedef tuple_t<ARGS...>    arguments;
            typedef R                   return_type;
        };

        template <typename F>
        struct callable_type
        {
            typedef typename callable_type_impl<F>::type        type;
            typedef typename callable_type_impl<F>::arguments   arguments;
            typedef typename callable_type_impl<F>::return_type return_type;
        };

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

    template <size_t N>
    auto select_argument_lambda()
    {
        return [](const auto && ... args)
        {
            return select_argument<N>(std::forward<decltype(args)>(args)...);
        };
    }


    /*template <size_t ... NS, typename R, typename ... FNARGS, typename ... ARGS>
    R select_and_forward_arguments(R (*fn)(FNARGS...), ARGS && ... args)
    {
        return fn(select_argument<NS>(std::forward<ARGS>(args)...)...);
    }*/
    template <size_t ... NS, typename F, typename ... ARGS>
    typename detail::callable_type<F>::return_type select_and_forward_arguments(F && functor, ARGS && ... args)
    {
        return functor(select_argument<NS>(std::forward<ARGS>(args)...)...);
    }

    template <size_t ... NS>
    auto select_and_forward_arguments_lambda()
    {
        return [] (auto fn, auto && ... args)
        {
            return select_and_forward_arguments<NS...>(fn,std::forward<decltype(args)>(args)...);
        };
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
            /*template <typename R, typename ... FARGS, typename T, typename ... TS>
            static arg_type<N,T,TS...> select(R (*fn)(FARGS...), const T&&, const TS&& ... ts)
            {
                return N <= sizeof...(TS) ? select_argument_or_function_impl<THISN,FN,FSEL,N-1>::select(fn,std::forward<const TS>(ts)...) : arg_type<N,T,TS...>{};
            }*/
            template <typename F, typename T, typename ... TS>
            static arg_type<N,T,TS...> select(F && functor, const T&&, const TS&& ... ts)
            {
                return N <= sizeof...(TS) ? select_argument_or_function_impl<THISN,FN,FSEL,N-1>::select(std::forward<F>(functor),std::forward<const TS>(ts)...) : arg_type<N,T,TS...>{};
            }
        };

        template <size_t THISN, size_t FN, typename FSEL>
        struct select_argument_or_function_impl<THISN,FN,FSEL,0>
        {
            /*template <typename R, typename ... ARGS, typename T, typename ... TS>
            static T select(R (*)(ARGS...), const T&& t, const TS&& ...)
            {
                return t;
            }*/
            template <typename F, typename T, typename ... TS>
            static T select(F&&, const T&& t, const TS&& ...)
            {
                return std::forward<const T>(t);
            }
        };
        template <size_t THISN, size_t ... FSEL, size_t N>
        struct select_argument_or_function_impl<THISN,THISN,tuple_i<FSEL...>,N>
        {
            /*template <typename R, typename ... ARGS, typename ... TS>
            static R select(R (*fn)(ARGS...), TS&& ... ts)
            {
                return select_and_forward_arguments<FSEL...>(fn,std::forward<TS>(ts)...);
            }*/
            template <typename F, typename ... TS>
            static typename callable_type<F>::return_type select(F && functor, TS&& ... ts)
            {
                return select_and_forward_arguments<FSEL...>(std::forward<F>(functor),std::forward<TS>(ts)...);
            }
        };

        template <size_t F1F2, typename SEQ, typename F1SEL, typename F2SEL, typename RF1, typename RF2, typename F1ARGS, typename F2ARGS>
        struct compose_functions_impl;
        template <size_t F1F2, size_t ... SEQ, size_t ... F1SEL, typename F2SEL,
                  typename RF1, typename RF2, typename ... F1ARGS, typename ... F2ARGS>
        struct compose_functions_impl<F1F2, tuple_i<SEQ...>, tuple_i<F1SEL...>, F2SEL,
                RF1, RF2, tuple_t<F1ARGS...>, tuple_t<F2ARGS...>>
        {
            /*template <typename ... GARGS>
            static RF1 compose_functions(RF1 (*f1)(F1ARGS...), RF2 (*f2)(F2ARGS...), GARGS&& ... args)
            {
                return f1(select_argument_or_function_impl<SEQ,F1F2,F2SEL,F1SEL>::select(f2,std::forward<GARGS>(args)...)...);
            }
            template <typename LFUNCTOR, typename ... GARGS>
            static RF1 compose_functions(LFUNCTOR lfunctor, RF2 (*f2)(F2ARGS...), GARGS&& ... args)
            {
                return lfunctor(select_argument_or_function_impl<SEQ,F1F2,F2SEL,F1SEL>::select(f2,std::forward<GARGS>(args)...)...);
            }
            template <typename RFUNCTOR, typename ... GARGS>
            static RF1 compose_functions(RF1 (*f1)(F1ARGS...), RFUNCTOR rfunctor, GARGS&& ... args)
            {
                return f1(select_argument_or_function_impl<SEQ,F1F2,F2SEL,F1SEL>::select(rfunctor,std::forward<GARGS>(args)...)...);
            }*/
            template <typename LFUNCTOR,typename RFUNCTOR, typename ... GARGS>
            static RF1 compose_functions(LFUNCTOR && lfunctor, RFUNCTOR && rfunctor, GARGS&& ... args)
            {
                return lfunctor(select_argument_or_function_impl<SEQ,F1F2,F2SEL,F1SEL>::select(std::forward<RFUNCTOR>(rfunctor),std::forward<GARGS>(args)...)...);
            }
        };

        /*template <  size_t   F1F2,      // The function parameter index of F1 where F2 will be invoked
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
        template <  size_t   F1F2,      // The function parameter index of F1 where F2 will be invoked
                    typename F1SEL,     // A tuple specifying which argument to which function parameter for F1
                    typename F2SEL,     // A tuple specifying which argument to which function parameter for F2
                    typename LFUNCTOR,
                    typename RF2,       // F2 return type
                    typename ... F2ARGS,// F2 arguments
                    typename ... GARGS>
        typename callable_type<LFUNCTOR>::return_type compose_functions(LFUNCTOR lfunctor, RF2 (*f2)(F2ARGS...), GARGS&& ... args)
        {
            return compose_functions_impl<F1F2,
                            sequence<cardinality<typename callable_type<LFUNCTOR>::arguments>::value>,
                            F1SEL, F2SEL, typename callable_type<LFUNCTOR>::return_type, RF2,
                            typename callable_type<LFUNCTOR>::arguments, tuple_t<F2ARGS...>>::compose_functions
                   (lfunctor,f2,std::forward<GARGS>(args)...);
        }
        template <  size_t   F1F2,      // The function parameter index of F1 where F2 will be invoked
                    typename F1SEL,     // A tuple specifying which argument to which function parameter for F1
                    typename F2SEL,     // A tuple specifying which argument to which function parameter for F2
                    typename RF1,       // F1 return type
                    typename ... F1ARGS,// F1 arguments
                    typename RFUNCTOR,
                    typename ... GARGS>
        RF1 compose_functions(RF1 (*f1)(F1ARGS...), RFUNCTOR rfunctor, GARGS&& ... args)
        {
            return compose_functions_impl<F1F2,
                            sequence<sizeof...(F1ARGS)>,
                            F1SEL, F2SEL, RF1, typename callable_type<RFUNCTOR>::return_type,
                            tuple_t<F1ARGS...>, typename callable_type<RFUNCTOR>::arguments>::compose_functions
                   (f1,rfunctor,std::forward<GARGS>(args)...);
        }*/
        template <  size_t   F1F2,      // The function parameter index of F1 where F2 will be invoked
                    typename F1SEL,     // A tuple specifying which argument to which function parameter for F1
                    typename F2SEL,     // A tuple specifying which argument to which function parameter for F2
                    typename LFUNCTOR,
                    typename RFUNCTOR,
                    typename ... GARGS>
        typename callable_type<LFUNCTOR>::return_type compose_functions(LFUNCTOR && lfunctor, RFUNCTOR && rfunctor, GARGS&& ... args)
        {
            return compose_functions_impl<F1F2,
                            sequence<cardinality<typename callable_type<LFUNCTOR>::arguments>::value>,
                            F1SEL, F2SEL, typename callable_type<LFUNCTOR>::return_type, typename callable_type<RFUNCTOR>::return_type,
                            typename callable_type<RFUNCTOR>::arguments, typename callable_type<RFUNCTOR>::arguments>::compose_functions
                   (std::forward<LFUNCTOR>(lfunctor),std::forward<RFUNCTOR>(rfunctor),std::forward<GARGS>(args)...);
        }

        template <size_t F1F2, typename F1SEL, typename F2SEL>
        struct composed_selection;
        template <size_t F1F2, size_t ... F1SEL, size_t ... F2SEL>
        struct composed_selection<F1F2, tuple_i<F1SEL...>, tuple_i<F2SEL...>>
        {
            typedef merge_tuples_i<merge_tuples_i< typename select_first_i_impl<F1F2,tuple_i<>,F1SEL...>::type,
                                                   typename select_last_i_impl<F1F2+1,F1SEL...>::type >,
                                tuple_i<F2SEL...> >
            type;
        };

        template <size_t F1F2, typename F1ARGS, typename F2ARGS>
        struct composed_arguments;
        template <size_t F1F2, typename ... F1ARGS, typename ... F2ARGS>
        struct composed_arguments<F1F2, tuple_t<F1ARGS...>, tuple_t<F2ARGS...>>
        {
            typedef merge_tuples<merge_tuples<  typename detail::select_first_impl<F1F2,tuple_t<>,F1ARGS...>::type,
                                                typename detail::select_last_impl<F1F2+1,F1ARGS...>::type >,
                                tuple_t<F2ARGS...> >
            type;
        };

        // f1(S10,S12,..., (SF1F2) f2(SF20,...,SF2M), ... , S1N)
        template <  size_t   F1F2,      // The function parameter index of F1 where F2 will be invoked
                    typename RF1,       // F1 return type
                    typename RF2,       // F2 return type
                    typename F1ARGS,    // F1 arguments
                    typename F2ARGS,    // F2 arguments
                    typename CARGS>
        struct compose_functions_lambda_impl;
        template <  size_t   F1F2,      // The function parameter index of F1 where F2 will be invoked
                    typename RF1,       // F1 return type
                    typename RF2,       // F2 return type
                    typename ... F1ARGS,// F1 arguments
                    typename ... F2ARGS,// F2 arguments
                    typename ... CARGS>
        struct compose_functions_lambda_impl<F1F2,RF1,RF2,tuple_t<F1ARGS...>,tuple_t<F2ARGS...>,tuple_t<CARGS...>>
        {
            /*static auto lambda(RF1 (*f1)(F1ARGS...), RF2 (*f2)(F2ARGS...))
            {
                return [f1,f2] (CARGS && ... args)
                {
                    typedef typename detail::split_tuple_impl<tuple_i<sizeof...(F1ARGS)-1,sizeof...(F2ARGS)>,tuple_t<>,sequence<sizeof...(F1ARGS) + sizeof...(F2ARGS)-1>>::type split;
                    return compose_functions<   F1F2,
                                                typename insert_value_in<-1,get_element_in<0,split>>::template at<F1F2>,
                                                get_element_in<1,split> >
                                     (f1,f2,std::forward<CARGS>(args)...);

                };
            }*/
            template <typename L1, typename L2>
            static auto lambda(L1 && l1, L2 && l2)
            {
                return [&l1,&l2] (CARGS && ... args)
                {
                    typedef typename detail::split_tuple_impl<tuple_i<sizeof...(F1ARGS)-1,sizeof...(F2ARGS)>,tuple_t<>,sequence<sizeof...(F1ARGS) + sizeof...(F2ARGS)-1>>::type split;
                    return compose_functions<   F1F2,
                                                typename insert_value_in<-1,get_element_in<0,split>>::template at<F1F2>,
                                                get_element_in<1,split> >
                                     (std::forward<L1>(l1),std::forward<L2>(l2),std::forward<CARGS>(args)...);
                };
            }
            /*template <typename L1>
            static auto lambda(L1 l1, RF2 (*f2)(F2ARGS...), CARGS && ...)
            {
                return [l1,f2] (CARGS && ... args)
                {
                    typedef typename detail::split_tuple_impl<tuple_i<sizeof...(F1ARGS)-1,sizeof...(F2ARGS)>,tuple_t<>,sequence<sizeof...(F1ARGS) + sizeof...(F2ARGS)-1>>::type split;
                    return compose_functions<   F1F2,
                                                typename insert_value_in<-1,get_element_in<0,split>>::template at<F1F2>,
                                                get_element_in<1,split> >
                                     (l1,f2,std::forward<CARGS>(args)...);
                };
            }
            template <typename L2>
            static auto lambda(RF1 (*f1)(F1ARGS...), L2 l2, CARGS && ...)
            {
                return [f1,l2] (CARGS && ... args)
                {
                    typedef typename detail::split_tuple_impl<tuple_i<sizeof...(F1ARGS)-1,sizeof...(F2ARGS)>,tuple_t<>,sequence<sizeof...(F1ARGS) + sizeof...(F2ARGS)-1>>::type split;
                    return compose_functions<   F1F2,
                                                typename insert_value_in<-1,get_element_in<0,split>>::template at<F1F2>,
                                                get_element_in<1,split> >
                                     (f1,l2,std::forward<CARGS>(args)...);
                };
            }*/
        };
    } // namespace detail

        template <size_t F1F2, typename F1, typename F2>
        auto compose_functions_lambda(F1 && f1, F2 && f2)
        {
            return detail::compose_functions_lambda_impl<F1F2, typename detail::callable_type<F1>::return_type, typename detail::callable_type<F2>::return_type,
                    typename detail::callable_type<F1>::arguments, typename detail::callable_type<F2>::arguments,
                    typename detail::composed_arguments<F1F2,typename detail::callable_type<F1>::arguments, typename detail::callable_type<F2>::arguments
                    >::type>::lambda(std::forward<F1>(f1),std::forward<F2>(f2));
        }
        /*template <size_t F1F2, typename RF1, typename ... F1ARGS, typename F2>
        auto compose_functions_lambda(RF1 (*f1)(F1ARGS...), F2 f2)
        {
            return detail::compose_functions_lambda_impl<F1F2, RF1, typename detail::callable_type<F2>::return_type,
                    tuple_t<F1ARGS...>, typename detail::callable_type<F2>::arguments,
                    typename detail::composed_arguments<F1F2,tuple_t<F1ARGS...>, typename detail::callable_type<F2>::arguments>::type>::lambda(f1,f2);
        }
        template <size_t F1F2, typename F1, typename RF2, typename ... F2ARGS>
        auto compose_functions_lambda(F1 f1, RF2 (*f2)(F2ARGS...))
        {
            return detail::compose_functions_lambda_impl<F1F2, typename detail::callable_type<F1>::return_type, RF2,
                    typename detail::callable_type<F1>::arguments, tuple_t<F2ARGS...>,
                    typename detail::composed_arguments<F1F2,tuple_t<F2ARGS...>, tuple_t<F2ARGS...>>::type>::lambda(f1,f2);
        }

        template <size_t F1F2, typename RF1, typename ... F1ARGS, typename RF2, typename ... F2ARGS>
        auto compose_functions_lambda(RF1 (*f1)(F1ARGS...), RF2 (*f2)(F2ARGS...))
        {
            return detail::compose_functions_lambda_impl<F1F2, RF1, RF2,
                    tuple_t<F1ARGS...>, tuple_t<F2ARGS...>,
                    typename detail::composed_arguments<F1F2,tuple_t<F1ARGS...>, tuple_t<F2ARGS...>>::type>::lambda(f1,f2);
        }*/

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
