
# Sax

Sax, acronymed loosely after Swiss army something (x). This is a collection of small libraries, which can all be used independently. Disclaimer: not all libraries are written by me or might contain bits of code lifted of stackoverflow.com.


## Individual libraries

#### string_split (C++17)

A string splitter allowing for strings as delimiters.

Pass in a STL-string, followed by the delimiters. The order of the passed in delimiters is important as the delimiters are applied left to right on the (sub-strings) of the previous parsing pass.


    std::string s { "Tom and Jerry" };
    auto vector = sax::string_split ( s, "and" );

    returns a vector of strings "Tom" and "Jerry".

    Leading and trailing white-space is trimmed by default.

    std::string s { " a,      bc d and e " };
    auto vector = sax::string_split ( s, "and", ",", " " );

    returns a vector of strings "a", "bc", "d" and "e".

    To deal with tabs, pass in a "\t" as a delimiter.

    Lines of a csv-file get are easily parsed with ",", " ", "\t" as delimiters, which will parse most csv-files out of the box. 

    std::string s { "Tom and, Jerry" };
    auto vector = sax::string_split ( s, "and" );

    returns a vector of strings "Tom" and ", Jerry".

    std::string s { "Tom and, Jerry" };
    auto vector = sax::string_split ( s, "and", "," );

    returns a vector of strings "Tom" and "Jerry".
    
    std::string s { "Tom and, Jerry" };
    auto vector = sax::string_split ( s, " and ", "," );
    
    returns a vector of strings "Tom and" and "Jerry".


## License

MIT, unless the license in the individual file states differently.
