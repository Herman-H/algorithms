#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE

#include <utility>
#include "key_value_pair.h"

namespace alg
{
namespace ds
{
namespace detail
{

template <typename V, std::size_t N>
class binary_heap
{
private:
    typedef std::size_t idx_type;
    V data[N+1]; // First element always unused
    idx_type n;
    inline idx_type l_child(idx_type p) const
    {
        return p*2;
    }
    inline idx_type r_child(idx_type p) const
    {
        return p*2+1;
    }
    inline idx_type parent(idx_type c) const
    {
        return c/2;
    }
    inline bool is_not_heap_up(idx_type c) const
    {
        return c != 1 && data[c] < data[parent(c)];
    }
    inline idx_type heap_down_idx(idx_type p) const
    {
        idx_type a = (l_child(p) <= n && data[l_child(p)] < data[p] ? l_child(p) : p);
        return (r_child(p) <= n && data[r_child(p)] < data[a] ? r_child(p) : a);
    }
    void heapify_up(idx_type c)
    {
        while(is_not_heap_up(c))
        {
            idx_type p = parent(c);
            std::swap(data[c], data[p]);
            c = p;
        }
    }
    void heapify_down(idx_type p)
    {
        idx_type idx = heap_down_idx(p);
        while(idx != p)
        {
            std::swap(data[p],data[idx]);
            p = idx;
            idx = heap_down_idx(p);
        }
    }

public:
    binary_heap() : n(0) {}
    ~binary_heap() {}

    void insert(const V & v)
    {
        n++;
        data[n] = v;
        heapify_up(n);
    }
    void insert(const V && v)
    {
        n++;
        data[n] = std::move(v);
        heapify_up(n);
    }

    void delete_element(idx_type idx)
    {
        data[idx] = data[n];
        --n;
        heapify_down(idx);
    }

    V get_element(idx_type idx) const
    {
        return data[idx];
    }

    idx_type size() const
    {
        return n;
    }
};

} /* detail */

template <typename V, std::size_t N>
class priority_queue
{
private:
    detail::binary_heap<V,N> heap;
public:
    priority_queue() : heap()
    {

    }
    ~priority_queue(){}

    void insert(const V& v)
    {
        heap.insert(v);
    }
    void insert(const V&& v)
    {
        heap.insert(std::move(v));
    }

    V element() const
    {
        return heap.get_element(1);
    }
    void decrement()
    {
        heap.delete_element(1);
    }
    bool is_empty() const
    {
        return heap.size() == 0;
    }
};

template <typename K, typename V, std::size_t N>
using kv_priority_queue = priority_queue<detail::key_value_pair<K,V>,N>;

} /* ds */
} /* alg */

#endif /* PRIORITY_QUEUE */
