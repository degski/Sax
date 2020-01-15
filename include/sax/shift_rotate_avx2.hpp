
// MIT License
//
// Copyright (c) 2018, 2019, 2020 degski
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

#include <immintrin.h>

#ifdef __AVX2__

namespace sax {
namespace detail {

[[nodiscard]] inline __m256i left_shift_000_063 ( __m256i a, int n ) noexcept { // 6
    return _mm256_or_si256 (
        _mm256_slli_epi64 ( a, n ),
        _mm256_blend_epi32 ( _mm256_setzero_si256 ( ),
                             _mm256_permute4x64_epi64 ( _mm256_srli_epi64 ( a, 64 - n ), _MM_SHUFFLE ( 2, 1, 0, 0 ) ),
                             _MM_SHUFFLE ( 3, 3, 3, 0 ) ) );
}
[[nodiscard]] inline __m256i left_shift_064_127 ( __m256i a, int n ) noexcept { // 7
    __m256i b = _mm256_slli_epi64 ( a, n );
    __m256i d = _mm256_permute4x64_epi64 ( b, _MM_SHUFFLE ( 2, 1, 0, 0 ) );
    __m256i c = _mm256_srli_epi64 ( a, 64 - n );
    __m256i e = _mm256_permute4x64_epi64 ( c, _MM_SHUFFLE ( 1, 0, 0, 0 ) );
    __m256i f = _mm256_blend_epi32 ( _mm256_setzero_si256 ( ), d, _MM_SHUFFLE ( 3, 3, 3, 0 ) );
    __m256i g = _mm256_blend_epi32 ( _mm256_setzero_si256 ( ), e, _MM_SHUFFLE ( 3, 3, 0, 0 ) ); // 6
    return _mm256_or_si256 ( f, g );
}
[[nodiscard]] inline __m256i left_shift_128_191 ( __m256i a, int n ) noexcept { // 7
    __m256i b = _mm256_slli_epi64 ( a, n );
    __m256i d = _mm256_permute4x64_epi64 ( b, _MM_SHUFFLE ( 1, 0, 0, 0 ) );
    __m256i c = _mm256_srli_epi64 ( a, 64 - n );
    __m256i e = _mm256_permute4x64_epi64 ( c, _MM_SHUFFLE ( 1, 0, 0, 0 ) );
    __m256i f = _mm256_blend_epi32 ( _mm256_setzero_si256 ( ), d, _MM_SHUFFLE ( 3, 3, 0, 0 ) );
    __m256i g = _mm256_blend_epi32 ( _mm256_setzero_si256 ( ), e, _MM_SHUFFLE ( 3, 0, 0, 0 ) );
    return _mm256_or_si256 ( f, g );
}
[[nodiscard]] inline __m256i left_shift_192_255 ( __m256i a, int n ) noexcept { // 5
    return _mm256_blend_epi32 ( _mm256_setzero_si256 ( ),
                                _mm256_slli_epi64 ( _mm256_permute4x64_epi64 ( a, _MM_SHUFFLE ( 0, 0, 0, 0 ) ), n ),
                                _MM_SHUFFLE ( 3, 0, 0, 0 ) );
}
} // namespace detail
[[nodiscard]] inline __m256i _mm256_sli_si256 ( __m256i a, int n ) noexcept {
    if ( n < 128 )
        return n < 64 ? detail::left_shift_000_063 ( a, n ) : detail::left_shift_064_127 ( a, n % 64 );
    else
        return n < 192 ? detail::left_shift_128_191 ( a, n % 64 ) : detail::left_shift_192_255 ( a, n % 64 );
}

namespace detail {

[[nodiscard]] inline __m256i right_shift_000_063 ( __m256i a, int n ) noexcept { // 6
    return _mm256_or_si256 (
        _mm256_srli_epi64 ( a, n ),
        _mm256_blend_epi32 ( _mm256_setzero_si256 ( ),
                             _mm256_permute4x64_epi64 ( _mm256_slli_epi64 ( a, 64 - n ), _MM_SHUFFLE ( 0, 3, 2, 1 ) ),
                             _MM_SHUFFLE ( 0, 3, 3, 3 ) ) );
}
[[nodiscard]] inline __m256i right_shift_064_127 ( __m256i a, int n ) noexcept { // 7
    __m256i b = _mm256_srli_epi64 ( a, n );
    __m256i d = _mm256_permute4x64_epi64 ( b, _MM_SHUFFLE ( 3, 3, 2, 1 ) );
    __m256i c = _mm256_slli_epi64 ( a, 64 - n );
    __m256i e = _mm256_permute4x64_epi64 ( c, _MM_SHUFFLE ( 3, 3, 3, 2 ) );
    __m256i f = _mm256_blend_epi32 ( _mm256_setzero_si256 ( ), d, _MM_SHUFFLE ( 0, 3, 3, 3 ) );
    __m256i g = _mm256_blend_epi32 ( _mm256_setzero_si256 ( ), e, _MM_SHUFFLE ( 0, 0, 3, 3 ) );
    return _mm256_or_si256 ( f, g );
}
[[nodiscard]] inline __m256i right_shift_128_191 ( __m256i a, int n ) noexcept { // 7
    __m256i b = _mm256_srli_epi64 ( a, n );
    __m256i d = _mm256_permute4x64_epi64 ( b, _MM_SHUFFLE ( 3, 2, 3, 2 ) );
    __m256i c = _mm256_slli_epi64 ( a, 64 - n );
    __m256i e = _mm256_permute4x64_epi64 ( c, _MM_SHUFFLE ( 3, 2, 1, 3 ) );
    __m256i f = _mm256_blend_epi32 ( _mm256_setzero_si256 ( ), d, _MM_SHUFFLE ( 0, 0, 3, 3 ) );
    __m256i g = _mm256_blend_epi32 ( _mm256_setzero_si256 ( ), e, _MM_SHUFFLE ( 0, 0, 0, 3 ) );
    return _mm256_or_si256 ( f, g );
}
[[nodiscard]] inline __m256i right_shift_192_255 ( __m256i a, int n ) noexcept { // 5
    return _mm256_blend_epi32 ( _mm256_setzero_si256 ( ),
                                _mm256_srli_epi64 ( _mm256_permute4x64_epi64 ( a, _MM_SHUFFLE ( 0, 0, 0, 3 ) ), n ),
                                _MM_SHUFFLE ( 0, 0, 0, 3 ) );
}
} // namespace detail

[[nodiscard]] inline __m256i _mm256_sri_si256 ( __m256i a, int n ) noexcept {
    if ( n < 128 )
        return n < 64 ? detail::right_shift_000_063 ( a, n ) : detail::right_shift_064_127 ( a, n % 64 );
    else
        return n < 192 ? detail::right_shift_128_191 ( a, n % 64 ) : detail::right_shift_192_255 ( a, n % 64 );
}

namespace detail {

[[nodiscard]] inline __m256i left_rotate_000_063 ( __m256i a, int n ) noexcept { // 5
    return _mm256_or_si256 ( _mm256_slli_epi64 ( a, n ),
                             _mm256_permute4x64_epi64 ( _mm256_srli_epi64 ( a, 64 - n ), _MM_SHUFFLE ( 2, 1, 0, 3 ) ) );
}
[[nodiscard]] inline __m256i left_rotate_064_127 ( __m256i a, int n ) noexcept { // 6
    __m256i b = _mm256_slli_epi64 ( a, n );
    __m256i c = _mm256_srli_epi64 ( a, 64 - n );
    __m256i d = _mm256_permute4x64_epi64 ( b, _MM_SHUFFLE ( 2, 1, 0, 3 ) );
    __m256i e = _mm256_permute4x64_epi64 ( c, _MM_SHUFFLE ( 1, 0, 3, 2 ) );
    return _mm256_or_si256 ( d, e );
}
[[nodiscard]] inline __m256i left_rotate_128_191 ( __m256i a, int n ) noexcept { // 6
    __m256i b = _mm256_slli_epi64 ( a, n );
    __m256i c = _mm256_srli_epi64 ( a, 64 - n );
    __m256i d = _mm256_permute4x64_epi64 ( b, _MM_SHUFFLE ( 1, 0, 3, 2 ) );
    __m256i e = _mm256_permute4x64_epi64 ( c, _MM_SHUFFLE ( 0, 3, 2, 1 ) );
    return _mm256_or_si256 ( d, e );
}
[[nodiscard]] inline __m256i left_rotate_192_255 ( __m256i a, int n ) noexcept { // 5
    return _mm256_or_si256 ( _mm256_srli_epi64 ( a, 64 - n ),
                             _mm256_permute4x64_epi64 ( _mm256_slli_epi64 ( a, n ), _MM_SHUFFLE ( 0, 3, 2, 1 ) ) );
}
} // namespace detail

[[nodiscard]] inline __m256i _mm256_rli_si256 ( __m256i a, int n ) noexcept {
    if ( n < 128 )
        return n < 64 ? detail::left_rotate_000_063 ( a, n ) : detail::left_rotate_064_127 ( a, n % 64 );
    else
        return n < 192 ? detail::left_rotate_128_191 ( a, n % 64 ) : detail::left_rotate_192_255 ( a, n % 64 );
}

namespace detail {

[[nodiscard]] inline __m256i right_rotate_000_063 ( __m256i a, int n ) noexcept { // 5
    return _mm256_or_si256 ( _mm256_srli_epi64 ( a, n ),
                             _mm256_permute4x64_epi64 ( _mm256_slli_epi64 ( a, 64 - n ), _MM_SHUFFLE ( 0, 3, 2, 1 ) ) );
}
[[nodiscard]] inline __m256i right_rotate_064_127 ( __m256i a, int n ) noexcept { // 6
    __m256i b = _mm256_srli_epi64 ( a, n );
    __m256i c = _mm256_slli_epi64 ( a, 64 - n );
    __m256i d = _mm256_permute4x64_epi64 ( b, _MM_SHUFFLE ( 0, 3, 2, 1 ) );
    __m256i e = _mm256_permute4x64_epi64 ( c, _MM_SHUFFLE ( 1, 0, 3, 2 ) );
    return _mm256_or_si256 ( d, e );
}
[[nodiscard]] inline __m256i right_rotate_128_191 ( __m256i a, int n ) noexcept { // 6
    __m256i b = _mm256_srli_epi64 ( a, n );
    __m256i c = _mm256_slli_epi64 ( a, 64 - n );
    __m256i d = _mm256_permute4x64_epi64 ( b, _MM_SHUFFLE ( 1, 0, 3, 2 ) );
    __m256i e = _mm256_permute4x64_epi64 ( c, _MM_SHUFFLE ( 2, 1, 0, 3 ) );
    return _mm256_or_si256 ( d, e );
}
[[nodiscard]] inline __m256i right_rotate_192_255 ( __m256i a, int n ) noexcept { // 5
    return _mm256_or_si256 ( _mm256_slli_epi64 ( a, 64 - n ),
                             _mm256_permute4x64_epi64 ( _mm256_srli_epi64 ( a, n ), _MM_SHUFFLE ( 2, 1, 0, 3 ) ) );
}
} // namespace detail

[[nodiscard]] inline __m256i _mm256_rri_si256 ( __m256i a, int n ) noexcept {
    if ( n < 128 )
        return n < 64 ? detail::right_rotate_000_063 ( a, n ) : detail::right_rotate_064_127 ( a, n % 64 );
    else
        return n < 192 ? detail::right_rotate_128_191 ( a, n % 64 ) : detail::right_rotate_192_255 ( a, n % 64 );
}
} // namespace sax

#endif // ifdef __AVX2__
