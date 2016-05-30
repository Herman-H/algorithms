#ifndef TMP_TYPE_TRAITS_H
#define TMP_TYPE_TRAITS_H

#include <type_traits>

namespace tmp
{

    template <typename T>
    struct is_xvalue
    {
        enum { value = !std::is_lvalue_reference<T>::value && std::is_rvalue_reference<T>::value };
    };

    template <typename T>
    struct is_prvalue
    {
        enum { value = !std::is_lvalue_reference<T>::value && !std::is_rvalue_reference<T>::value };
    };


} // namespace tmp


#endif // TMP_TYPES_H
