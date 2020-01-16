
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

// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences

// Ascii colour output codes for the (Windows-) console.

namespace sax {

namespace fg {

inline constexpr char const * black{ "\033[30m" };
inline constexpr char const * red{ "\033[31m" };
inline constexpr char const * green{ "\033[32m" };
inline constexpr char const * yellow{ "\033[33m" };
inline constexpr char const * blue{ "\033[34m" };
inline constexpr char const * magenta{ "\033[35m" };
inline constexpr char const * cyan{ "\033[36m" };
inline constexpr char const * white{ "\033[37m" };
inline constexpr char const * default { "\033[39m" };
inline constexpr char const * bright_black{ "\033[90m" };
inline constexpr char const * bright_red{ "\033[91m" };
inline constexpr char const * bright_green{ "\033[92m" };
inline constexpr char const * bright_yellow{ "\033[93m" };
inline constexpr char const * bright_blue{ "\033[94m" };
inline constexpr char const * bright_magenta{ "\033[95m" };
inline constexpr char const * bright_cyan{ "\033[96m" };
inline constexpr char const * bright_white{ "\033[97m" };
inline constexpr char const * bold{ "\033[1m" };
#if defined( _UNICODE ) or defined( UNICODE )
inline constexpr wchar_t const * wblack{ L"\033[30m" };
inline constexpr wchar_t const * wred{ L"\033[31m" };
inline constexpr wchar_t const * wgreen{ L"\033[32m" };
inline constexpr wchar_t const * wyellow{ L"\033[33m" };
inline constexpr wchar_t const * wblue{ L"\033[34m" };
inline constexpr wchar_t const * wmagenta{ L"\033[35m" };
inline constexpr wchar_t const * wcyan{ L"\033[36m" };
inline constexpr wchar_t const * wwhite{ L"\033[37m" };
inline constexpr wchar_t const * wdefault{ L"\033[39m" };
inline constexpr wchar_t const * wbright_black{ L"\033[90m" };
inline constexpr wchar_t const * wbright_red{ L"\033[91m" };
inline constexpr wchar_t const * wbright_green{ L"\033[92m" };
inline constexpr wchar_t const * wbright_yellow{ L"\033[93m" };
inline constexpr wchar_t const * wbright_blue{ L"\033[94m" };
inline constexpr wchar_t const * wbright_magenta{ L"\033[95m" };
inline constexpr wchar_t const * wbright_cyan{ L"\033[96m" };
inline constexpr wchar_t const * wbright_white{ L"\033[97m" };
inline constexpr wchar_t const * wbold{ L"\033[1m" };
#endif
} // namespace fg

namespace bg {

inline constexpr char const * black{ "\033[40m" };
inline constexpr char const * red{ "\033[41m" };
inline constexpr char const * green{ "\033[42m" };
inline constexpr char const * yellow{ "\033[43m" };
inline constexpr char const * blue{ "\033[44m" };
inline constexpr char const * magenta{ "\033[45m" };
inline constexpr char const * cyan{ "\033[46m" };
inline constexpr char const * white{ "\033[47m" };
inline constexpr char const * default { "\033[49m" };
inline constexpr char const * bright_black{ "\033[100m" };
inline constexpr char const * bright_red{ "\033[101m" };
inline constexpr char const * bright_green{ "\033[102m" };
inline constexpr char const * bright_yellow{ "\033[103m" };
inline constexpr char const * bright_blue{ "\033[104m" };
inline constexpr char const * bright_magenta{ "\033[105m" };
inline constexpr char const * bright_cyan{ "\033[106m" };
inline constexpr char const * bright_white{ "\033[107m" };
#if defined( _UNICODE ) or defined( UNICODE )
inline constexpr wchar_t const * wblack{ L"\033[40m" };
inline constexpr wchar_t const * wred{ L"\033[41m" };
inline constexpr wchar_t const * wgreen{ L"\033[42m" };
inline constexpr wchar_t const * wyellow{ L"\033[43m" };
inline constexpr wchar_t const * wblue{ L"\033[44m" };
inline constexpr wchar_t const * wmagenta{ L"\033[45m" };
inline constexpr wchar_t const * wcyan{ L"\033[46m" };
inline constexpr wchar_t const * wwhite{ L"\033[47m" };
inline constexpr wchar_t const * wdefault{ L"\033[49m" };
inline constexpr wchar_t const * wbright_black{ L"\033[100m" };
inline constexpr wchar_t const * wbright_red{ L"\033[101m" };
inline constexpr wchar_t const * wbright_green{ L"\033[102m" };
inline constexpr wchar_t const * wbright_yellow{ L"\033[103m" };
inline constexpr wchar_t const * wbright_blue{ L"\033[104m" };
inline constexpr wchar_t const * wbright_magenta{ L"\033[105m" };
inline constexpr wchar_t const * wbright_cyan{ L"\033[106m" };
inline constexpr wchar_t const * wbright_white{ L"\033[107m" };
#endif
} // namespace bg

inline constexpr char const * default_colors{ "\033[0m" };
inline constexpr char const * invert_colors{ "\033[7m" };
inline constexpr char const * revert_colors{ "\033[27m" };
#if defined( _UNICODE ) or defined( UNICODE )
inline constexpr wchar_t const * wdefault_colors{ L"\033[0m" };
inline constexpr wchar_t const * winvert_colors{ L"\033[7m" };
inline constexpr wchar_t const * revert_colors{ L"\033[27m" };
#endif
} // namespace sax
