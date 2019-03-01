
# Sax (C++17)

Sax, acronymed loosely after *Swiss army x (something)* [could be a knife, a sword, a machete, a gun or a bazooka, i.e. to speak with Bjarne: "it might shoot your leg off"], which is short and distinctive and allows for the namespace `sax`. Sax is a collection of small useful (in my mind) libraries I've either written or collected over time, and can all be used independently. 

All libraries are within the `sax` namespace, also those which were not contained in a namespace initially. An example of the latter is Howard Hinnant's `short_alloc`, hence `sax::short_alloc`, but otherwise vanilla. The convenience header `iostream.hpp` is an exception to this rule. `iostream.hpp` allows for some type-safe short-cuts in conjunction with `std::cout` or `std::wcout` and friends and pulls in `<iostream>`.

No dependencies other than your local STL.

**Disclaimer: not all libraries are written by me and/or might contain bits of code lifted of stackoverflow.com, reddit.com/r/cpp, github.com and maybe other places.**


## Individual libraries

### shift_rotate_avx2.hpp

Implementation of lane-crossing rotates and shifts in AVX2.

    [[ nodiscard ]] inline __m256i _mm256_sli_si256 ( __m256i a, int n ) noexcept;
    [[ nodiscard ]] inline __m256i _mm256_sri_si256 ( __m256i a, int n ) noexcept;
    [[ nodiscard ]] inline __m256i _mm256_rli_si256 ( __m256i a, int n ) noexcept;
    [[ nodiscard ]] inline __m256i _mm256_rri_si256 ( __m256i a, int n ) noexcept;


### statistics.hpp

Returns, in one pass through the data [i.e. std::sort of efficiently] - using [Wellford's method](https://www.johndcook.com/blog/standard_deviation/), the minimum, maximum, mean, variance, sample standard deviation and population standard deviation of the data in a std::tuple.

    template<typename T>
    [[ nodiscard ]] std::tuple<T, T, T, T, T, T> statistics ( T const * const data, const std::size_t n ) noexcept;

`statistics` is purposely fitted with a C-interface, as to allow for maximum flexibility.


### stl.hpp

For use with a std::variant (or drop-in) and std::visit using lambda's as
per the [example #4](https://en.cppreference.com/w/cpp/utility/variant/visit)

    template<typename ... Ts>
    struct overloaded;


A back_emplacer, like std::back_inserter, but emplacing.

    template<typename Container>
    [[ nodiscard ]] back_emplace_iterator<Container> back_emplacer ( Container & c );


Free function calculating the median of a container.

    template<typename Container, typename T = typename Container::value_type, typename Comp = std::less<T>>
    [[ nodiscard ]] T median ( const Container & container_, Comp comp_ = std::less<T> ( ) );


A pair<> that is is_trivially_copyable and therefor faster than std::pair<>.
This was discussed on [reddit](https://www.reddit.com/r/cpp/comments/ar4ghs/stdpair_disappointing_performance/).

    template<typename KeyType, typename ValueType>
    struct pair;


### string_split.hpp

A string splitter allowing for strings to be used as delimiters.

    template<typename CharT, typename ... Delimiters>
    [[ nodiscard ]] std::vector<std::basic_string_view<CharT>> string_split ( const std::basic_string<CharT> & string_, Delimiters ... delimiters_ );


Just pass in a STL-string, followed by a number of delimiters (string-literals or characters).


    std::string s { "Cheech and Chong" };
    auto vector = sax::string_split ( s, "and" );

returns a vector of string_views "Cheech" and "Chong", this implies the passed-in string should be ket alive and unmodified.

To deal with tabs, pass in a "\t" as a delimiter.

Lines in a csv-file are easily parsed with the combo of 
`",", " ", "\t"` as delimiters, which will parse most 
csv-files out of the box. 

    std::string s { "Cheech and, Chong" };
    auto vector = sax::string_split ( s, "and" );

returns a vector of string_views "Cheech" and ", Chong".

    std::string s { "Cheech and, Chong" };
    auto vector = sax::string_split ( s, "and", "," );

returns a vector of string_views "Cheech" and "Chong".
    
    std::string s { "Cheech and, Chong" };
    auto vector = sax::string_split ( s, " and ", "," );
    
returns a vector of string_views "Cheech and" and "Chong".


### uniform_int_distribution.hpp

C++17-compliant (much) faster drop-in replacement for [`std::uniform_int_distribution`](https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution), i.e. [`sax::uniform_int_distribution`](https://github.com/degski/uniform_int_distribution_fast), based on the bounded_rand-function, as per the [paper by Daniel Lemire](https://arxiv.org/abs/1805.10941) and optimizations added to bounded_rand [published by Melissa E. O'Neill](http://www.pcg-random.org/posts/bounded-rands.html).


### zip.hpp

A header only implementation of an iterator zipper made in C++17. `zip.hpp` is just a slight mod of [`ZipIter`(C++14)](https://github.com/matheuspf/ZipIter).

You can iterate and use stl algorithms on multiple iterators at the same time easily with no runtime overhead (using -O3 optimization flag).

#### Examples


```c++

#include <algorithm>
#include <array>
#include <numeric>
#include <sax/iostream.hpp>
#include <vector>


#include <sax/zip.hpp>  // The header file


int main ()
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::array<double, 5> u = {5, 4, 3, 2, 1};



    // Iterating through both containers using std::for_each
    std::for_each(sax::zip_begin(v, u), sax::zip_end(v, u), [](auto tup){
        std::cout << std::get<0>(tup) << "     " << std::get<1>(tup) << nl;
    });



    // Using the sax::unzip to unpack those values.
    // They can be taken as references too
    std::for_each(sax::zip_begin(v, u), sax::zip_end(v, u), sax::unzip([](int x, double y){
        std::cout << x << "     " << y << nl;
    }));



    // Using for range -- The return is a tuple containing references
    for(auto tup : sax::zip(v, u)) sax::unzip(tup, [](int x, double y){
        std::cout << x << "     " << y << nl;
    });



    // Or using a function that encapsulates the above.
    // The lambda comes after the variadic arguments 
    sax::zip_for_each(v, u, [](int x, double y){
        std::cout << x << "     " << y << nl;
    });



    // Sorting both containers using the std::tuple operator <
    std::sort(sax::zipit(v.begin(), u.begin()), sax::zipit(v.end(), u.end()));


    // or
    std::sort(sax::zip_begin(v, u), sax::zip_end(v, u));


    // or even using a macro that does exactly the same as above
    std::sort(ZIP_ALL(v, u));


    // using a custom comparison
    std::sort(ZIP_ALL(v, u), [](auto tup1, auto tup2){
        return std::get<0>(tup1) + std::get<1>(tup1) < std::get<0>(tup2) + std::get<1>(tup2);
    });


    // or using the sax::unzip to magically unpack those tuples
    std::sort(ZIP_ALL(v, u), sax::unzip([](int v1, double u1, int v2, double u2){
        return v1 + u1 < v2 + u2;
    }));



    // It is really that easy
    std::transform(ZIP_ALL(v, u), sax::zip_begin(v, u), sax::unzip([](int x, double y){
        return std::make_tuple(0, 0.0);
    }));


    std::reverse(ZIP_ALL(v, u));


    auto sum = std::accumulate(ZIP_ALL(v, u), 0.0, sax::unzip([](double sum, int x, double y){
        return sum + x + y;
    }));

    std::cout << sum << nl;


    return EXIT_SUCCESS;
}
```
<br>


## License

MIT, unless the license in the individual file states differently. The library contains no (L)GPL'ed code.
