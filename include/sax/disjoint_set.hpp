
// MIT License
//
// Copyright (c) 2020 degski
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

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <array>

namespace sax {

template<typename SizeType, SizeType N>
struct alignas ( 64 ) disjoint_set {

    using size_type = SizeType;

    constexpr size_type find ( size_type x_ ) noexcept {
        if ( parent[ x_ ] == x_ )
            return x_;
        return parent[ x_ ] = find ( parent[ x_ ] );
    }

    constexpr void unite ( size_type x_, size_type y_ ) noexcept {
        size_type irep = find ( x_ ), jrep = find ( y_ );
        if ( irep == jrep )
            return;
        switch ( size_type irank = rank[ irep ], jrank = rank[ jrep ]; ( irank > jrank ) - ( irank < jrank ) ) {
            case -1: parent[ irep ] = jrep; return;
            case +0:
                parent[ irep ] = jrep;
                rank[ jrep ]++;
                return;
            case +1: parent[ jrep ] = irep; return;
        }
    }

    private:
    using container = std::array<size_type, N>;
    using iterator  = typename container::iterator;

    static constexpr size_type n = N;

    constexpr void iota ( iterator first_, iterator last_, size_type value_ = 0 ) noexcept {
        while ( first_ != last_ )
            *first_++ = value_++;
    }

    constexpr container make_parent ( ) noexcept {
        container parent;
        iota ( parent.begin ( ), parent.end ( ) );
        return parent;
    }

    container rank = { }, parent = make_parent ( );
};

} // namespace sax
