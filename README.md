
# Sax

Sax, acronymed loosely after *Swiss army x (something)* [could be a knife, a sword, a machete, a gun or a bazooka, i.e. to speak with Bjarne: "it might shoot your leg off"], which is short and distinctive and allows for the namespace `sax`. Sax is a collection of small useful (in my mind) libraries I've either written or collected over time, and can all be used independently. 

All libraries are within the `sax` namespace, also those which were not contained in a namespace initially. An example of the latter is Howard Hinnant's `short_alloc`, hence `sax::short_alloc`. The convenience header `iostream.hpp` is an exception to this rule. `iostream.hpp` allows for some type-safe short-cuts in conjunction with `std::cout` or `std::wcout` and friends and pulls in `<iostream>`.

**Important disclaimer: not all libraries are written by me and/or might contain bits of code lifted of stackoverflow.com, reddit.com/r/cpp, github.com and maybe other places.**


## Individual libraries

#### string_split (C++17)

A string splitter allowing for strings to be used as delimiters.

Just pass in a STL-string, followed by a number of delimiters (string-literals). **The order of the passed-in delimiters is important as the delimiters are applied left to right on the (sub-strings) of the previous parsing pass.**


    std::string s { "Tom and Jerry" };
    auto vector = sax::string_split ( s, "and" );

    returns a vector of strings "Tom" and "Jerry".


    Leading and trailing white-space is trimmed by default.

    std::string s { " a,      bc d and e " };
    auto vector = sax::string_split ( s, "and", ",", " " );

    returns a vector of strings "a", "bc", "d" and "e".


    To deal with tabs, pass in a "\t" as a delimiter.

    Lines of a csv-file get are easily parsed with ",", " ", "\t" as 
    delimiters, which will parse most csv-files out of the box. 


    std::string s { "Tom and, Jerry" };
    auto vector = sax::string_split ( s, "and" );

    returns a vector of strings "Tom" and ", Jerry".

    std::string s { "Tom and, Jerry" };
    auto vector = sax::string_split ( s, "and", "," );

    returns a vector of strings "Tom" and "Jerry".
    
    std::string s { "Tom and, Jerry" };
    auto vector = sax::string_split ( s, " and ", "," );
    
    returns a vector of strings "Tom and" and "Jerry".


#### uniform_int_distribution_fast (C++17)

C++17-compliant [uniform_int_distribution_fast](https://github.com/degski/uniform_int_distribution_fast), based on bounded_rand-function, as per the [paper by Daniel Lemire](https://arxiv.org/abs/1805.10941) and optimizations added to bounded_rand [published by Melissa E. O'Neill](http://www.pcg-random.org/posts/bounded-rands.html).


## License

MIT, unless the license in the individual file states differently. The library contains no (L)GPL'ed code.
