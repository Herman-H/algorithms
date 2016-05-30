#ifndef KEY_VALUE_PAIR
#define KEY_VALUE_PAIR

namespace alg
{
namespace ds
{
namespace detail
{
template <typename K, typename V>
struct key_value_pair
{
private:
    K k;
    V v;
public:
    key_value_pair(){}
    key_value_pair(const K && k, const V && v) : k(std::move(k)), v(std::move(v)) {}

    friend bool operator< (const key_value_pair & lhs, const key_value_pair & rhs)
    {
        return lhs.k < rhs.k;
    }
    V value() const
    {
        return v;
    }
};

} /* detail */
} /* ds */
} /* alg */


#endif /* KEY_VALUE_PAIR */
