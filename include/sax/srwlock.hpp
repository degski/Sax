
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

#define _AMD64_ // For SRWLock...
#include <windef.h>
#include <WinBase.h>


namespace sax {

template < bool L = false >
struct SRWLock {

};

template < >
struct SRWLock < true > {

	void lock ( ) noexcept { AcquireSRWLockExclusive ( &srwlock_handle ); }
	bool tryLock ( ) noexcept { return TryAcquireSRWLockExclusive ( &srwlock_handle ) != 0; }
	void unlock ( ) noexcept { ReleaseSRWLockExclusive ( &srwlock_handle ); } // Look at this...

	void lockRead ( ) noexcept { AcquireSRWLockShared ( &srwlock_handle ); }
	bool tryLockRead ( ) noexcept { return TryAcquireSRWLockShared ( &srwlock_handle ) != 0; }
	void unlockRead ( ) noexcept { ReleaseSRWLockShared ( &srwlock_handle ); }

	SRWLock ( ) noexcept : srwlock_handle ( SRWLOCK_INIT ) { }
	~SRWLock ( ) noexcept { tryLockRead ( ) ? unlockRead ( ) : unlock ( ); }

	SRWLock < true > & operator = ( const SRWLock < true > & rhs_ ) { return * this; }

private:

	SRWLOCK srwlock_handle;
};

template < >
struct SRWLock < false > {

	void lock ( ) const noexcept { }
	bool tryLock ( ) const noexcept { return true; }
	void unlock ( ) const noexcept { }

	void lockRead ( ) const noexcept { }
	bool tryLockRead ( ) const noexcept { return true; }
	void unlockRead ( ) const noexcept { }

	SRWLock < false > & operator = ( const SRWLock < false > & rhs_ ) { return * this; }
};

}
