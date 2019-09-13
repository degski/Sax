
// MIT License
//
// Copyright (c) 2019 degski
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
#include <cstring>
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
template<typename ... Ts>
struct overloaded : Ts... {
    using Ts::operator ( ) ...;
};
template<typename ... Ts>
overloaded ( Ts ... )->overloaded<Ts ...>;
} // namespace detail.


// Applying the above overload.
template<typename Variant, typename ... Matchers>
auto match ( Variant && variant, Matchers && ... matchers ) {
  return std::visit ( detail::overloaded { std::forward<Matchers> ( matchers) ... }, std::forward<Variant> ( variant ) );
}


template<typename Container>
class back_emplace_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void> {

    public:

    using container_type = Container;
    using value_type = typename Container::value_type;

    protected:

    Container * m_container;

    public:

    explicit back_emplace_iterator ( Container & x ) noexcept : m_container ( & x ) { }

    template<typename ... Args>
    [[ nodiscard ]] back_emplace_iterator & operator = ( Args && ... args ) {
        static_assert ( std::is_constructible_v<typename Container::value_type, Args ... >, "should be constructible" );
        assert ( m_container );
        m_container->emplace_back ( std::forward<Args> ( args ) ... );
        return *this;
    }

    [[ maybe_unused ]] back_emplace_iterator & operator * ( ) { return * this; }
    [[ maybe_unused ]] back_emplace_iterator & operator ++ ( ) { return * this; }
    [[ maybe_unused ]] back_emplace_iterator & operator ++ ( int ) { return * this; }
};

// A back_emplacer, like std::back_inserter, but emplacing.
template<typename Container>
[[ nodiscard ]] back_emplace_iterator<Container> back_emplacer ( Container & c ) {
    return back_emplace_iterator<Container> ( c );
}


template<typename Container, typename T = typename Container::value_type, typename Comp = std::less<T>>
[[ nodiscard ]] T median ( const Container & container_, Comp comp_ = std::less<T> ( ) ) {
    Container copy { container_ };
    auto median = std::next ( std::begin ( copy ), copy.size ( ) / 2 );
    std::nth_element ( std::begin ( copy ), median, std::end ( copy ), comp_ );
    return *median;
}


// A pair<> that is is_trivially_copyable and therefor faster than std::pair<>.
// https://www.reddit.com/r/cpp/comments/ar4ghs/stdpair_disappointing_performance/

template<typename KeyType, typename ValueType>
struct pair {
    using first_type = KeyType;
    using second_type = ValueType;
    pair ( ) = default;
    template<typename T1, typename T2>
    pair ( T1 && key, T2 && value ) :
        first ( std::forward<T1> ( key ) ),
        second ( std::forward<T2> ( value ) ) {
    }
    KeyType first;
    ValueType second;
};

// https://www.fluentcpp.com/2019/03/05/for_each_arg-applying-a-function-to-each-argument-of-a-function-in-cpp/

template<typename F, typename ... Args>
F for_each_arg ( F f, Args && ... args ) {
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
void replace_all ( std::basic_string<CharT, Traits, Allocator> & input_, std::basic_string<CharT, Traits, Allocator> const & from_, std::basic_string<CharT, Traits, Allocator> const & to_ ) noexcept {
    std::size_t pos = 0u;
    while ( std::basic_string<CharT, Traits, Allocator>::npos != ( pos = input_.find ( from_, pos ) ) ) {
        input_.replace ( pos, from_.length ( ), to_ );
        pos += to_.length ( );
    }
}

#ifdef _WIN32
[[maybe_unused]] inline void * memccpy ( void * dest, void const *src, int c, std::size_t count) noexcept {
    return _memccpy (dest, src, c, count );
}
#endif

}
