
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

#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <random>


#if UINTPTR_MAX == 0xFFFF'FFFF'FFFF'FFFF
#if defined ( __clang__ ) or defined ( __GNUC__ )
#include "lehmer.hpp"
#else
#include "splitmix.hpp"
#endif
#elif UINTPTR_MAX == 0xFFFF'FFFF
#include "jsf.hpp"
#else
#error funny pointers detected
#endif


namespace sax {

#if UINTPTR_MAX == 0xFFFF'FFFF'FFFF'FFFF
#if defined ( __clang__ ) or defined ( __GNUC__ )
using uint128_t = __uint128_t;
using Rng = mcg128_fast;
[[ nodiscard ]] inline sax::uint128_t os_seed ( ) noexcept {
    std::random_device rd;
    auto rnd = [ & rd ] ( const int shift ) { return static_cast<sax::uint128_t> ( rd ( ) ) << shift; };
    return rnd ( 96 ) | rnd ( 64 ) | rnd ( 32 ) | rnd ( 0 );
}
// Returns first prime less than 2 ^ 128.
[[ nodiscard ]] inline constexpr sax::uint128_t fixed_seed ( ) noexcept {
    auto seed = [ ] ( const int shift ) { return static_cast<sax::uint128_t> ( 0xFFFF'FFFF'FFFF'FFFF ) << shift; };
    return ( seed ( 64 ) | seed ( 0 ) ) - static_cast<sax::uint128_t> ( 158 );
}
#else
using Rng = splitmix64;
[[ nodiscard ]] inline std::uint64_t os_seed ( ) noexcept {
    std::random_device rd;
    auto rnd = [ & rd ] ( const int shift ) { return static_cast<std::uint64_t> ( rd ( ) ) << shift; };
    return rnd ( 32 ) | rnd ( 0 );
}
// Returns first prime less than 2 ^ 64.
[[ nodiscard ]] inline constexpr std::uint64_t fixed_seed ( ) noexcept {
    return 18'446'744'073'709'551'557u;
}
#endif
#elif UINTPTR_MAX == 0xFFFF'FFFF
using Rng = jsf32;
[[ nodiscard ]] inline std::uint32_t os_seed ( ) noexcept {
    return std::random_device { } ( );
}
// Returns first prime less than 2 ^ 32.
[[ nodiscard ]] inline constexpr std::uint32_t fixed_seed ( ) noexcept {
    return 4'294'967'291u;
}
#else
#error funny pointers detected
#endif

} // namespace sax
