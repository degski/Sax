
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

#include <random>


#if UINTPTR_MAX == 0xFFFF'FFFF'FFFF'FFFF
#if defined ( __clang__ ) or defined ( __GNUC__ )
#include <lehmer.hpp>       // https://github.com/degski/Sax/blob/master/lehmer.hpp
#else
#include <splitmix.hpp>     // https://github.com/degski/Sax/blob/master/splitmix.hpp
#endif
#endif

#include <singleton.hpp>    // https://github.com/degski/Sax/blob/master/singleton.hpp


namespace rnd {

#if UINTPTR_MAX == 0xFFFF'FFFF'FFFF'FFFF
#if defined ( __clang__ ) or defined ( __GNUC__ )
using Rng = mcg128_fast;
[[ nodiscard ]] __uint128_t os_seed ( ) noexcept {
    std::random_device rd;
    auto rnd = [ & rd ] ( const int shift ) { return static_cast<__uint128_t> ( rd ( ) ) << shift; };
    return rnd ( 96 ) | rnd ( 64 ) | rnd ( 32 ) | rnd ( 0 );
}
#else
using Rng = splitmix64;
[[ nodiscard ]] std::uint64_t os_seed ( ) noexcept {
    std::random_device rd;
    auto rnd = [ & rd ] ( const int shift ) { return static_cast<std::uint64_t> ( rd ( ) ) << shift; };
    return rnd ( 32 ) | rnd ( 0 );
}
#endif
#elif UINTPTR_MAX == 0xFFFF'FFFF
using Rng = std::minstd_rand;
[[ nodiscard ]] std::uint32_t os_seed ( ) noexcept {
    return std::random_device { } ( );
}
#else
#error funny pointers detected
#endif

namespace detail {
singleton<Rng> rng;
auto seed_from_os = [ ] { const auto s = os_seed ( ); rng.instance ( ).seed ( s ); return s; } ( );
}

auto rng = [ ] { return detail::rng.instance ( ); } ( );

} // namespace rnd
