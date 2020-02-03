
// MIT License
//
// Copyright (c) 2019, 2020 degski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#ifdef _WIN32
#    include <cstring>
#endif
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <algorithm>
#include <future>
#include <iterator>
#include <type_traits>
#include <utility>
#include <variant>

#include <iostream>

// STL-like-type functions and classes.

namespace sax {

namespace detail {
// For use with a std::variant (or drop-in) and std::visit using lambda's as
// per the example #4 https://en.cppreference.com/w/cpp/utility/variant/visit
template<typename... Ts>
struct overloaded : Ts... {
    using Ts::operator( )...;
};
template<typename... Ts>
overloaded ( Ts... ) -> overloaded<Ts...>;
} // namespace detail.

// Applying the above overload.
template<typename Variant, typename... Matchers>
auto match ( Variant && variant, Matchers &&... matchers ) {
    return std::visit ( detail::overloaded{ std::forward<Matchers> ( matchers )... }, std::forward<Variant> ( variant ) );
}

template<typename Container>
class back_emplace_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void> {

    public:
    using container_type = Container;
    using value_type     = typename Container::value_type;

    protected:
    Container * m_container;

    public:
    explicit back_emplace_iterator ( Container & x ) noexcept : m_container ( &x ) {}

    template<typename... Args>
    [[nodiscard]] back_emplace_iterator & operator= ( Args &&... args ) {
        static_assert ( std::is_constructible_v<typename Container::value_type, Args...>, "should be constructible" );
        assert ( m_container );
        m_container->emplace_back ( std::forward<Args> ( args )... );
        return *this;
    }

    [[maybe_unused]] back_emplace_iterator & operator* ( ) { return *this; }
    [[maybe_unused]] back_emplace_iterator & operator++ ( ) { return *this; }
    [[maybe_unused]] back_emplace_iterator & operator++ ( int ) { return *this; }
};

// A back_emplacer, like std::back_inserter, but emplacing.
template<typename Container>
[[nodiscard]] back_emplace_iterator<Container> back_emplacer ( Container & c ) {
    return back_emplace_iterator<Container> ( c );
}

template<typename Container, typename T = typename Container::value_type, typename Comp = std::less<T>>
[[nodiscard]] T median ( const Container & container_, Comp comp_ = std::less<T> ( ) ) {
    Container copy{ container_ };
    auto median = std::next ( std::begin ( copy ), copy.size ( ) / 2 );
    std::nth_element ( std::begin ( copy ), median, std::end ( copy ), comp_ );
    return *median;
}

// A pair<> that is is_trivially_copyable and therefor faster than std::pair<>.
// https://www.reddit.com/r/cpp/comments/ar4ghs/stdpair_disappointing_performance/

template<typename KeyType, typename ValueType>
struct pair {
    using first_type  = KeyType;
    using second_type = ValueType;
    pair ( )          = default;
    template<typename T1, typename T2>
    pair ( T1 && key, T2 && value ) : first ( std::forward<T1> ( key ) ), second ( std::forward<T2> ( value ) ) {}
    KeyType first;
    ValueType second;
};

// https://www.fluentcpp.com/2019/03/05/for_each_arg-applying-a-function-to-each-argument-of-a-function-in-cpp/

template<typename F, typename... Args>
F for_each_arg ( F f, Args &&... args ) {
    ( f ( std::forward<Args> ( args ) ), ... );
    return f;
}

namespace details {
template<class>
struct is_ref_wrapper : std::false_type {};
template<class T>
struct is_ref_wrapper<std::reference_wrapper<T>> : std::true_type {};

template<class T>
using not_ref_wrapper = std::negation<is_ref_wrapper<std::decay_t<T>>>;

template<class D, class...>
struct return_type_helper {
    using type = D;
};
template<class... Types>
struct return_type_helper<void, Types...> : std::common_type<Types...> {
    static_assert ( std::conjunction_v<not_ref_wrapper<Types>...>, "Types cannot contain reference_wrappers when D is void" );
};

template<class D, class... Types>
using return_type = std::array<typename return_type_helper<D, Types...>::type, sizeof...( Types )>;
} // namespace details

template<class D = void, class... Types>
constexpr details::return_type<D, Types...> make_array ( Types &&... t ) {
    return { std::forward<Types> ( t )... };
}

template<typename CharT, typename Traits, typename Allocator>
void replace_all ( std::basic_string<CharT, Traits, Allocator> & input_, std::basic_string<CharT, Traits, Allocator> const & from_,
                   std::basic_string<CharT, Traits, Allocator> const & to_ ) noexcept {
    std::size_t pos = 0u;
    while ( std::basic_string<CharT, Traits, Allocator>::npos != ( pos = input_.find ( from_, pos ) ) ) {
        input_.replace ( pos, from_.length ( ), to_ );
        pos += to_.length ( );
    }
}

#ifdef _WIN32
[[maybe_unused]] inline void * memccpy ( void * dest, void const * src, int c, std::size_t count ) noexcept {
    return _memccpy ( dest, src, c, count );
}
#endif

// Pointer alignment, assumes C++20 two's-complement.
[[nodiscard]] int pointer_alignment ( void const * ptr_ ) noexcept {
    return static_cast<int> ( reinterpret_cast<std::uintptr_t> ( ptr_ ) &
                              static_cast<std::uintptr_t> ( -reinterpret_cast<std::intptr_t> ( ptr_ ) ) );
}

// https://stackoverflow.com/a/18405291/646940

// My work around when I have a container that needs to know if the contained
// object can be safely copied, comparedand ordered is to specialize for
// std::vector on a custom traits class, and fall back on the value of the
// custom traits class on the contained type.This is a patchwork solution, and
// quite intrusive. And similar for < and ==. I can add more specializations
// when I run into more container-types that should really forward their
// properties down to their data, or I could write a fancier SFINAE container-
// test and traits and extract the underlying value-type and dispatch the
// question to the test on the value-type.

/*
    template<template<typename>class test, typename T>
    struct smart_test : test<T> {};
    template<template<typename>class test, typename T, typename A>
    struct smart_test<test, std::vector<T, A>> : smart_test<T> {}

    template<typename T>
    using smart_is_copy_constructible = smart_test<std::is_copy_constructible, T >;
*/

// https://stackoverflow.com/a/481333/646940

template<typename T>
inline T median_of_five ( T * p ) noexcept {
    if ( p[ 0 ] > p[ 1 ] )
        std::swap ( p[ 0 ], p[ 1 ] );
    if ( p[ 2 ] > p[ 3 ] )
        std::swap ( p[ 2 ], p[ 3 ] );
    if ( p[ 2 ] < p[ 0 ] ) {
        std::swap ( p[ 1 ], p[ 3 ] );
        p[ 2 ] = p[ 0 ];
    }
    p[ 0 ] = p[ 4 ];
    if ( p[ 0 ] > p[ 1 ] )
        std::swap ( p[ 0 ], p[ 1 ] );
    if ( p[ 0 ] < p[ 2 ] ) {
        std::swap ( p[ 1 ], p[ 3 ] );
        p[ 0 ] = p[ 2 ];
    }
    return std::min ( p[ 3 ], p[ 0 ] );
}

// https://stackoverflow.com/a/2117018/646940

template<typename T>
inline T median_of_five2 ( T * p ) noexcept {
    return p[ 1 ] < p[ 0 ]
               ? p[ 3 ] < p[ 2 ]
                     ? p[ 1 ] < p[ 3 ]
                           ? p[ 0 ] < p[ 4 ]
                                 ? p[ 0 ] < p[ 3 ] ? p[ 4 ] < p[ 3 ] ? p[ 4 ] : p[ 3 ] : p[ 2 ] < p[ 0 ] ? p[ 2 ] : p[ 0 ]
                                 : p[ 4 ] < p[ 3 ] ? p[ 0 ] < p[ 3 ] ? p[ 0 ] : p[ 3 ] : p[ 2 ] < p[ 4 ] ? p[ 2 ] : p[ 4 ]
                           : p[ 2 ] < p[ 4 ]
                                 ? p[ 1 ] < p[ 2 ] ? p[ 0 ] < p[ 2 ] ? p[ 0 ] : p[ 2 ] : p[ 4 ] < p[ 1 ] ? p[ 4 ] : p[ 1 ]
                                 : p[ 1 ] < p[ 4 ] ? p[ 0 ] < p[ 4 ] ? p[ 0 ] : p[ 4 ] : p[ 2 ] < p[ 1 ] ? p[ 2 ] : p[ 1 ]
                     : p[ 1 ] < p[ 2 ]
                           ? p[ 0 ] < p[ 4 ]
                                 ? p[ 0 ] < p[ 2 ] ? p[ 4 ] < p[ 2 ] ? p[ 4 ] : p[ 2 ] : p[ 3 ] < p[ 0 ] ? p[ 3 ] : p[ 0 ]
                                 : p[ 4 ] < p[ 2 ] ? p[ 0 ] < p[ 2 ] ? p[ 0 ] : p[ 2 ] : p[ 3 ] < p[ 4 ] ? p[ 3 ] : p[ 4 ]
                           : p[ 3 ] < p[ 4 ]
                                 ? p[ 1 ] < p[ 3 ] ? p[ 0 ] < p[ 3 ] ? p[ 0 ] : p[ 3 ] : p[ 4 ] < p[ 1 ] ? p[ 4 ] : p[ 1 ]
                                 : p[ 1 ] < p[ 4 ] ? p[ 0 ] < p[ 4 ] ? p[ 0 ] : p[ 4 ] : p[ 3 ] < p[ 1 ] ? p[ 3 ] : p[ 1 ]
               : p[ 3 ] < p[ 2 ]
                     ? p[ 0 ] < p[ 3 ]
                           ? p[ 1 ] < p[ 4 ]
                                 ? p[ 1 ] < p[ 3 ] ? p[ 4 ] < p[ 3 ] ? p[ 4 ] : p[ 3 ] : p[ 2 ] < p[ 1 ] ? p[ 2 ] : p[ 1 ]
                                 : p[ 4 ] < p[ 3 ] ? p[ 1 ] < p[ 3 ] ? p[ 1 ] : p[ 3 ] : p[ 2 ] < p[ 4 ] ? p[ 2 ] : p[ 4 ]
                           : p[ 2 ] < p[ 4 ]
                                 ? p[ 0 ] < p[ 2 ] ? p[ 1 ] < p[ 2 ] ? p[ 1 ] : p[ 2 ] : p[ 4 ] < p[ 0 ] ? p[ 4 ] : p[ 0 ]
                                 : p[ 0 ] < p[ 4 ] ? p[ 1 ] < p[ 4 ] ? p[ 1 ] : p[ 4 ] : p[ 2 ] < p[ 0 ] ? p[ 2 ] : p[ 0 ]
                     : p[ 0 ] < p[ 2 ]
                           ? p[ 1 ] < p[ 4 ]
                                 ? p[ 1 ] < p[ 2 ] ? p[ 4 ] < p[ 2 ] ? p[ 4 ] : p[ 2 ] : p[ 3 ] < p[ 1 ] ? p[ 3 ] : p[ 1 ]
                                 : p[ 4 ] < p[ 2 ] ? p[ 1 ] < p[ 2 ] ? p[ 1 ] : p[ 2 ] : p[ 3 ] < p[ 4 ] ? p[ 3 ] : p[ 4 ]
                           : p[ 3 ] < p[ 4 ] ? p[ 0 ] < p[ 3 ] ? p[ 1 ] < p[ 3 ] ? p[ 1 ] : p[ 3 ]
                                                               : p[ 4 ] < p[ 0 ] ? p[ 4 ] : p[ 0 ]
                                             : p[ 0 ] < p[ 4 ] ? p[ 1 ] < p[ 4 ] ? p[ 1 ] : p[ 4 ]
                                                               : p[ 3 ] < p[ 0 ] ? p[ 3 ] : p[ 0 ];
}

} // namespace sax

#include <immintrin.h>

namespace sax {
// dst and src must be 32-byte aligned.
// size must be multiple of 32*2 = 64 bytes.
inline void memcpy_avx ( void * dst, void const * src, size_t size ) noexcept {
    // https://hero.handmade.network/forums/code-discussion/t/157-memory_bandwidth_+_implementing_memcpy
    constexpr size_t stride = 2 * sizeof ( __m256i );
    while ( size ) {
        __m256i a = _mm256_load_si256 ( ( __m256i * ) src + 0 );
        __m256i b = _mm256_load_si256 ( ( __m256i * ) src + 1 );
        _mm256_stream_si256 ( ( __m256i * ) dst + 0, a );
        _mm256_stream_si256 ( ( __m256i * ) dst + 1, b );
        size -= stride;
        src = reinterpret_cast<uint8_t const *> ( src ) + stride;
        dst = reinterpret_cast<uint8_t *> ( dst ) + stride;
    }
}
// dst and src must be 16-byte aligned
// size must be multiple of 16*2 = 32 bytes
inline void memcpy_sse ( void * dst, void const * src, size_t size ) noexcept {
    size_t stride = 2 * sizeof ( __m128 );
    while ( size ) {
        __m128 a = _mm_load_ps ( ( float * ) ( reinterpret_cast<uint8_t const *> ( src ) + 0 * sizeof ( __m128 ) ) );
        __m128 b = _mm_load_ps ( ( float * ) ( reinterpret_cast<uint8_t const *> ( src ) + 1 * sizeof ( __m128 ) ) );
        _mm_stream_ps ( ( float * ) ( reinterpret_cast<uint8_t *> ( dst ) + 0 * sizeof ( __m128 ) ), a );
        _mm_stream_ps ( ( float * ) ( reinterpret_cast<uint8_t *> ( dst ) + 1 * sizeof ( __m128 ) ), b );
        size -= stride;
        src = reinterpret_cast<uint8_t const *> ( src ) + stride;
        dst = reinterpret_cast<uint8_t *> ( dst ) + stride;
    }
}
} // namespace sax

template<typename Stream, typename T, typename = std::enable_if_t<std::is_pointer<T>::value>>
Stream & operator<< ( Stream & out_, T const & n_ ) noexcept {
    std::int8_t n[ 8 ];
    std::memcpy ( &n, &n_, sizeof ( T ) );
    std::swap ( n[ 0 ], n[ 6 ] );
    std::swap ( n[ 1 ], n[ 7 ] );
    std::swap ( n[ 2 ], n[ 4 ] );
    std::swap ( n[ 3 ], n[ 5 ] );
    out_ << "0x" << std::setfill ( '0' ) << std::setw ( 4 ) << std::hex << std::uppercase
         << ( std::uint32_t ) ( ( std::uint16_t * ) n )[ 0 ];
    for ( int i = 1; i < 4; ++i )
        out_ << '\'' << std::setw ( 4 ) << ( std::uint32_t ) ( ( std::uint16_t * ) n )[ i ];
    out_ << std::setfill ( ' ' ) << std::setw ( 0 ) << std::dec << std::nouppercase << lf;
    return out_;
}
