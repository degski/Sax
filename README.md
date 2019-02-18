
# Sax (C++17)

Sax, acronymed loosely after *Swiss army x (something)* [could be a knife, a sword, a machete, a gun or a bazooka, i.e. to speak with Bjarne: "it might shoot your leg off"], which is short and distinctive and allows for the namespace `sax`. Sax is a collection of small useful (in my mind) libraries I've either written or collected over time, and can all be used independently. 

All libraries are within the `sax` namespace, also those which were not contained in a namespace initially. An example of the latter is Howard Hinnant's `short_alloc`, hence `sax::short_alloc`, but otherwise vanilla. The convenience header `iostream.hpp` is an exception to this rule. `iostream.hpp` allows for some type-safe short-cuts in conjunction with `std::cout` or `std::wcout` and friends and pulls in `<iostream>`.

No dependencies other than your local STL.

**Disclaimer: not all libraries are written by me and/or might contain bits of code lifted of stackoverflow.com, reddit.com/r/cpp, github.com and maybe other places.**


## Individual libraries

### string_split.hpp

A string splitter allowing for strings to be used as delimiters.

Just pass in a STL-string, followed by a number of delimiters (string-literals). **The order of the passed-in delimiters is important as the delimiters are applied left to right on the (sub-strings) of the previous parsing pass.**


    std::string s { "Cheech and Chong" };
    auto vector = sax::string_split ( s, "and" );

returns a vector of strings "Cheech" and "Chong".

    Leading and trailing white-space is trimmed by default.

    std::string s { " a,      bc d and e " };
    auto vector = sax::string_split ( s, "and", ",", " " );

returns a vector of strings "a", "bc", "d" and "e".

To deal with tabs, pass in a "\t" as a delimiter.

Lines in a csv-file are easily parsed with the combo of 
`",", " ", "\t"` as delimiters, which will parse most 
csv-files out of the box. 

    std::string s { "Cheech and, Chong" };
    auto vector = sax::string_split ( s, "and" );

returns a vector of strings "Cheech" and ", Chong".

    std::string s { "Cheech and, Chong" };
    auto vector = sax::string_split ( s, "and", "," );

returns a vector of strings "Cheech" and "Chong".
    
    std::string s { "Cheech and, Chong" };
    auto vector = sax::string_split ( s, " and ", "," );
    
returns a vector of strings "Cheech and" and "Chong".


### uniform_int_distribution.hpp

C++17-compliant (much) faster drop-in replacement for [`std::uniform_int_distribution`](https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution), i.e. [`sax::uniform_int_distribution`](https://github.com/degski/uniform_int_distribution_fast), based on the bounded_rand-function, as per the [paper by Daniel Lemire](https://arxiv.org/abs/1805.10941) and optimizations added to bounded_rand [published by Melissa E. O'Neill](http://www.pcg-random.org/posts/bounded-rands.html).


### statistics.hpp

Returns, in one pass through the data [i.e. sort of efficiently] - using [Wellford's method](https://www.johndcook.com/blog/standard_deviation/), the minimum, maximum, mean, variance, sample standard deviation and population standard deviation of the data in a std::tuple.

    template<typename T>
    [[ nodiscard ]] std::tuple<T, T, T, T, T, T> statistics ( T const * const data, const std::size_t n ) noexcept;

`statistics` is purposely fitted with a C-interface, as to allow for maximum flexibility.


### shift_rotate_avx2.hpp

Implementation of lane-crossing rotates and shifts in AVX2.

    [[ nodiscard ]] inline __m256i _mm256_sli_si256 ( __m256i a, int n ) noexcept;
    [[ nodiscard ]] inline __m256i _mm256_sri_si256 ( __m256i a, int n ) noexcept;
    [[ nodiscard ]] inline __m256i _mm256_rli_si256 ( __m256i a, int n ) noexcept;
    [[ nodiscard ]] inline __m256i _mm256_rri_si256 ( __m256i a, int n ) noexcept;


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
    [[ nodiscard ]] T median ( const Container & container_ );


A pair<> that is is_trivially_copyable and therefor faster than std::pair<>.
This was discussed on [reddit](https://www.reddit.com/r/cpp/comments/ar4ghs/stdpair_disappointing_performance/).

    template<typename KeyType, typename ValueType>
    struct pair;


## License

MIT, unless the license in the individual file states differently. The library contains no (L)GPL'ed code.
