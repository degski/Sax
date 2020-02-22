
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

#include <algorithm>
#include <array>
#include <limits>

namespace sax {

using atom_type                     = char const *;
static constexpr atom_type nullatom = "none";

namespace detail {

template<std::intptr_t Size>
using required_size_type =
    std::conditional_t<std::numeric_limits<char>::max ( ) <= Size, char,
                       std::conditional_t<std::numeric_limits<short>::max ( ) <= Size, short,
                                          std::conditional_t<std::numeric_limits<int>::max ( ) <= Size, int, long long>>>;
}

// 'disjoint_set' using the path-compression technique and ranking
template<std::intptr_t PopulationSize, std::intptr_t GroupsSize>
struct disjoint_set {

    using size_type = detail::required_size_type<PopulationSize>;

    constexpr size_type find ( size_type x_ ) noexcept { return find_impl ( x_ ); }
    constexpr atom_type find_name ( size_type x_ ) noexcept { return name[ find ( x_ ) ]; }

    // Returns the current group-representative-index, iff an atom is passed, that atom will be set as the groupss name.
    // The groups' name will be set to the value that was passed the last call with a name for the current group.
    [[maybe_unused]] constexpr size_type unite ( size_type x_, size_type y_, atom_type group_name_ = nullatom ) noexcept {
        size_type group_rep = unite_impl ( x_, y_ );
        if ( nullatom != group_name_ )
            name[ group_rep ] = group_name_;
        return group_rep;
    }
    [[maybe_unused]] constexpr atom_type unite_name ( size_type x_, size_type y_, atom_type group_name_ = nullatom ) noexcept {
        return name[ unite ( x_, y_, group_name_ ) ];
    }

    private:
    constexpr size_type find_impl ( size_type x_ ) noexcept {
        if ( parent[ x_ ] == x_ )
            return x_;
        return parent[ x_ ] = find_impl ( parent[ x_ ] );
    }

    // Returns the current group representative.
    [[maybe_unused]] constexpr size_type unite_impl ( size_type x_, size_type y_ ) noexcept {
        if ( size_type irep = find_impl ( x_ ), jrep = find_impl ( y_ ); irep != jrep )
            switch ( size_type irank = rank[ irep ], jrank = rank[ jrep ]; ( irank > jrank ) - ( irank < jrank ) ) {
                case -1: return parent[ irep ] = jrep;
                case +0: rank[ jrep ]++; return parent[ irep ] = jrep;
                case +1: [[fallthrough]];
                default: return parent[ jrep ] = irep;
            }
        else
            return irep;
    }

    using container = std::array<size_type, PopulationSize>;
    using names     = std::array<atom_type, PopulationSize>;

    alignas ( 64 ) container rank   = { };
    alignas ( 64 ) container parent = [] {
        container p;
        auto it = p.begin ( ), end = p.end ( );
        size_type value = 0;
        while ( it != end )
            *it++ = value++;
        return p;
    }( );

    alignas ( 64 ) names name = [] {
        names n;
        std::fill ( n.begin ( ), n.end ( ), nullatom );
        return n;
    }( );
};

} // namespace sax
