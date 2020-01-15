
// MIT License
//
// Copyright (c) 2013 Cosku Acay, http://www.coskuacay.com, https://github.com/cacay/MemoryPool
// Copyright (c) 2019, 2020 degski
//      std::vector -> pt::pector
//      std::sort -> sax::ska_sort
//      Functionally still as per the original.
//      Optimized for is_trivially_destructible-types
//      Requires C++17
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

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <cstddef>

#include <pector/pector.h>
#include <pector/malloc_allocator.h>

namespace sax {

template<typename T, size_t BlockSize = 4096>
class MemoryPool {
    public:
    using value_type                             = T;
    using pointer                                = T *;
    using reference                              = T &;
    using const_pointer                          = const T *;
    using const_reference                        = const T &;
    using size_type                              = size_t;
    using difference_type                        = ptrdiff_t;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap            = std::true_type;

    template<typename U>
    struct rebind {
        using other = MemoryPool<U>;
    };

    MemoryPool ( ) noexcept;
    MemoryPool ( const MemoryPool & memoryPool ) noexcept;
    MemoryPool ( MemoryPool && memoryPool ) noexcept;
    template<typename U>
    MemoryPool ( const MemoryPool<U> & memoryPool ) noexcept;

    ~MemoryPool ( ) noexcept;

    MemoryPool & operator= ( const MemoryPool & memoryPool ) = delete;
    MemoryPool & operator                                    = ( MemoryPool && memoryPool ) noexcept;

    // Can only allocate one object at a time. n and hint are ignored
    pointer allocate ( size_type n = 1, const_pointer hint = 0 );
    void deallocate ( pointer p, size_type n = 1 );

    static constexpr size_type block_size ( ) noexcept;
    static constexpr size_type max_size ( ) noexcept;

    size_type memory_size ( ) const noexcept;

    template<typename U, typename... Args>
    void construct ( U * p, Args &&... args );
    template<typename U>
    void destroy ( U * p );

    template<typename... Args>
    pointer newElement ( Args &&... args );
    void deleteElement ( pointer p );

    private:
    union Slot_ {
        value_type element;
        Slot_ * next;
    };

    using data_pointer_        = char *;
    using slot_type_           = Slot_;
    using slot_pointer_        = Slot_ *;
    using slot_pointer_vector_ = pt::pector<slot_pointer_, pt::malloc_allocator<slot_pointer_, true, true>, std::uint32_t>;

    slot_pointer_ currentBlock_;
    slot_pointer_ currentSlot_;
    slot_pointer_ lastSlot_;
    slot_pointer_ freeSlots_;

    size_type padPointer ( data_pointer_ p, size_type align ) const noexcept;
    void allocateBlock ( );
    slot_pointer_ firstSlot ( const slot_pointer_ block_ ) const noexcept;
    slot_pointer_ lastSlot ( const slot_pointer_ block_ ) const noexcept;
    slot_pointer_vector_ pointerVector ( slot_pointer_ sp_ ) const noexcept;
    slot_pointer_vector_ blockVector ( ) const noexcept;

    static_assert ( BlockSize >= 2 * sizeof ( slot_type_ ), "BlockSize too small." );
};
} // namespace sax

#include "detail/memorypool.inl"
