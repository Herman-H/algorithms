#ifndef TMP_PREDICATES_H
#define TMP_PREDICATES_H

#include <cstddef>

/* Forward declarations */
namespace tmp{namespace detail{template <typename,typename>
struct types_equal_impl;}}
namespace tmp{template <size_t>
struct int_type;}
namespace tmp{namespace detail{template <typename T>
struct number_of_bits_of_fundamental_;}}
/* End of forward declarations*/

namespace tmp
{
template <typename L>
struct compare
{
    template <typename R>
    struct with
    {
        enum { value = detail::types_equal_impl<L,R>::value };
    };
};
template <size_t B>
struct compare_type_size
{
    template <typename R>
    struct with_size_of
    {
        enum { value = (detail::number_of_bits_of_fundamental_<R>::value == B) };
    };
};
template <typename T>
struct int_type_of_bit_size
{
    typedef int_type<detail::number_of_bits_of_fundamental_<T>::value> type;
};

} // namespace tmp

#endif // TMP_PREDICATES_H
