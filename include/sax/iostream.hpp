
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

#include <frozen/string.h>
#include <frozen/map.h>

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

// TODO: Wide streams.

namespace sax {

namespace detail {

struct Code {
    frozen::string foreground, background;
};

inline constexpr frozen::map<frozen::string, Code, 17> const color_map{
    { "black", { "\033[30m", "\033[40m" } },         { "red", { "\033[31m", "\033[41m" } },
    { "green", { "\033[32m", "\033[42m" } },         { "yellow", { "\033[33m", "\033[43m" } },
    { "blue", { "\033[34m", "\033[44m" } },          { "magenta", { "\033[35m", "\033[45m" } },
    { "cyan", { "\033[36m", "\033[46m" } },          { "white", { "\033[37m", "\033[47m" } },
    { "bright_black", { "\033[90m", "\033[100m" } }, { "bright_red", { "\033[91m", "\033[101m" } },
    { "bright_green", { "\033[92m", "\033[102m" } }, { "bright_yellow", { "\033[93m", "\033[103m" } },
    { "bright_blue", { "\033[94m", "\033[104m" } },  { "bright_magenta", { "\033[95m", "\033[105m" } },
    { "bright_cyan", { "\033[96m", "\033[106m" } },  { "bright_white", { "\033[97m", "\033[107m" } },
    { "default_colors", { "\033[0m", "\033[0m" } }
};

inline constexpr char const * fg ( frozen::string const & c_ ) noexcept { return color_map.at ( c_ ).foreground.data ( ); }
inline constexpr char const * bg ( frozen::string const & c_ ) noexcept { return color_map.at ( c_ ).background.data ( ); }

} // namespace detail

namespace fg {

inline std::ostream & black ( std::ostream & out_ ) { return out_ << detail::fg ( "black" ); }
inline std::ostream & red ( std::ostream & out_ ) { return out_ << detail::fg ( "red" ); }
inline std::ostream & green ( std::ostream & out_ ) { return out_ << detail::fg ( "green" ); }
inline std::ostream & yellow ( std::ostream & out_ ) { return out_ << detail::fg ( "yellow" ); }
inline std::ostream & blue ( std::ostream & out_ ) { return out_ << detail::fg ( "blue" ); }
inline std::ostream & magenta ( std::ostream & out_ ) { return out_ << detail::fg ( "magenta" ); }
inline std::ostream & cyan ( std::ostream & out_ ) { return out_ << detail::fg ( "cyan" ); }
inline std::ostream & white ( std::ostream & out_ ) { return out_ << detail::fg ( "white" ); }
inline std::ostream & bright_black ( std::ostream & out_ ) { return out_ << detail::fg ( "bright_black" ); }
inline std::ostream & bright_red ( std::ostream & out_ ) { return out_ << detail::fg ( "bright_red" ); }
inline std::ostream & bright_green ( std::ostream & out_ ) { return out_ << detail::fg ( "bright_green" ); }
inline std::ostream & bright_yellow ( std::ostream & out_ ) { return out_ << detail::fg ( "bright_yellow" ); }
inline std::ostream & bright_blue ( std::ostream & out_ ) { return out_ << detail::fg ( "bright_blue" ); }
inline std::ostream & bright_magenta ( std::ostream & out_ ) { return out_ << detail::fg ( "bright_magenta" ); }
inline std::ostream & bright_cyan ( std::ostream & out_ ) { return out_ << detail::fg ( "bright_cyan" ); }
inline std::ostream & bright_white ( std::ostream & out_ ) { return out_ << detail::fg ( "bright_white" ); }

} // namespace fg

namespace bg {

inline std::ostream & black ( std::ostream & out_ ) { return out_ << detail::bg ( "black" ); }
inline std::ostream & red ( std::ostream & out_ ) { return out_ << detail::bg ( "red" ); }
inline std::ostream & green ( std::ostream & out_ ) { return out_ << detail::bg ( "green" ); }
inline std::ostream & yellow ( std::ostream & out_ ) { return out_ << detail::bg ( "yellow" ); }
inline std::ostream & blue ( std::ostream & out_ ) { return out_ << detail::bg ( "blue" ); }
inline std::ostream & magenta ( std::ostream & out_ ) { return out_ << detail::bg ( "magenta" ); }
inline std::ostream & cyan ( std::ostream & out_ ) { return out_ << detail::bg ( "cyan" ); }
inline std::ostream & white ( std::ostream & out_ ) { return out_ << detail::bg ( "white" ); }
inline std::ostream & bright_black ( std::ostream & out_ ) { return out_ << detail::bg ( "bright_black" ); }
inline std::ostream & bright_red ( std::ostream & out_ ) { return out_ << detail::bg ( "bright_red" ); }
inline std::ostream & bright_green ( std::ostream & out_ ) { return out_ << detail::bg ( "bright_green" ); }
inline std::ostream & bright_yellow ( std::ostream & out_ ) { return out_ << detail::bg ( "bright_yellow" ); }
inline std::ostream & bright_blue ( std::ostream & out_ ) { return out_ << detail::bg ( "bright_blue" ); }
inline std::ostream & bright_magenta ( std::ostream & out_ ) { return out_ << detail::bg ( "bright_magenta" ); }
inline std::ostream & bright_cyan ( std::ostream & out_ ) { return out_ << detail::bg ( "bright_cyan" ); }
inline std::ostream & bright_white ( std::ostream & out_ ) { return out_ << detail::bg ( "bright_white" ); }

} // namespace bg

inline std::ostream & reset_colors ( std::ostream & out_ ) {
    return out_ << detail::color_map.at ( "default_colors" ).foreground.data ( );
}

} // namespace sax
