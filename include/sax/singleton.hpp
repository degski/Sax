
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

#include <new>
#include <utility>

namespace sax {

template<typename Derived>
struct singleton final {

    singleton ( )                       = default;
    singleton ( const singleton & )     = delete;
    singleton ( singleton && ) noexcept = delete;
    virtual ~singleton ( )              = default;

    singleton & operator= ( const singleton & ) = delete;
    singleton & operator= ( singleton && ) noexcept = delete;

    template<typename... Args>
    [[nodiscard]] static Derived & instance ( Args &&... args_ ) {
        static Derived instance ( std::forward<Args> ( args_ )... );
        return instance;
    }

    [[nodiscard]] bool is_singleton ( ) const noexcept { return this == &instance ( ); }
};

template<typename Derived>
struct thread_singleton final {

    thread_singleton ( )                              = default;
    thread_singleton ( const thread_singleton & )     = delete;
    thread_singleton ( thread_singleton && ) noexcept = delete;
    virtual ~thread_singleton ( )                     = default;

    thread_singleton & operator= ( const thread_singleton & ) = delete;
    thread_singleton & operator= ( thread_singleton && ) noexcept = delete;

    template<typename... Args>
    [[nodiscard]] static Derived & instance ( Args &&... args_ ) {
        static thread_local Derived instance ( std::forward<Args> ( args_ )... );
        return instance;
    }

    [[nodiscard]] bool is_singleton ( ) const noexcept { return this == &instance ( ); }
};

} // namespace sax
