
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

#ifndef NOMINMAX
#    define NOMINMAX
#endif

#include <Memoryapi.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include <hedley.hpp>

namespace sax {

template<typename ValueType, typename SizeType, SizeType Capacity>
struct vm_array {

    using value_type = ValueType;

    using pointer       = value_type *;
    using const_pointer = value_type const *;

    using reference       = value_type &;
    using const_reference = value_type const &;
    using rv_reference    = value_type &&;

    using size_type       = SizeType;
    using difference_type = std::make_signed<size_type>;

    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = pointer;
    using const_reverse_iterator = const_pointer;

    vm_array ( ) :
        m_begin{ reinterpret_cast<pointer> ( VirtualAlloc ( nullptr, capacity_b ( ), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE ) ) },
        m_end{ m_begin + capacity_b ( ) } {
        if ( HEDLEY_UNLIKELY ( not m_begin ) )
            throw std::bad_alloc ( );
        if constexpr ( not std::is_trivial<value_type>::value ) {
            for ( auto & v : *this )
                new ( std::addressof ( v ) ) value_type{ };
        }
    };

    vm_array ( std::initializer_list<value_type> il_ ) : vm_array{ } {
        pointer p = m_begin;
        for ( value_type const & v : il_ )
            new ( p++ ) value_type{ v };
    }

    ~vm_array ( ) {
        if constexpr ( not std::is_trivial<value_type>::value ) {
            for ( value_type & v : *this )
                v.~value_type ( );
        }
        if ( HEDLEY_LIKELY ( m_begin ) ) {
            VirtualFree ( m_begin, capacity_b ( ), MEM_RELEASE );
            m_end = m_begin = nullptr;
        }
    }

    [[nodiscard]] constexpr size_type capacity ( ) const noexcept { return Capacity; }
    [[nodiscard]] constexpr size_type size ( ) const noexcept { return capacity ( ); }
    [[nodiscard]] constexpr size_type max_size ( ) const noexcept { return capacity ( ); }

    [[nodiscard]] const_pointer data ( ) const noexcept { return reinterpret_cast<pointer> ( m_begin ); }
    [[nodiscard]] pointer data ( ) noexcept { return const_cast<pointer> ( std::as_const ( *this ).data ( ) ); }

    [[nodiscard]] const_iterator begin ( ) const noexcept { return m_begin; }
    [[nodiscard]] const_iterator cbegin ( ) const noexcept { return begin ( ); }
    [[nodiscard]] iterator begin ( ) noexcept { return const_cast<iterator> ( std::as_const ( *this ).begin ( ) ); }

    [[nodiscard]] const_iterator end ( ) const noexcept { return m_end; }
    [[nodiscard]] const_iterator cend ( ) const noexcept { return end ( ); }
    [[nodiscard]] iterator end ( ) noexcept { return const_cast<iterator> ( std::as_const ( *this ).end ( ) ); }

    [[nodiscard]] const_iterator rbegin ( ) const noexcept { return m_end - 1; }
    [[nodiscard]] const_iterator crbegin ( ) const noexcept { return rbegin ( ); }
    [[nodiscard]] iterator rbegin ( ) noexcept { return const_cast<iterator> ( std::as_const ( *this ).rbegin ( ) ); }

    [[nodiscard]] const_iterator rend ( ) const noexcept { return m_begin - 1; }
    [[nodiscard]] const_iterator crend ( ) const noexcept { return rend ( ); }
    [[nodiscard]] iterator rend ( ) noexcept { return const_cast<iterator> ( std::as_const ( *this ).rend ( ) ); }

    [[nodiscard]] const_reference front ( ) const noexcept { return *begin ( ); }
    [[nodiscard]] reference front ( ) noexcept { return const_cast<reference> ( std::as_const ( *this ).front ( ) ); }

    [[nodiscard]] const_reference back ( ) const noexcept { return *rbegin ( ); }
    [[nodiscard]] reference back ( ) noexcept { return const_cast<reference> ( std::as_const ( *this ).back ( ) ); }

    [[nodiscard]] const_reference at ( size_type const i_ ) const {
        if constexpr ( std::is_signed<size_type>::value ) {
            if ( HEDLEY_LIKELY ( 0 <= i_ and i_ < size ( ) ) )
                return m_begin[ i_ ];
            else
                throw std::runtime_error ( "index out of bounds" );
        }
        else {
            if ( HEDLEY_LIKELY ( i_ < size ( ) ) )
                return m_begin[ i_ ];
            else
                throw std::runtime_error ( "index out of bounds" );
        }
    }
    [[nodiscard]] reference at ( size_type const i_ ) { return const_cast<reference> ( std::as_const ( *this ).at ( i_ ) ); }

    [[nodiscard]] const_reference operator[] ( size_type const i_ ) const noexcept {
        if constexpr ( std::is_signed<size_type>::value ) {
            assert ( 0 <= i_ and i_ < size ( ) );
        }
        else {
            assert ( i_ < size ( ) );
        }
        return m_begin[ i_ ];
    }
    [[nodiscard]] reference operator[] ( size_type const i_ ) noexcept {
        return const_cast<reference> ( std::as_const ( *this ).operator[] ( i_ ) );
    }

    private:
    static constexpr size_type page_size_b = static_cast<size_type> ( 65'536 ); // 64KB

    [[nodiscard]] constexpr size_type capacity_b ( ) const noexcept {
        constexpr size_type req = static_cast<size_type> ( Capacity * sizeof ( value_type ) );
        return req % page_size_b ? ( ( req + page_size_b ) / page_size_b ) * page_size_b : req;
    }
    [[nodiscard]] constexpr size_type size_b ( ) const noexcept { return capacity_b ( ); }

    pointer m_begin, m_end;
};

template<typename ValueType, typename SizeType, SizeType Capacity>
struct vm_vector {

    using value_type = ValueType;

    using pointer       = value_type *;
    using const_pointer = value_type const *;

    using reference       = value_type &;
    using const_reference = value_type const &;
    using rv_reference    = value_type &&;

    using size_type       = SizeType;
    using difference_type = std::make_signed<size_type>;

    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = pointer;
    using const_reverse_iterator = const_pointer;

    vm_vector ( ) :
        m_begin{ reinterpret_cast<pointer> ( VirtualAlloc ( nullptr, capacity_b ( ), MEM_RESERVE, PAGE_READWRITE ) ) },
        m_end{ m_begin }, m_committed_b{ 0u } {
        if ( HEDLEY_UNLIKELY ( not m_begin ) )
            throw std::bad_alloc ( );
    };

    vm_vector ( std::initializer_list<value_type> il_ ) : vm_vector{ } {
        for ( value_type const & v : il_ )
            push_back ( v );
    }

    explicit vm_vector ( size_type const s_, value_type const & v_ ) : vm_vector{ } {
        size_type rc = required_b ( s_ );
        if ( HEDLEY_UNLIKELY ( not VirtualAlloc ( m_end, rc, MEM_COMMIT, PAGE_READWRITE ) ) )
            throw std::bad_alloc ( );
        m_committed_b = rc;
        for ( pointer e = m_begin + std::min ( s_, capacity ( ) ); m_end < e; ++m_end )
            new ( m_end ) value_type{ v_ };
    }

    ~vm_vector ( ) {
        if constexpr ( not std::is_trivial<value_type>::value ) {
            for ( value_type & v : *this )
                v.~value_type ( );
        }
        if ( HEDLEY_LIKELY ( m_begin ) ) {
            VirtualFree ( m_begin, capacity_b ( ), MEM_RELEASE );
            m_end = m_begin = nullptr;
            m_committed_b   = 0u;
        }
    }

    [[nodiscard]] constexpr size_type capacity ( ) const noexcept { return Capacity; }
    [[nodiscard]] size_type size ( ) const noexcept {
        return reinterpret_cast<value_type *> ( m_end ) - reinterpret_cast<value_type *> ( m_begin );
    }
    [[nodiscard]] constexpr size_type max_size ( ) const noexcept { return capacity ( ); }

    template<typename... Args>
    [[maybe_unused]] reference emplace_back ( Args &&... value_ ) {
        if ( HEDLEY_UNLIKELY ( size_b ( ) == m_committed_b ) ) {
            size_type cib = std::min ( m_committed_b ? grow ( m_committed_b ) : allocation_page_size_b, capacity_b ( ) );
            if ( HEDLEY_UNLIKELY ( not VirtualAlloc ( m_end, cib - m_committed_b, MEM_COMMIT, PAGE_READWRITE ) ) )
                throw std::bad_alloc ( );
            m_committed_b = cib;
        }
        return *new ( m_end++ ) value_type{ std::forward<Args> ( value_ )... };
    }
    [[maybe_unused]] reference push_back ( const_reference value_ ) { return emplace_back ( value_type{ value_ } ); }

    void pop_back ( ) noexcept {
        assert ( size ( ) );
        if constexpr ( not std::is_trivial<value_type>::value ) {
            back ( ).~value_type ( );
        }
        --m_end;
    }

    [[nodiscard]] const_pointer data ( ) const noexcept { return reinterpret_cast<pointer> ( m_begin ); }
    [[nodiscard]] pointer data ( ) noexcept { return const_cast<pointer> ( std::as_const ( *this ).data ( ) ); }

    [[nodiscard]] const_iterator begin ( ) const noexcept { return m_begin; }
    [[nodiscard]] const_iterator cbegin ( ) const noexcept { return begin ( ); }
    [[nodiscard]] iterator begin ( ) noexcept { return const_cast<iterator> ( std::as_const ( *this ).begin ( ) ); }

    [[nodiscard]] const_iterator end ( ) const noexcept { return m_end; }
    [[nodiscard]] const_iterator cend ( ) const noexcept { return end ( ); }
    [[nodiscard]] iterator end ( ) noexcept { return const_cast<iterator> ( std::as_const ( *this ).end ( ) ); }

    [[nodiscard]] const_iterator rbegin ( ) const noexcept { return m_end - 1; }
    [[nodiscard]] const_iterator crbegin ( ) const noexcept { return rbegin ( ); }
    [[nodiscard]] iterator rbegin ( ) noexcept { return const_cast<iterator> ( std::as_const ( *this ).rbegin ( ) ); }

    [[nodiscard]] const_iterator rend ( ) const noexcept { return m_begin - 1; }
    [[nodiscard]] const_iterator crend ( ) const noexcept { return rend ( ); }
    [[nodiscard]] iterator rend ( ) noexcept { return const_cast<iterator> ( std::as_const ( *this ).rend ( ) ); }

    [[nodiscard]] const_reference front ( ) const noexcept { return *begin ( ); }
    [[nodiscard]] reference front ( ) noexcept { return const_cast<reference> ( std::as_const ( *this ).front ( ) ); }

    [[nodiscard]] const_reference back ( ) const noexcept { return *rbegin ( ); }
    [[nodiscard]] reference back ( ) noexcept { return const_cast<reference> ( std::as_const ( *this ).back ( ) ); }

    [[nodiscard]] const_reference at ( size_type const i_ ) const {
        if constexpr ( std::is_signed<size_type>::value ) {
            if ( HEDLEY_LIKELY ( 0 <= i_ and i_ < size ( ) ) )
                return m_begin[ i_ ];
            else
                throw std::runtime_error ( "index out of bounds" );
        }
        else {
            if ( HEDLEY_LIKELY ( i_ < size ( ) ) )
                return m_begin[ i_ ];
            else
                throw std::runtime_error ( "index out of bounds" );
        }
    }
    [[nodiscard]] reference at ( size_type const i_ ) { return const_cast<reference> ( std::as_const ( *this ).at ( i_ ) ); }

    [[nodiscard]] const_reference operator[] ( size_type const i_ ) const noexcept {
        if constexpr ( std::is_signed<size_type>::value ) {
            assert ( 0 <= i_ and i_ < size ( ) );
        }
        else {
            assert ( i_ < size ( ) );
        }
        return m_begin[ i_ ];
    }
    [[nodiscard]] reference operator[] ( size_type const i_ ) noexcept {
        return const_cast<reference> ( std::as_const ( *this ).operator[] ( i_ ) );
    }

    private:
    static constexpr size_type page_size_b            = static_cast<size_type> ( 65'536 );         // 64KB
    static constexpr size_type allocation_page_size_b = static_cast<size_type> ( 1'600 * 65'536 ); // 100MB

    [[nodiscard]] size_type required_b ( size_type const & r_ ) const noexcept {
        std::size_t req = r_ * sizeof ( value_type );
        return req % page_size_b ? ( ( req + page_size_b ) / page_size_b ) * page_size_b : req;
    }
    [[nodiscard]] constexpr size_type capacity_b ( ) const noexcept {
        constexpr std::size_t cap = Capacity * sizeof ( value_type );
        return cap % page_size_b ? ( ( cap + page_size_b ) / page_size_b ) * page_size_b : cap;
    }
    [[nodiscard]] size_type size_b ( ) const noexcept {
        return reinterpret_cast<char *> ( m_end ) - reinterpret_cast<char *> ( m_begin );
    }

    [[nodiscard]] static size_type grow ( size_type const & cap_b_ ) noexcept { return cap_b_ + allocation_page_size_b; }
    [[nodiscard]] static size_type shrink ( size_type const & cap_b_ ) noexcept { return cap_b_ - allocation_page_size_b; }

    pointer m_begin, m_end;
    size_type m_committed_b;
};

} // namespace sax
