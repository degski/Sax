
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

#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <utility>

#include <iostream>

// STL-like-type functions and classes.

namespace sax {

// For use with a std::variant (or drop-in) and std::visit using lambda's as
// per the example #4 https://en.cppreference.com/w/cpp/utility/variant/visit
template<typename ... Ts>
struct overloaded : Ts... {
    using Ts::operator ( ) ...;
};
template<typename ... Ts>
overloaded ( Ts ... )->overloaded<Ts ...>;


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
F for_each_arg ( F f, Args &&...args ) {
    ( f ( std::forward<Args> ( args ) ), ... );
    return f;
}

}
