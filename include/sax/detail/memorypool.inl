
// MIT License
//
// Copyright (c) 2013 Cosku Acay, http://www.coskuacay.com, https://github.com/cacay/MemoryPool
// Copyright (c) 2019 degski
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

#include <utility>
#include <type_traits>
#include <iterator>
#include <algorithm>

#include <sax/ska_sort.hpp>


namespace sax {

	template<typename T, size_t BlockSize>
	inline typename MemoryPool<T, BlockSize>::size_type
		MemoryPool<T, BlockSize>::padPointer ( data_pointer_ p, size_type align )
		const noexcept
	{
		uintptr_t result = reinterpret_cast< uintptr_t >( p );
		return ( ( align - result ) % align );
	}



	template<typename T, size_t BlockSize>
	MemoryPool<T, BlockSize>::MemoryPool ( )
		noexcept
		: currentBlock_ ( nullptr )
		, currentSlot_ ( nullptr )
		, lastSlot_ ( nullptr )
		, freeSlots_ ( nullptr )
	{
	}



	template<typename T, size_t BlockSize>
	MemoryPool<T, BlockSize>::MemoryPool ( const MemoryPool & memoryPool )
		noexcept :
	MemoryPool ( )
	{
	}



	template<typename T, size_t BlockSize>
	MemoryPool<T, BlockSize>::MemoryPool ( MemoryPool && memoryPool )
		noexcept
		: currentBlock_ ( memoryPool.currentBlock_ )
		, currentSlot_ ( memoryPool.currentSlot_ )
		, lastSlot_ ( memoryPool.lastSlot_ )
		, freeSlots_ ( memoryPool.freeSlots )
	{
		memoryPool.currentBlock_ = nullptr;
	}


	template<typename T, size_t BlockSize>
	template<class U>
	MemoryPool<T, BlockSize>::MemoryPool ( const MemoryPool<U> & memoryPool )
		noexcept :
	MemoryPool ( )
	{
	}



	template<typename T, size_t BlockSize>
	MemoryPool<T, BlockSize>&
		MemoryPool<T, BlockSize>::operator=( MemoryPool && memoryPool )
		noexcept
	{
		if ( this != &memoryPool ) {
			std::swap ( currentBlock_, memoryPool.currentBlock_ );
			currentSlot_ = memoryPool.currentSlot_;
			lastSlot_ = memoryPool.lastSlot_;
			freeSlots_ = memoryPool.freeSlots;
		}
		return *this;
	}



	template<typename T, size_t BlockSize>
	MemoryPool<T, BlockSize>::~MemoryPool ( )
		noexcept
	{
		if constexpr ( std::negation<std::is_trivially_destructible<T>>::value ) {
			slot_pointer_vector_ sorted_free_vector = pointerVector ( freeSlots_ );
			slot_pointer_ curr = currentBlock_, last = currentSlot_, prev = nullptr, p = nullptr;
			if ( sorted_free_vector.empty ( ) ) {
				while ( curr != nullptr ) {
					prev = curr->next;
					p = firstSlot ( curr );
					while ( p < last ) {
						reinterpret_cast<pointer>( p++ )->~value_type ( );
					}
					operator delete( reinterpret_cast<void*>( curr ) );
					curr = prev;
					last = lastSlot ( curr );
				}
			}
			else {
				ska_sort ( std::begin ( sorted_free_vector ), std::end ( sorted_free_vector ) );
				while ( curr != nullptr ) {
					prev = curr->next;
					p = firstSlot ( curr );
					while ( p < last ) {
						if ( p != *std::lower_bound ( std::cbegin ( sorted_free_vector ), std::cend ( sorted_free_vector ), p ) ) {
							reinterpret_cast<pointer>( p )->~value_type ( );
						}
						++p;
					}
					operator delete( reinterpret_cast<void*>( curr ) );
					curr = prev;
					last = lastSlot ( curr );
				}
			}
		}
		else if constexpr ( std::is_trivially_destructible<T>::value ) {
			slot_pointer_ curr = currentBlock_;
			while ( curr != nullptr ) {
				slot_pointer_ prev = curr->next;
				operator delete( reinterpret_cast<void*>( curr ) );
				curr = prev;
			}
		}
		currentBlock_ = nullptr;
		currentSlot_ = nullptr;
		lastSlot_ = nullptr;
		freeSlots_ = nullptr;
	}



	template<typename T, size_t BlockSize>
	void
		MemoryPool<T, BlockSize>::allocateBlock ( )
	{
		// Allocate space for the new block and store a pointer to the previous one
		data_pointer_ newBlock = reinterpret_cast< data_pointer_ >( operator new( BlockSize ) );
		reinterpret_cast< slot_pointer_ >( newBlock )->next = currentBlock_;
		currentBlock_ = reinterpret_cast< slot_pointer_ >( newBlock );
		// Pad block body to satisfy the alignment requirements for elements
		data_pointer_ body = newBlock + sizeof ( slot_pointer_ );
		size_type bodyPadding = padPointer ( body, alignof( slot_type_ ) );
		currentSlot_ = reinterpret_cast< slot_pointer_ >( body + bodyPadding );
		lastSlot_ = reinterpret_cast< slot_pointer_ >( newBlock + BlockSize - sizeof ( slot_type_ ) + 1 );
	}



	template<typename T, size_t BlockSize>
	inline typename MemoryPool<T, BlockSize>::pointer
		MemoryPool<T, BlockSize>::allocate ( size_type n, const_pointer hint )
	{
		if ( freeSlots_ != nullptr ) {
			pointer result = reinterpret_cast<pointer>( freeSlots_ );
			freeSlots_ = freeSlots_->next;
			return result;
		}
		else {
			if ( currentSlot_ >= lastSlot_ )
				allocateBlock ( );
			return reinterpret_cast<pointer>( currentSlot_++ );
		}
	}



	template<typename T, size_t BlockSize>
	inline void
		MemoryPool<T, BlockSize>::deallocate ( pointer p, size_type n )
	{
		if ( p != nullptr ) {
			reinterpret_cast< slot_pointer_ >( p )->next = freeSlots_;
			freeSlots_ = reinterpret_cast< slot_pointer_ >( p );
		}
	}



	template<typename T, size_t BlockSize>
	constexpr typename MemoryPool<T, BlockSize>::size_type
		MemoryPool<T, BlockSize>::block_size ( )
		noexcept
	{
		return ( BlockSize - sizeof ( data_pointer_ ) ) / sizeof ( slot_type_ );
	}



	template<typename T, size_t BlockSize>
	constexpr typename MemoryPool<T, BlockSize>::size_type
		MemoryPool<T, BlockSize>::max_size ( )
		noexcept
	{
		const size_type maxBlocks = -1 / BlockSize;
		return block_size ( ) * maxBlocks;
	}



	template<typename T, size_t BlockSize>
	typename MemoryPool<T, BlockSize>::size_type
		MemoryPool<T, BlockSize>::memory_size ( )
		const noexcept
	{
		slot_pointer_ sp_ = currentBlock_;
		size_type size = 0;

		while ( sp_ != nullptr ) {

			size += BlockSize;
			sp_ = sp_->next;
		}

		return size;
	}



	template<typename T, size_t BlockSize>
	template<class U, class... Args>
	inline void
		MemoryPool<T, BlockSize>::construct ( U* p, Args&&... args )
	{
		new ( p ) U ( std::forward<Args> ( args )... );
	}



	template<typename T, size_t BlockSize>
	template<class U>
	inline void
		MemoryPool<T, BlockSize>::destroy ( U* p )
	{
		if constexpr ( std::negation<std::is_trivially_destructible<T>>::value ) {
			p->~U ( );
		}
	}



	template<typename T, size_t BlockSize>
	template<class... Args>
	inline typename MemoryPool<T, BlockSize>::pointer
		MemoryPool<T, BlockSize>::newElement ( Args&&... args )
	{
		pointer result = allocate ( );
		construct<value_type> ( result, std::forward<Args> ( args )... );
		return result;
	}



	template<typename T, size_t BlockSize>
	inline void
		MemoryPool<T, BlockSize>::deleteElement ( pointer p )
	{
		if ( p != nullptr ) {
			if constexpr ( std::negation<std::is_trivially_destructible<T>>::value ) {
				p->~value_type ( );
			}
			deallocate ( p );
		}
	}



	template<typename T, size_t BlockSize>
	inline typename MemoryPool<T, BlockSize>::slot_pointer_
		MemoryPool<T, BlockSize>::firstSlot ( const slot_pointer_ block_ )
		const noexcept
	{
		const data_pointer_ body = reinterpret_cast< data_pointer_ >( block_ ) + sizeof ( slot_pointer_ );
		return reinterpret_cast<slot_pointer_>( body + padPointer ( body, alignof ( slot_type_ ) ) );
	}



	template<typename T, size_t BlockSize>
	inline typename MemoryPool<T, BlockSize>::slot_pointer_
		MemoryPool<T, BlockSize>::lastSlot ( const slot_pointer_ block_ )
		const noexcept
	{
		return reinterpret_cast< slot_pointer_ >( reinterpret_cast< data_pointer_ >( block_ ) + BlockSize - sizeof ( slot_type_ ) + 1 );
	}



	template<typename T, size_t BlockSize>
	typename MemoryPool<T, BlockSize>::slot_pointer_vector_
		MemoryPool<T, BlockSize>::pointerVector ( slot_pointer_ sp_ )
		const noexcept
	{
		slot_pointer_vector_ v;
		v.reserve ( BlockSize / sizeof ( slot_pointer_ ) );

		while ( sp_ != nullptr ) {

			v.push_back ( sp_ );
			sp_ = sp_->next;
		}

		return v;
	}



	template<typename T, size_t BlockSize>
	inline typename MemoryPool<T, BlockSize>::slot_pointer_vector_
		MemoryPool<T, BlockSize>::blockVector ( )
		const noexcept
	{
		return pointerVector ( currentBlock_ );
	}
}
