
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

#define _AMD64_ // For SRWLock...
#include <windef.h>
#include <WinBase.h>

#include <atomic>


namespace sax {

struct SpinLock {

    SpinLock ( ) noexcept { }
    SpinLock ( const SpinLock & ) = delete;
    SpinLock & operator = ( const SpinLock & ) = delete;

    void lock ( ) noexcept {
        while ( flag.test_and_set ( std::memory_order_acquire ) );
    }
    [[ nodiscard ]] bool try_lock ( ) noexcept {
        return not ( flag.test_and_set ( std::memory_order_acquire ) );
    }
    void unlock ( ) noexcept {
        flag.clear ( std::memory_order_release );
    }

    private:

    alignas ( 64 ) std::atomic_flag flag = ATOMIC_FLAG_INIT;
};


struct SRWLock {

    SRWLock ( ) noexcept { }
    SRWLock ( const SRWLock & ) = delete;
    SRWLock & operator = ( const SRWLock & ) = delete;

    void lock ( ) noexcept {
        AcquireSRWLockExclusive ( & m_handle );
    }
    [[ nodiscard]] bool try_lock ( ) noexcept {
        return 0 != TryAcquireSRWLockExclusive ( & m_handle );
    }
    void unlock ( ) noexcept {
        ReleaseSRWLockExclusive ( & m_handle );
    } // Look at this...

    void lock_read ( ) noexcept {
        AcquireSRWLockShared ( & m_handle );
    }
    [[ nodiscard ]] bool try_lock_read ( ) noexcept {
        return 0 != TryAcquireSRWLockShared ( & m_handle );
    }
    void unlock_read ( ) noexcept {
        ReleaseSRWLockShared ( & m_handle );
    }

    private:

    alignas ( 64 ) SRWLOCK m_handle = SRWLOCK_INIT;
};

}
