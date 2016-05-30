#ifndef SORTED_ARRAY
#define SORTED_ARRAY

#include <utility>
#include "key_value_pair.h"

namespace alg
{
namespace ds
{

template <typename V, std::size_t N>
class sorted_array
{
private:
    typedef std::size_t idx_type;
    V data[N];
    idx_type n;

    inline void make_space(idx_type idx)
    {
        for(idx_type i = n; i != idx; --i)
        {
            data[i+1] = std::move(data[i]);
        }
    }
    inline void remove_space(idx_type idx)
    {
        for(idx_type i = idx; i != n; ++i)
        {
            data[i] = std::move(data[i+1]);
        }
    }

    inline idx_type find(const V& v, idx_type low, idx_type high) const
    {
        idx_type m = (low+high)/2;
        if(low==high)
        {
            return low;
        }
        else if(data[m] < v)
        {
            return find(v,m+1,high);
        }
        else if(data[m] > v)
        {
            return find(v,low,m);
        }
    }

public:
    sorted_array() : n(0)
    {

    }
    ~sorted_array(){}

    idx_type size() const { return n; }

    V get_element(idx_type idx) const
    {
        return data[idx];
    }
    void insert(const V& v)
    {
        idx_type idx = find(v,0,n);
        make_space(idx);
        ++n;
        data[idx] = v;
    }
    void delete_element(idx_type idx)
    {
        remove_space(idx);
        --n;
    }
    void move_element(idx_type idx, const V& newvalue)
    {
        data[idx] = newvalue;

        while(idx > 0 && data[idx] < data[idx-1])
        {
            std::swap(data[idx-1],data[idx]);
            --idx;
        }
        while(idx < n-1 && data[idx] > data[idx+1])
        {
            std::swap(data[idx],data[idx+1]);
            ++idx;
        }
    }

};

template <typename K, typename V, std::size_t N>
using kv_sorted_array = sorted_array<detail::key_value_pair<K,V>,N>;

} /* ds */
} /* alg */

#endif /* SORTED_ARRAY */
