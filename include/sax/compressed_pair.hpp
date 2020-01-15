
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

#include <tuple>
#include <type_traits>


// Code from: https://www.reddit.com/r/cpp/comments/77gxna/tight_pair_a_compressed_pair_for_c17/dom0rhr/
//       and: https://godbolt.org/g/j8nkuD

namespace sax {

template<typename T, typename U>
struct compressed_pair : std::tuple<T, U> {

  using std::tuple<T, U>::tuple;

  [[ nodiscard ]] constexpr decltype ( auto ) first ( ) noexcept {
    return std::get<0>( * this );
  }

  [[ nodiscard ]] constexpr decltype ( auto ) first ( ) const noexcept {
    return std::get<0>( * this );
  }

  [[ nodiscard ]] constexpr decltype ( auto ) second ( ) noexcept {
    return std::get<1>( * this );
  }

  [[ nodiscard ]] constexpr decltype ( auto ) second ( ) const noexcept {
    return std::get<1>( * this );
  }
};

}


// Deduction guide for C++17.
template<typename T, typename U>
sax::compressed_pair(T, U) -> sax::compressed_pair<T, U>;


// Structured bindings stuff.
namespace std {

template <typename T, typename U>
struct tuple_size<sax::compressed_pair<T, U>>
  : integral_constant<size_t, 2> { };

template <size_t I, typename T, typename U>
struct tuple_element<I, sax::compressed_pair<T, U>>
  : tuple_element<I, tuple<T, U>> { };

}
