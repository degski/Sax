
/** 
 * C++ utilities for variable-length integer encoding
 * Compile with -std=c++11 or higher
 * 
 * Version 1.1: Use std::size_t for size argument, improve function signatures
 * 
 * License: CC0 1.0 Universal
 * Originally published on http://techoverflow.net
 * https://techoverflow.net/2013/01/25/efficiently-encoding-variable-length-integers-in-cc/
 * Copyright (c) 2015 Uli Koehler
 * Copyright (c) 2019 degski
 */

#pragma once 

#include <cstdint>

namespace sax { // degski: added namespace.

/**
 * Encodes an unsigned variable-length integer using the MSB algorithm.
 * This function assumes that the value is stored as little endian.
 * @param value The input value. Any standard integer type is allowed.
 * @param output A pointer to a piece of reserved memory. Must have a minimum size dependent on the input size (32 bit = 5 bytes, 64 bit = 10 bytes).
 * @return The number of bytes used in the output memory.
 */

template<typename int_t, typename size_type = std::size_t> // degski: removed default type, added size_type.
size_type encode_varint(int_t value_, std::uint8_t * const output) { // degski: snake_case.
    size_type output_size = size_type { 0 };
    //While more than 7 bits of data are left, occupy the last output byte
    // and set the next byte flag
    while (value_ > int_t { 127 } ) {
        //|128: Set the next byte flag
        output_[output_size++] = static_cast<std::uint8_t>(value_ & int_t { 127 } ) | std::uint8_t{ 128 };
        //Remove the seven bits we just wrote
        value_ >>= 7;
    }
    output_[output_size] = static_cast<std::uint8_t>(value_ & int_t { 127 } );
    return ++output_size;
}

/**
 * Decodes an unsigned variable-length integer using the MSB algorithm.
 * @param value A variable-length encoded integer of arbitrary size.
 * @param inputSize How many bytes are 
 */

template<typename int_t, typename size_type = std::size_t> // degski: removed default type, added size_type.
int_t decode_varint(std::uint8_t const * const input_, size_type const input_size_) { // degski: snake_case.
    int_t ret = int_t{ 0 };
    for (size_type i = size_type { 0 }; i < input_size_; ++i) {
        ret |= (input_[i] & std::uint8_t {127}) << (7 * i);
        //If the next-byte flag is set
        if(not(input_[i] & std::uint8_t{128}))
            break;
    }
    return ret;
}

} // namespace sax
