
// Author: Wang Yi <godspeed_china@yeah.net>
// https://github.com/wangyi-fudan/wyhash
// degski: C++-ified stuff a bit.

// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR a_ PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>


#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <iostream>

#if UINTPTR_MAX == 0xFFFF'FFFF
#define M32 1
#define M64 0
#elif UINTPTR_MAX == 0xFFFF'FFFF'FFFF'FFFF
#define M32 0
#define M64 1
#else
#error funny pointers detected
#endif


#if defined ( _WIN32 ) && ! ( defined ( __clang__ ) || defined ( __GNUC__ ) ) // MSVC and not clang or gcc on windows.
#include <intrin.h>
#ifdef _WIN64
#pragma intrinsic ( _umul128 )
#define CONSTEXPR
#else
#define CONSTEXPR constexpr
#endif
#define GNU 0
#define MSVC 1
#define GCC 0
#define CLANG 0
#pragma warning ( push )
#pragma warning ( disable : 4244 )
#else
#define GNU 1
#define MSVC 0
#if defined ( __clang__ )
#define CLANG 1
#define GCC 0
#else
#define CLANG 0
#define GCC 1
#endif
#define CONSTEXPR constexpr
#endif


namespace sax {
namespace detail {

constexpr std::uint64_t wyp0 = 0xa0761d6478bd642full, wyp1 = 0xe7037ed1a0b428dbull, wyp2 = 0x8ebc6af09c88c6e3ull;
constexpr std::uint64_t wyp3 = 0x589965cc75374cc3ull, wyp4 = 0x1d8e4e27c47d124full, wyp5 = 0xeb44accab455d165ull;

static inline CONSTEXPR std::uint64_t wymum ( std::uint64_t a_, std::uint64_t b_ ) noexcept {
    #if GNU and M64
    __uint128_t r = a_; r *= b_; return ( r >> 64 ) ^ r;
    #elif M64
    a_ = _umul128 ( a_, b_, & b_ ); return a_ ^ b_;
    #else
    std::uint64_t ha = a_ >> 32, hb = b_ >> 32, la = ( std::uint32_t ) a_, lb = ( std::uint32_t ) b_, hi, lo;
    std::uint64_t rh = ha * hb, rm0 = ha * lb, rm1 = hb * la, rl = la * lb, t = rl + ( rm0 << 32 ), c = t < rl;
    lo = t + ( rm1 << 32 ); c += lo < t; hi = rh + ( rm0 >> 32 ) + ( rm1 >> 32 ) + c;
    return hi ^ lo;
    #endif
}

static inline std::uint64_t wyr08 ( const unsigned char *p ) noexcept { unsigned char v; std::memcpy ( &v, p, 1 ); return v; }
static inline std::uint64_t wyr16 ( const unsigned char *p ) noexcept { unsigned short v; std::memcpy ( &v, p, 2 ); return v; }
static inline std::uint64_t wyr32 ( const unsigned char *p ) noexcept { std::uint32_t v; std::memcpy ( &v, p, 4 ); return v; }
static inline std::uint64_t wyr64 ( const unsigned char *p ) noexcept { std::uint64_t v; std::memcpy ( &v, p, 8 ); return v; }

static inline std::uint64_t wyr64_2 ( const unsigned char *p ) noexcept { return ( wyr32 ( p ) << 32 ) | wyr32 ( p + 4 ); }
}

static inline std::uint64_t wyhash ( const void *key, std::uint64_t len, std::uint64_t seed ) noexcept {
    const unsigned char *p = ( const unsigned char* ) key; std::uint64_t i;
    for ( i = 0; i + 32 <= len; i += 32, p += 32 )
        seed = detail::wymum ( seed ^ detail::wyp0, detail::wymum ( detail::wyr64 ( p ) ^ detail::wyp1, detail::wyr64 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( detail::wyr64 ( p + 16 ) ^ detail::wyp3, detail::wyr64 ( p + 24 ) ^ detail::wyp4 ) );
    seed ^= detail::wyp0;
    switch ( len & 31 ) {
    case  1: seed = detail::wymum ( seed, detail::wyr08 ( p ) ^ detail::wyp1 ); break;
    case  2: seed = detail::wymum ( seed, detail::wyr16 ( p ) ^ detail::wyp1 ); break;
    case  3: seed = detail::wymum ( seed, ( ( detail::wyr16 ( p ) << 8 ) | detail::wyr08 ( p + 2 ) ) ^ detail::wyp1 ); break;
    case  4: seed = detail::wymum ( seed, detail::wyr32 ( p ) ^ detail::wyp1 ); break;
    case  5: seed = detail::wymum ( seed, ( ( detail::wyr32 ( p ) << 8 ) | detail::wyr08 ( p + 4 ) ) ^ detail::wyp1 ); break;
    case  6: seed = detail::wymum ( seed, ( ( detail::wyr32 ( p ) << 16 ) | detail::wyr16 ( p + 4 ) ) ^ detail::wyp1 ); break;
    case  7: seed = detail::wymum ( seed, ( ( detail::wyr32 ( p ) << 24 ) | ( detail::wyr16 ( p + 4 ) << 8 ) | detail::wyr08 ( p + 6 ) ) ^ detail::wyp1 ); break;
    case  8: seed = detail::wymum ( seed, detail::wyr64_2 ( p ) ^ detail::wyp1 ); break;
    case  9: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr08 ( p + 8 ) ^ detail::wyp2 ); break;
    case 10: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr16 ( p + 8 ) ^ detail::wyp2 ); break;
    case 11: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, ( ( detail::wyr16 ( p + 8 ) << 8 ) | detail::wyr08 ( p + 8 + 2 ) ) ^ detail::wyp2 ); break;
    case 12: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr32 ( p + 8 ) ^ detail::wyp2 ); break;
    case 13: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, ( ( detail::wyr32 ( p + 8 ) << 8 ) | detail::wyr08 ( p + 8 + 4 ) ) ^ detail::wyp2 ); break;
    case 14: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, ( ( detail::wyr32 ( p + 8 ) << 16 ) | detail::wyr16 ( p + 8 + 4 ) ) ^ detail::wyp2 ); break;
    case 15: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, ( ( detail::wyr32 ( p + 8 ) << 24 ) | ( detail::wyr16 ( p + 8 + 4 ) << 8 ) | detail::wyr08 ( p + 8 + 6 ) ) ^ detail::wyp2 ); break;
    case 16: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ); break;
    case 17: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( seed, detail::wyr08 ( p + 16 ) ^ detail::wyp3 ); break;
    case 18: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( seed, detail::wyr16 ( p + 16 ) ^ detail::wyp3 ); break;
    case 19: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( seed, ( ( detail::wyr16 ( p + 16 ) << 8 ) | detail::wyr08 ( p + 16 + 2 ) ) ^ detail::wyp3 ); break;
    case 20: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( seed, detail::wyr32 ( p + 16 ) ^ detail::wyp3 ); break;
    case 21: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( seed, ( ( detail::wyr32 ( p + 16 ) << 8 ) | detail::wyr08 ( p + 16 + 4 ) ) ^ detail::wyp3 ); break;
    case 22: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( seed, ( ( detail::wyr32 ( p + 16 ) << 16 ) | detail::wyr16 ( p + 16 + 4 ) ) ^ detail::wyp3 ); break;
    case 23: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( seed, ( ( detail::wyr32 ( p + 16 ) << 24 ) | ( detail::wyr16 ( p + 16 + 4 ) << 8 ) | detail::wyr08 ( p + 16 + 6 ) ) ^ detail::wyp3 ); break;
    case 24: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( seed, detail::wyr64_2 ( p + 16 ) ^ detail::wyp3 ); break;
    case 25: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( detail::wyr64_2 ( p + 16 ) ^ seed, detail::wyr08 ( p + 24 ) ^ detail::wyp4 ); break;
    case 26: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( detail::wyr64_2 ( p + 16 ) ^ seed, detail::wyr16 ( p + 24 ) ^ detail::wyp4 ); break;
    case 27: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( detail::wyr64_2 ( p + 16 ) ^ seed, ( ( detail::wyr16 ( p + 24 ) << 8 ) | detail::wyr08 ( p + 24 + 2 ) ) ^ detail::wyp4 ); break;
    case 28: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( detail::wyr64_2 ( p + 16 ) ^ seed, detail::wyr32 ( p + 24 ) ^ detail::wyp4 ); break;
    case 29: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( detail::wyr64_2 ( p + 16 ) ^ seed, ( ( detail::wyr32 ( p + 24 ) << 8 ) | detail::wyr08 ( p + 24 + 4 ) ) ^ detail::wyp4 ); break;
    case 30: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( detail::wyr64_2 ( p + 16 ) ^ seed, ( ( detail::wyr32 ( p + 24 ) << 16 ) | detail::wyr16 ( p + 24 + 4 ) ) ^ detail::wyp4 ); break;
    case 31: seed = detail::wymum ( detail::wyr64_2 ( p ) ^ seed, detail::wyr64_2 ( p + 8 ) ^ detail::wyp2 ) ^ detail::wymum ( detail::wyr64_2 ( p + 16 ) ^ seed, ( ( detail::wyr32 ( p + 24 ) << 24 ) | ( detail::wyr16 ( p + 24 + 4 ) << 8 ) | detail::wyr08 ( p + 24 + 6 ) ) ^ detail::wyp4 ); break;
    }
    return detail::wymum ( seed, len^detail::wyp5 );
}

static inline std::uint64_t wyhash64 ( std::uint64_t a_, std::uint64_t b_ ) noexcept {
    return detail::wymum ( detail::wymum ( a_ ^ detail::wyp0, b_ ^ detail::wyp1 ), detail::wyp2 );
}
static inline std::uint64_t wyrand ( std::uint64_t *s_ ) noexcept {
    *s_ += detail::wyp0;
    return detail::wymum ( *s_ ^ detail::wyp1, *s_ );
}

// static inline double wyrandu01(std::uint64_t *seed){ const double wynorm=1.0/(1ull<<52); return (wyrand(seed)&0x000fffffffffffffull)*wynorm; }

}

#undef GNU
#undef MSVC
#undef CLANG
#undef GCC
#undef M64
#undef M32
#undef CONSTEXPR
