
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

#include "jsf.hpp"

namespace sax {

#if UINTPTR_MAX == 0xFFFF'FFFF'FFFF'FFFF
using Rng = jsf64;
[[ nodiscard ]] inline typename Rng::result_type os_seed ( ) noexcept {
    std::random_device rd;
    auto rnd = [ & rd ] ( const int shift ) { return static_cast<typename Rng::result_type> ( rd ( ) ) << shift; };
    return rnd ( 32 ) | rnd ( 0 );
}
// Returns first prime less than 2 ^ 64.
[[ nodiscard ]] inline constexpr typename Rng::result_type fixed_seed ( ) noexcept {
    return 18'446'744'073'709'551'557u;
}
#elif UINTPTR_MAX == 0xFFFF'FFFF
using Rng = jsf32;
[[ nodiscard ]] inline typename Rng::result_type os_seed ( ) noexcept {
    return std::random_device { } ( );
}
// Returns first prime less than 2 ^ 32.
[[ nodiscard ]] inline constexpr typename Rng::result_type fixed_seed ( ) noexcept {
    return 4'294'967'291u;
}
#else
#error funny pointers detected
#endif

} // namespace sax
