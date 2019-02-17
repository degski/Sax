
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

#include <iostream>

// Escape sequences.

std::ostream &  nl ( std::ostream  & out_ ) { return out_ <<  '\n'; }
std::wostream & nl ( std::wostream & out_ ) { return out_ << L'\n'; }

std::ostream &  np ( std::ostream  & out_ ) { return out_ <<  '\f'; }
std::wostream & np ( std::wostream & out_ ) { return out_ << L'\f'; }

std::ostream &  cr ( std::ostream  & out_ ) { return out_ <<  '\r'; }
std::wostream & cr ( std::wostream & out_ ) { return out_ << L'\r'; }

std::ostream &  bs ( std::ostream  & out_ ) { return out_ <<  '\b'; }
std::wostream & bs ( std::wostream & out_ ) { return out_ << L'\b'; }

std::ostream &  sp ( std::ostream  & out_ ) { return out_ <<  ' '; }
std::wostream & sp ( std::wostream & out_ ) { return out_ << L' '; }

std::ostream &  ab ( std::ostream  & out_ ) { return out_ <<  '\a'; }
std::wostream & ab ( std::wostream & out_ ) { return out_ << L'\a'; }

std::ostream &  ht ( std::ostream  & out_ ) { return out_ <<  '\t'; }
std::wostream & ht ( std::wostream & out_ ) { return out_ << L'\t'; }

std::ostream &  vt ( std::ostream  & out_ ) { return out_ <<  '\v'; }
std::wostream & vt ( std::wostream & out_ ) { return out_ << L'\v'; }

// Spaced dash.

std::ostream &  sd ( std::ostream  & out_ ) { return out_ <<  " - "; }
std::wostream & sd ( std::wostream & out_ ) { return out_ << L" - "; }
