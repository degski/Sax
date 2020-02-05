
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

#ifndef NOMINMAX
#    define NOMINMAX
#endif

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <limits>
#include <string>
#include <type_traits>

namespace sax {

namespace detail {
template<typename T>
using is_string = std::is_base_of<std::basic_string<typename T::value_type>, T>;
}

// Greatest Common Denominator.
template<typename T, typename = std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>>>>
constexpr T gcd ( T a_, T b_ ) noexcept {
    while ( true ) {
        if ( !a_ ) {
            return b_;
        }
        b_ %= a_;
        if ( !b_ ) {
            return a_;
        }
        a_ %= b_;
    }
}

// Least Common Multiple.
template<typename T, typename = std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>>>>
constexpr T lcm ( T const a_, T const b_ ) noexcept {
    T const t = gcd<T> ( a_, b_ );
    return t ? a_ / t * b_ : T ( 0 );
}

// In number theory, two integers a and b are said to be relatively prime, mutually prime, or
// coprime (also spelled co-prime) if the only positive integer that divides both of them is 1.
template<typename T, typename = std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>>>>
constexpr bool are_coprime ( T const a_, T const b_ ) noexcept {
    assert ( a_ > 0 && b_ > 0 );
    return gcd<T> ( a_, b_ ) == T ( 1 );
}

// Integer LogN.
template<int Base, typename T, typename sfinae = std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>>>>
constexpr T iLog ( T const n_, T const p_ = T ( 0 ) ) noexcept {
    return n_ < Base ? p_ : iLog<Base, T, sfinae> ( n_ / Base, p_ + 1 );
}

// Integer Log2.
template<typename T, typename = std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>>>>
constexpr T ilog2 ( T const n_ ) noexcept {
    return iLog<2, T> ( n_ );
}

template<typename T, typename = std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>>>>
constexpr T prev_power_2 ( T value_ ) noexcept {
    value_ |= ( value_ >> 1 );
    value_ |= ( value_ >> 2 );
    if constexpr ( sizeof ( T ) > 1 ) {
        value_ |= ( value_ >> 4 );
    }
    if constexpr ( sizeof ( T ) > 2 ) {
        value_ |= ( value_ >> 8 );
    }
    if constexpr ( sizeof ( T ) > 4 ) {
        value_ |= ( value_ >> 16 );
    }
    return value_;
}

template<typename T, typename = std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>>>>
constexpr T next_power_2 ( T value_ ) noexcept {
    return ++prev_power_2 ( value_ );
}

template<typename T, typename = std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>>>>
constexpr bool is_power_2 ( T const n_ ) noexcept {
    return n_ and not( n_ & ( n_ - 1 ) );
}

template<typename T, typename = std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>>>>
constexpr T sum2n ( T const n_ ) noexcept {
    return ( n_ * ( n_ + 1 ) ) / T ( 2 );
}

template<typename T, typename = std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>>>>
constexpr T sumMToN ( T const m_, T const n_ ) noexcept {
    return ( ( n_ * ( n_ + 1 ) ) - ( m_ * ( m_ + 1 ) ) ) / T ( 2 );
}

// Gray Coding.
template<typename T, typename = std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>>>>
constexpr T dec2gray ( T const i_ ) noexcept {
    return i_ ^ ( i_ >> 1 );
}

inline constexpr std::uint8_t gray2dec ( std::uint8_t gray_ ) noexcept {
    gray_ ^= gray_ >> 4;
    gray_ ^= gray_ >> 2;
    gray_ ^= gray_ >> 1;
    return gray_;
}

inline constexpr std::uint16_t gray2dec ( std::uint16_t gray_ ) noexcept {
    gray_ ^= gray_ >> 8;
    gray_ ^= gray_ >> 4;
    gray_ ^= gray_ >> 2;
    gray_ ^= gray_ >> 1;
    return gray_;
}

inline constexpr std::uint32_t gray2dec ( std::uint32_t gray_ ) noexcept {
    gray_ ^= gray_ >> 16;
    gray_ ^= gray_ >> 8;
    gray_ ^= gray_ >> 4;
    gray_ ^= gray_ >> 2;
    gray_ ^= gray_ >> 1;
    return gray_;
}

inline constexpr std::uint64_t gray2dec ( std::uint64_t gray_ ) noexcept {
    gray_ ^= gray_ >> 32;
    gray_ ^= gray_ >> 16;
    gray_ ^= gray_ >> 8;
    gray_ ^= gray_ >> 4;
    gray_ ^= gray_ >> 2;
    gray_ ^= gray_ >> 1;
    return gray_;
}

// FNV1a c++11 constexpr compile time hash functions, 32 and 64 bit
// str should be a null terminated string literal, value_ should be left out
// e.g hash_32_fnv1a_const("example")
// code license: public domain or equivalent
// post: https://notes.underscorediscovery.com/constexpr-fnv1a/
inline constexpr std::uint32_t hash_32_fnv1a_const ( char const * const str, std::uint32_t const value_ = 0x811c9dc5 ) noexcept {
    return ( str[ 0 ] == '\0' ) ? value_ : hash_32_fnv1a_const ( &str[ 1 ], ( value_ ^ std::uint32_t ( str[ 0 ] ) ) * 0x1000193 );
}

inline constexpr std::uint64_t hash_64_fnv1a_const ( char const * const str,
                                                     std::uint64_t const value_ = 0xcbf29ce484222325 ) noexcept {
    return ( str[ 0 ] == '\0' ) ? value_
                                : hash_64_fnv1a_const ( &str[ 1 ], ( value_ ^ std::uint64_t ( str[ 0 ] ) ) * 0x100000001b3 );
}

inline constexpr std::uint32_t hash_32_fnv1a_const ( std::string const & str_, std::uint32_t const value_ = 0x811c9dc5 ) noexcept {
    return hash_32_fnv1a_const ( str_.c_str ( ), value_ );
}

inline constexpr std::uint64_t hash_64_fnv1a_const ( std::string const & str_,
                                                     std::uint64_t const value_ = 0xcbf29ce484222325 ) noexcept {
    return hash_64_fnv1a_const ( str_.c_str ( ), value_ );
}

// Integer Hashing.
inline constexpr std::uint32_t hash ( std::uint32_t hash_ ) noexcept {
    hash_ = ( ( hash_ >> 16 ) ^ hash_ ) * 0X45D9F3B;
    hash_ = ( ( hash_ >> 16 ) ^ hash_ ) * 0X45D9F3B;
    hash_ = ( ( hash_ >> 16 ) ^ hash_ );
    return hash_;
}

inline constexpr std::uint32_t unhash ( std::uint32_t hash_ ) noexcept {
    hash_ = ( ( hash_ >> 16 ) ^ hash_ ) * 0X119DE1F3;
    hash_ = ( ( hash_ >> 16 ) ^ hash_ ) * 0X119DE1F3;
    hash_ = ( ( hash_ >> 16 ) ^ hash_ );
    return hash_;
}

// 0x0CF3FD1B9997F637 0xAFC1530680179F87 - 0.0033298184
// 0xD6E8FEB86659FD93 0xCFEE444D8B59A89B - 0.0033215807

inline constexpr std::uint64_t hash ( std::uint64_t hash_ ) noexcept {
    hash_ = ( ( hash_ >> 32 ) ^ hash_ ) * 0xD6E8FEB86659FD93;
    hash_ = ( ( hash_ >> 32 ) ^ hash_ ) * 0xD6E8FEB86659FD93;
    hash_ = ( ( hash_ >> 32 ) ^ hash_ );
    return hash_;
}

inline constexpr std::uint64_t unhash ( std::uint64_t hash_ ) noexcept {
    hash_ = ( ( hash_ >> 32 ) ^ hash_ ) * 0xCFEE444D8B59A89B;
    hash_ = ( ( hash_ >> 32 ) ^ hash_ ) * 0xCFEE444D8B59A89B;
    hash_ = ( ( hash_ >> 32 ) ^ hash_ );
    return hash_;
}

template<typename T>
void hash_combine ( std::uint8_t & seed_, T const & value_ ) noexcept {
    // https://stackoverflow.com/questions/5889238/why-is-xor-the-default-way-to-combine-hashes
    std::hash<T> hasher;
    seed_ ^= hasher ( value_ ) + 0x9E + ( seed_ << 6 ) + ( seed_ >> 2 );
}

template<typename T>
void hash_combine ( std::uint16_t & seed_, T const & value_ ) noexcept {
    // https://stackoverflow.com/questions/5889238/why-is-xor-the-default-way-to-combine-hashes
    std::hash<T> hasher;
    seed_ ^= hasher ( value_ ) + 0x9'E37 + ( seed_ << 6 ) + ( seed_ >> 2 );
}

template<typename T>
void hash_combine ( std::uint32_t & seed_, T const & value_ ) noexcept {
    // https://stackoverflow.com/questions/5889238/why-is-xor-the-default-way-to-combine-hashes
    std::hash<T> hasher;
    seed_ ^= hasher ( value_ ) + 0x9E37'79B9 + ( seed_ << 6 ) + ( seed_ >> 2 );
}

template<typename T>
void hash_combine ( std::uint64_t & seed_, T const & value_ ) noexcept {
    // https://stackoverflow.com/questions/5889238/why-is-xor-the-default-way-to-combine-hashes
    std::hash<T> hasher;
    seed_ ^= hasher ( value_ ) + 0x9E3779B9'7F4A7C15 + ( seed_ << 6 ) + ( seed_ >> 2 );
}

// An invertible function used to mix the bits, borrowed from murmurhash.
inline constexpr std::uint64_t fmix64 ( std::uint64_t key_ ) noexcept {
    key_ ^= key_ >> 33;
    key_ *= 0xFF51AFD7ED558CCD;
    key_ ^= key_ >> 33;
    key_ *= 0xC4CEB9FE1A85EC53;
    key_ ^= key_ >> 33;
    return key_;
}

template<typename T, typename = std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>>>>
constexpr std::uint32_t popCount ( T const x_ ) noexcept {
    if constexpr ( std::is_same<T, std::uint64_t>::value ) {
        return ( std::uint32_t ) __popcnt64 ( x_ );
    }
    else {
        return ( std::uint32_t ) __popcnt ( ( std::uint32_t ) x_ );
    }
}

template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
constexpr T make_odd ( T const i_ ) noexcept {
    return i_ | T ( 1 );
}

template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
constexpr T make_even ( T const i_ ) noexcept {
    return i_ & ~T ( 1 );
}

template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
T bit_xor ( T const l_, T const r_ ) noexcept {
    return l_ ^ r_;
}

template<typename T, typename = std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>>>>
void print_bits (T const n_) noexcept {
    using Tu = typename std::make_unsigned<T>::type;
    Tu n;
    std::memcpy (&n, &n_, sizeof (Tu));
    Tu i = Tu (1) << (sizeof (Tu) * 8 - 1);
    while (i) {
        putchar (int ((n & i) > 0) + int (48));
        i >>= 1;
    }
}

} // namespace sax

