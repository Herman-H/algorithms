#ifndef TMP_PREDICATES_H
#define TMP_PREDICATES_H

#include <cstddef>

/* Forward declarations */
namespace tmp{template <typename,typename>
struct types_equal;}
namespace tmp{template <size_t>
struct int_type;}
namespace tmp{template <typename T>
struct number_of_bits_of_fundamental;}
/* End of forward declarations*/

namespace tmp
{
template <typename L>
struct compare
{
    template <typename R>
    struct with
    {
        enum { value = types_equal<L,R>::value };
    };
};
template <size_t B>
struct compare_type_size
{
    template <typename R>
    struct with_size_of
    {
        enum { value = (number_of_bits_of_fundamental<R>::value == B) };
    };
};
template <typename T>
struct int_type_of_bit_size
{
    typedef int_type<number_of_bits_of_fundamental<T>::value> type;
};

} // namespace tmp

#endif // TMP_PREDICATES_H
