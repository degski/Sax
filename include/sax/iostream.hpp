
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

#include <iostream>
#include <string_view>
#include <type_traits>

#include <frozen/string.h>
#include <frozen/map.h>

#include "sax/utf8conv.hpp"

// Escape sequences.

inline std::ostream & nl ( std::ostream & out_ ) { return out_ << '\n'; }
inline std::wostream & nl ( std::wostream & out_ ) { return out_ << L'\n'; }

inline std::ostream & np ( std::ostream & out_ ) { return out_ << '\f'; }
inline std::wostream & np ( std::wostream & out_ ) { return out_ << L'\f'; }

inline std::ostream & cr ( std::ostream & out_ ) { return out_ << '\r'; }
inline std::wostream & cr ( std::wostream & out_ ) { return out_ << L'\r'; }

inline std::ostream & bs ( std::ostream & out_ ) { return out_ << '\b'; }
inline std::wostream & bs ( std::wostream & out_ ) { return out_ << L'\b'; }

inline std::ostream & sp ( std::ostream & out_ ) { return out_ << ' '; }
inline std::wostream & sp ( std::wostream & out_ ) { return out_ << L' '; }

inline std::ostream & ab ( std::ostream & out_ ) { return out_ << '\a'; }
inline std::wostream & ab ( std::wostream & out_ ) { return out_ << L'\a'; }

inline std::ostream & ht ( std::ostream & out_ ) { return out_ << '\t'; }
inline std::wostream & ht ( std::wostream & out_ ) { return out_ << L'\t'; }

inline std::ostream & vt ( std::ostream & out_ ) { return out_ << '\v'; }
inline std::wostream & vt ( std::wostream & out_ ) { return out_ << L'\v'; }

// Spaced dash.

inline std::ostream & sd ( std::ostream & out_ ) { return out_ << " - "; }
inline std::wostream & sd ( std::wostream & out_ ) { return out_ << L" - "; }

/*

    Name            FG   BG
    Black           30   40
    Red             31   41
    Green           32   42
    Yellow          33   43
    Blue            34   44
    Magenta         35   45
    Cyan            36   46
    White           37   47
    Bright Black    90  100
    Bright Red      91  101
    Bright Green    92  102
    Bright Yellow   93  103
    Bright Blue     94  104
    Bright Magenta  95  105
    Bright Cyan     96  106
    Bright White    97  107

*/

// Ascii colour output codes for the (Windows-) console.

namespace sax {

template<typename Char>
using ios = std::basic_ostream<Char>;

namespace detail {

struct Code {
    frozen::string foreground, background;
};

inline constexpr frozen::map<frozen::string, Code, 18> const color_map{
    { "black", { "\033[30m", "\033[40m" } },         { "red", { "\033[31m", "\033[41m" } },
    { "green", { "\033[32m", "\033[42m" } },         { "yellow", { "\033[33m", "\033[43m" } },
    { "blue", { "\033[34m", "\033[44m" } },          { "magenta", { "\033[35m", "\033[45m" } },
    { "cyan", { "\033[36m", "\033[46m" } },          { "white", { "\033[37m", "\033[47m" } },
    { "bright_black", { "\033[90m", "\033[100m" } }, { "bright_red", { "\033[91m", "\033[101m" } },
    { "bright_green", { "\033[92m", "\033[102m" } }, { "bright_yellow", { "\033[93m", "\033[103m" } },
    { "bright_blue", { "\033[94m", "\033[104m" } },  { "bright_magenta", { "\033[95m", "\033[105m" } },
    { "bright_cyan", { "\033[96m", "\033[106m" } },  { "bright_white", { "\033[97m", "\033[107m" } },
    { "default_colors", { "\033[0m", "\033[0m" } },  { "invert_colors", { "\033[7m", "\033[7m" } }
};

inline constexpr frozen::string const fg ( frozen::string const & c_ ) noexcept { return color_map.at ( c_ ).foreground; }
inline constexpr frozen::string const bg ( frozen::string const & c_ ) noexcept { return color_map.at ( c_ ).background; }

using fg_t = std::true_type;
using bg_t = std::false_type;

template<typename Char, typename Ground>
[[maybe_unused]] ios<Char> & get ( ios<Char> & out_, frozen::string const & s_ ) {
    if constexpr ( std::is_same<Ground, fg_t>::value ) {
        auto const code = detail::fg ( s_ );
        if constexpr ( std::is_same<Char, char>::value ) {
            out_ << code;
        }
        else {
            out_ << ::sax::utf8_to_utf16 ( std::basic_string_view<char> ( code.data ( ), code.size ( ) ) );
        }
    }
    else {
        auto const code = detail::bg ( s_ );
        if constexpr ( std::is_same<Char, char>::value ) {
            out_ << code;
        }
        else {
            out_ << ::sax::utf8_to_utf16 ( std::basic_string_view<char> ( code.data ( ), code.size ( ) ) );
        }
    }
    return out_;
}
} // namespace detail

namespace fg {

using namespace detail;

template<typename Char>
ios<Char> & black ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( out_, "black" );
}
template<typename Char>
ios<Char> & red ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( out_, "red" );
}
template<typename Char>
ios<Char> & green ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( out_, "green" );
}
template<typename Char>
ios<Char> & yellow ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( out_, "yellow" );
}
template<typename Char>
ios<Char> & blue ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( out_, "blue" );
}
template<typename Char>
ios<Char> & magenta ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( out_, "magenta" );
}
template<typename Char>
ios<Char> & cyan ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( out_, "cyan" );
}
template<typename Char>
ios<Char> & white ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( out_, "white" );
}
template<typename Char>
ios<Char> & bright_black ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( out_, "bright_black" );
}
template<typename Char>
ios<Char> & bright_red ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( out_, "bright_red" );
}
template<typename Char>
ios<Char> & bright_green ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( out_, "bright_green" );
}
template<typename Char>
ios<Char> & bright_yellow ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( out_, "bright_yellow" );
}
template<typename Char>
ios<Char> & bright_blue ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( out_, "bright_blue" );
}
template<typename Char>
ios<Char> & bright_magenta ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( "bright_magenta" );
}
template<typename Char>
ios<Char> & bright_cyan ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( out_, "bright_cyan" );
}
template<typename Char>
ios<Char> & bright_white ( ios<Char> & out_ ) {
    return get<Char, fg_t> ( out_, "bright_white" );
}

} // namespace fg

namespace bg {

using namespace detail;

template<typename Char>
ios<Char> & black ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "black" );
}
template<typename Char>
ios<Char> & red ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "red" );
}
template<typename Char>
ios<Char> & green ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "green" );
}
template<typename Char>
ios<Char> & yellow ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "yellow" );
}
template<typename Char>
ios<Char> & blue ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "blue" );
}
template<typename Char>
ios<Char> & magenta ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "magenta" );
}
template<typename Char>
ios<Char> & cyan ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "cyan" );
}
template<typename Char>
ios<Char> & white ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "white" );
}
template<typename Char>
ios<Char> & bright_black ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "bright_black" );
}
template<typename Char>
ios<Char> & bright_red ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "bright_red" );
}
template<typename Char>
ios<Char> & bright_green ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "bright_green" );
}
template<typename Char>
ios<Char> & bright_yellow ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "bright_yellow" );
}
template<typename Char>
ios<Char> & bright_blue ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "bright_blue" );
}
template<typename Char>
ios<Char> & bright_magenta ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "bright_magenta" );
}
template<typename Char>
ios<Char> & bright_cyan ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "bright_cyan" );
}
template<typename Char>
ios<Char> & bright_white ( ios<Char> & out_ ) {
    return get<Char, bg_t> ( out_, "bright_white" );
}

} // namespace bg

template<typename Char>
ios<Char> & reset_colors ( ios<Char> & out_ ) {
    return detail::get<Char, detail::fg_t> ( out_, "default_colors" );
}
template<typename Char>
ios<Char> & invert_colors ( ios<Char> & out_ ) {
    return detail::get<Char, detail::fg_t> ( out_, "invert_colors" );
}

} // namespace sax
