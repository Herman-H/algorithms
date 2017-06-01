# algorithms

## tmp
This is a collection of functions that rely on template metaprogramming that I find very useful. Some things are more experimental.

### tmp.h
Basic template metaprogramming functionality. tuple_t is an ubiquitous type "containing" other types. Use get_element_in(N, tuple_t(...)) to get N:th element in tuple_t(...). More functionality; folds, maps, set, sequence, filter.

### tmp_data.h
Underlying "overspecified" storage types. tmp::data::tuple is my own implementation of something like std::tuple. tmp::data:sized_bitfield has a neat way of specifying a bitfield and it is easy to retrieve and set fields using std::get like functions; std::data::get_field(Index)(bitfield), std::data::set_field(Index)(bitfield,data).

### tmp_types.h
Overspecified fundamental C++ types. The core of this is that all fundamental types are listed in a tuple_t, then it is possible to query these types for size and alignment, signedness, and retrieve a type matching your specification. The reason for this is to enable compile time network code protocols, i.e. if a type doesn't exist that is matching your specification on the system you compile for, then you could now be alerted of this at compile time.

### tmp_math.h
Some simple math function like sum(tuple), sum_first(N)(tuple), prime(N). I found a practical use for C++14 fold expressions in the implementation of prime(N). Previous implementation managed to get prime(N), N~=200, before running out of compile time memory, with fold expression implementation, the compiler managed N~=850 without running out of memory and doing it much faster.

### tmp_function.h
Some useful functions like forwarding the n:th argument from a variadic argument list. Some experimental functionality that builds on this; composing functions at compile time. The motivation for this was to use them for being able to implement a general divide and conquer algorithm, given an argument list, a combine function, and a split function and the number of splits on each level. Still work in progress (check alg/divide_and_conquer.h).

### tmp_rational.h
A rational number type. Skirting on the usage of expression template techniques. The motivation here is that N/1 * 1/M dont need any multiplication at all. You simply move each variable N and M to correct places so we get N/M directly. This way you can always get the minimum amount of needed operations for each operation. Some other examples of rational numbers that need less operations that standard rational operations are 1/N * 1/M = 1/(NM), N/1 + M/1 = (N+M)/1, 1/N / M/1 = 1/(NM), N/1 / 1/M = NM/1, etc, etc...

I have also added a "compile time factor", Sn/Sd to all rational numbers, so each number is actually (Sn/Sd) * (N/M). This allows for greater precision while sacrificing nothing when only doing multiplications and divisions. The motivation for this is to allow "compile time-rational numbers" to be multiplied with a "dynamic" rational number, without actually doing any operation at all. Of course, at some point you need to pay this cost, so you could easily switch the behaviour of the rational number type to multiply the static factors _into_ the dynamic storage for each operation. The reason I currently don't do this behaviour is that the static rational factor can be normalized after each operation for no runtime cost. Some extra operations are needed when doing additions and subtractions.

In a matrix implementation, I want it to be possible to compute inverse matrices at compile time. Since floats can't be compile time, I instead need to rely on compile time rational numbers but it must be possible to use these together with runtime variables. So this implementation is the first step in this development.

Some further additions to implement here is to define comparison operators (==, <, >, etc). Together with this I need to normalize numbers each comparison. But one way to save normalizations is to return a "carrying" type that denotes that the variable is normalized. Then, in comparisons, if one number is of normalized type, then we don't need to normalize this variable. Doing any operation that could make it "un-normalized", we make the result of this operation lose the normalized type.

## alg
Some different algorithms that I have implemented mainly as part of an algorithms course. They are not general, they mostly use special types as inputs.

## ds
Some useful datastructures that are needed in my algorithms.
