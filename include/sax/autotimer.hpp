
// MIT License
//
// Copyright (c) 2018, 2019 degski
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

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN_WAS_NOT_DEFINED
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN_WAS_NOT_DEFINED
#define VC_EXTRALEAN
#endif

#include <Windows.h>
#include <intrin.h>

#include <cstdio>
#include <ctime>

#include <string>


namespace at {

enum timer_precision : std::size_t { years, days, hours, minutes, seconds, milliseconds, microseconds, nanoseconds, picoseconds };
static const std::string precision_desc [ 9 ] { " years.\n", " days.\n", " hours.\n", " minutes.\n", " seconds.\n", " milliseconds.\n", " microseconds.\n", " nanoseconds.\n", " picoseconds.\n" };

class AutoTimer {

    static constexpr double r [ ] = { 1.0 / 31557600.0, 1.0 / 86400.0, 1.0 / 3600.0, 1.0 / 60.0, 1.0, 1e3, 1e6, 1e9, 1e12 };

    double period;
    std::string fs;
    timer_precision precision;

    LARGE_INTEGER begin, end;

    double * total_time = nullptr;

    [[ nodiscard ]] double query_period ( ) noexcept {
        LARGE_INTEGER performance_frequency;
        QueryPerformanceFrequency ( & performance_frequency );
        return 1.0 / static_cast<double> ( performance_frequency.QuadPart );
    }

public:

    AutoTimer ( timer_precision _p = microseconds, double * total_time_ = nullptr, std::string _fs = " %.0f" ) noexcept :
        period { query_period ( ) },
        fs { _fs + ( _fs != "" ? precision_desc [ _p ] : "" ) },
        precision { _p },
        total_time { total_time_ } {
            QueryPerformanceCounter ( & begin );
        }

    ~AutoTimer ( ) noexcept {
        const double duration = toc ( );
        if ( total_time != nullptr ) {
            *total_time += duration;
        }
        if ( not ( fs.empty ( ) ) ) {
            std::printf ( fs.c_str ( ), duration );
        }
    }

    void tic ( ) noexcept {
        QueryPerformanceCounter ( & begin );
    }

    [[ nodiscard ]] double toc ( ) noexcept {
        QueryPerformanceCounter ( & end );
        return ( static_cast<double> ( end.QuadPart - begin.QuadPart ) * period ) * r [ precision ];
    }

    void start ( ) noexcept {
        QueryPerformanceCounter ( & begin );
    }

    [[ nodiscard ]] double restart ( ) noexcept {
        QueryPerformanceCounter ( & end );
        const double d { ( static_cast<double> ( end.QuadPart - begin.QuadPart ) * period ) * r [ precision ] };
        QueryPerformanceCounter ( & begin );
        return d;
    }
};


}

#ifdef WIN32_LEAN_AND_MEAN_WAS_NOT_DEFINED
#undef WIN32_LEAN_AND_MEAN_WAS_NOT_DEFINED
#undef WIN32_LEAN_AND_MEAN
#endif
#ifdef VC_EXTRALEAN_WAS_NOT_DEFINED
#undef VC_EXTRALEAN_WAS_NOT_DEFINED
#undef VC_EXTRALEAN
#endif
