////////////////////////////////////////////////////////////////////////////////
//
// utf8except.hpp -- Copyright (C) by Giovanni Dicanio
// Copyright (c) 2019, 2020 degski
//      27.08.2019: changed header extension name and location.
//      27.08.2019: #pragma once
//
// This header file defines a C++ exception class that represents
// UTF-8 encoding conversion errors.
// This class is written using portable cross-platform C++ code,
// so this exception can be caught in cross-platform C++ portions of
// code (even if the throwing point is Windows-specific).
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdint.h>   // for uint32_t
#include <stdexcept>  // for std::runtime_error
#include <string>     // for std::string

namespace sax 
{


//------------------------------------------------------------------------------
// Error occurred during UTF-8 encoding conversions
//------------------------------------------------------------------------------
class Utf8ConversionException
    : public std::runtime_error
{
public:

    // Possible conversion "directions"
    enum class ConversionType
    {
        FromUtf8ToUtf16 = 0,
        FromUtf16ToUtf8
    };


    // Initialize with error message raw C-string, last Win32 error code and conversion direction
    Utf8ConversionException(const char* message, uint32_t errorCode, ConversionType type);

    // Initialize with error message string, last Win32 error code and conversion direction
    Utf8ConversionException(const std::string& message, uint32_t errorCode, ConversionType type);

    // Retrieve error code associated to the failed conversion
    uint32_t error_code() const;

    // Direction of the conversion (e.g. from UTF-8 to UTF-16)
    ConversionType direction() const;


private:
    // Error code from GetLastError()
    uint32_t _errorCode;

    // Direction of the conversion
    ConversionType _conversionType;
};


//
// Inline Method Implementations
//

inline Utf8ConversionException::Utf8ConversionException(
    const char* const message, 
    const uint32_t errorCode,
    const ConversionType type)

    : std::runtime_error(message)
    , _errorCode(errorCode)
    , _conversionType(type)
{
}


inline Utf8ConversionException::Utf8ConversionException(
    const std::string& message,
    const uint32_t errorCode,
    const ConversionType type)

    : std::runtime_error(message)
    , _errorCode(errorCode)
    , _conversionType(type)
{
}


inline uint32_t Utf8ConversionException::error_code() const
{
    return _errorCode;
}


inline Utf8ConversionException::ConversionType Utf8ConversionException::direction() const
{
    return _conversionType;
}


} // namespace sax
