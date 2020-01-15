
// MIT License
//
// Copyright (c) 2017 Matheus (https://github.com/matheuspf/ZipIter)
// Copyright (c) 2019, 2020 degski
//              - namespace detail
//              - renamed to be more in line with STL, no pascal- or camel-case
//              - #pragma once
//              - fixed warning : must specify at least one argument for '...'
//                      parameter of variadic macro
//              - [[ nodiscard ]]
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

/**
 *  @file    zipit.h
 *
 *  @brief Simple facilities to iterate through multiple containers
 *         and iterators at the same time, similar to Python's zip.
 *         Works easily with stl algorithms as well.
 */


#pragma once

#include "detail/zip.hpp"

namespace sax {
namespace detail {

/** \class zipit
  *
  * Main iterator class. Inherits from std::iterator, being of the
  * most generic std::iterator_category from all of its arguments.
  * The value type is a tuple of the value_type's of all the arguments.
  * The basic iterator interface is implemented, while some functions
  * are only alowed if all the iterator parameters meet some requirements.
  * For example, the '+' and '-' operators are defined only for random
  * access operators.
*/

template <typename T, typename... Iters>
class zipit : public help::IteratorBase<T, std::remove_reference_t< Iters >...> {
    public:

    using Base = help::IteratorBase<T, std::remove_reference_t< Iters >...>;


    /// Some type definitions defined over the std::iterator base
    using iters_type = std::tuple<T, std::remove_reference_t< Iters >...>;

    using value_type = typename Base::value_type;
    using reference = typename Base::reference;
    using difference_type = typename Base::difference_type;

    using iterator_category = typename Base::iterator_category;



    /** A single constructor. Everything else is defaulted. The tuple
      * of iterators consists of values. No modification is done to
      * the real iterators.
    */
    zipit ( T t, Iters... iterators ) : iters ( t, iterators... ) { }




    /** All the following operators simply apply a function to every member of the
      * tuple of iterators. Some of the methods are disabled if some iterator
      * does not meet all the requirements.
    */
    zipit& operator ++ ( ) {
        help::execTuple ( help::increment, iters ); return *this;
    }

    zipit operator ++ ( int ) {
        zipit temp { *this };

        operator++( );

        return temp;
    }


    template <class Tag = iterator_category, help::EnableIfMinimumTag< Tag, std::bidirectional_iterator_tag > = 0 >
    zipit& operator -- ( ) {
        help::execTuple ( help::decrement, iters ); return *this;
    }

    template <class Tag = iterator_category, help::EnableIfMinimumTag< Tag, std::bidirectional_iterator_tag > = 0 >
    zipit operator -- ( int ) {
        zipit temp { *this };

        operator--( );

        return temp;
    }


    template <class Tag = iterator_category, help::EnableIfMinimumTag< Tag, std::random_access_iterator_tag > = 0 >
    zipit& operator += ( int inc ) {
        help::execTuple ( help::add, iters, inc );

        return *this;
    }

    template <class Tag = iterator_category, help::EnableIfMinimumTag< Tag, std::random_access_iterator_tag > = 0 >
    zipit& operator -= ( int inc ) {
        help::execTuple ( help::add, iters, -inc );

        return *this;
    }



    /// Here we have non member function operators
    template <typename U, typename... Args>
    friend auto operator+ ( zipit<U, Args...>, int );

    template <typename U, typename... Args>
    friend auto operator- ( zipit<U, Args...>, int );

    template <typename U, typename... Args>
    friend auto operator+ ( const zipit<U, Args...>&, const zipit<U, Args...>& );

    template <typename U, typename... Args>
    friend auto operator- ( const zipit<U, Args...>&, const zipit<U, Args...>& );


    template <typename U, typename... Args>
    friend bool operator == ( const zipit<U, Args...>&, const zipit<U, Args...>& );

    template <typename U, typename... Args>
    friend bool operator < ( const zipit<U, Args...>&, const zipit<U, Args...>& );




    /** Delegating. I dont implement 'operator->' because the return of dereferencing
      * is a temporary, and because it is almost not used (not by any stl function I now).
    */
    decltype( auto ) operator * ( ) {
        return dereference ( std::make_index_sequence< sizeof... ( Iters ) + 1 > ( ) );
    }

    decltype( auto ) operator * ( ) const {
        return dereference ( std::make_index_sequence< sizeof... ( Iters ) + 1 > ( ) );
    }



    private:



        /// Here the tuple of value is returned as a temporary to avoid any extra extorage or access
    template <std::size_t... Is>
    auto dereference ( std::index_sequence<Is...> ) {
        return std::forward_as_tuple ( *std::get< Is > ( iters )... );
    }

    template <std::size_t... Is>
    auto dereference ( std::index_sequence<Is...> ) const {
        return std::forward_as_tuple ( *std::get< Is > ( iters )... );
    }



    /// The tuple of iterators
    iters_type iters;

};



/// 'operator+' and 'operator-' will call 'operator+=' and 'operator-=' for increment
template <typename T, typename... Iters>
inline auto operator+ ( zipit<T, Iters...> iter, int inc ) {
    iter += inc;

    return iter;
}

template <typename T, typename... Iters>
inline auto operator- ( zipit<T, Iters...> iter, int inc ) {
    iter -= inc;

    return iter;
}


/// Distance from two iterators
template <typename T, typename... Iters>
inline auto operator+ ( const zipit<T, Iters...>& iter1, const zipit<T, Iters...>& iter2 ) {
    return std::get<0> ( iter1.iters ) + std::get<0> ( iter2.iters );
}

template <typename T, typename... Iters>
inline auto operator- ( const zipit<T, Iters...>& iter1, const zipit<T, Iters...>& iter2 ) {
    return std::get<0> ( iter1.iters ) - std::get<0> ( iter2.iters );
}


/// Comparisons. Only 'operator==' and 'operator<' are defined
template <typename T, typename... Iters>
inline bool operator== ( const zipit<T, Iters...>& iter1, const zipit<T, Iters...>& iter2 ) {
    return std::get<0> ( iter1.iters ) == std::get<0> ( iter2.iters );
}

template <typename T, typename... Iters>
inline bool operator!= ( const zipit<T, Iters...>& iter1, const zipit<T, Iters...>& iter2 ) {
    return !operator==( iter1, iter2 );
}


template <typename T, typename... Iters>
inline bool operator< ( const zipit<T, Iters...>& iter1, const zipit<T, Iters...>& iter2 ) {
    return std::get<0> ( iter1.iters ) < std::get<0> ( iter2.iters );
}

template <typename T, typename... Iters>
inline bool operator> ( const zipit<T, Iters...>& iter1, const zipit<T, Iters...>& iter2 ) {
    return operator<( iter2, iter1 );
}

template <typename T, typename... Iters>
inline bool operator<= ( const zipit<T, Iters...>& iter1, const zipit<T, Iters...>& iter2 ) {
    return !operator>( iter2, iter1 );
}

template <typename T, typename... Iters>
inline bool operator>= ( const zipit<T, Iters...>& iter1, const zipit<T, Iters...>& iter2 ) {
    return !operator<( iter2, iter1 );
}









/** \class zip
  *
  * This class servers mainly as a wrapper for iterating in the for range loop.
  * It is composed of a tuple of references to containers that are iterable,
  * and exposes a 'begin' and 'end' methods, returning a 'zipit' with the
  * iterators of each container passed as argument.
*/
template <typename... Containers>
class zip {
    public:


        /// Some type definitions
    using value_type = std::tuple < Containers... >;

    using iterator = zipit < typename help::Iterable<std::remove_reference_t<Containers>>::iterator... >;

    using iterator_category = typename iterator::iterator_category;

    using const_iterator = iterator;

    static constexpr std::size_t containersSize = sizeof... ( Containers );


    /// Also a single constructor
    zip ( Containers... containers ) : containers ( containers... ) { }



    /// begin and end methods
    iterator begin ( ) { return begin ( std::make_index_sequence<containersSize> ( ) ); }

    const_iterator begin ( ) const { return begin ( std::make_index_sequence<containersSize> ( ) ); }


    iterator end ( ) { return end ( std::make_index_sequence<containersSize> ( ) ); }

    const_iterator end ( ) const { return end ( std::make_index_sequence<containersSize> ( ) ); }



    /// This is simply a facility for acessing random access containers, returning a tuple of values
    template <class Tag = iterator_category, help::EnableIfMinimumTag< Tag, std::random_access_iterator_tag > = 0 >
    auto operator [] ( std::size_t pos ) {
        return at ( pos, std::make_index_sequence<containersSize> ( ) );
    }

    template <class Tag = iterator_category, help::EnableIfMinimumTag< Tag, std::random_access_iterator_tag > = 0 >
    auto operator [] ( std::size_t pos ) const {
        return at ( pos, std::make_index_sequence<containersSize> ( ) );
    }



    /// The size of the first element defines the range
    std::size_t size ( ) const { return std::get<0> ( containers ).size ( ); }



    private:


        /// The actual implementations
    template <std::size_t... Is>
    iterator begin ( std::index_sequence<Is...> ) {
        return iterator ( help::begin ( std::get<Is> ( containers ) )... );
    }

    template <std::size_t... Is>
    const_iterator begin ( std::index_sequence<Is...> ) const {
        return const_iterator ( help::begin ( std::get<Is> ( containers ) )... );
    }

    template <std::size_t... Is>
    iterator end ( std::index_sequence<Is...> ) {
        return iterator ( help::end ( std::get<Is> ( containers ) )... );
    }

    template <std::size_t... Is>
    const_iterator end ( std::index_sequence<Is...> ) const {
        return const_iterator ( help::end ( std::get<Is> ( containers ) )... );
    }

    template <std::size_t... Is,
        class Tag = iterator_category, help::EnableIfMinimumTag< Tag, std::random_access_iterator_tag > = 0 >
        auto at ( std::size_t pos, std::index_sequence<Is...> ) {
        return std::forward_as_tuple ( containers [ Is ]... );
    }

    template <std::size_t... Is,
        class Tag = iterator_category, help::EnableIfMinimumTag< Tag, std::random_access_iterator_tag > = 0 >
        auto at ( std::size_t pos, std::index_sequence<Is...> ) const {
        return std::forward_as_tuple ( containers [ Is ]... );
    }



    /// Tuple of references to containers
    value_type containers;

};

} // namespace detail



/** These are the functions that will actually be called instead of
  * initializing the classes with cumbersome types. I used the first type
  * separatelly because it is easier to defined constraints (the first
  * element of a container cannot be a pointer) and forces the call with
  * at least 1 element
*/
template <typename T, typename... Iterators>
[[ nodiscard ]] auto zipit (T&& t, Iterators&&... iterators)
{
    return detail::zipit<T, Iterators...>(std::forward<T>(t), std::forward<Iterators>(iterators)...);
}


template <typename T, typename... Containers, std::enable_if_t< !std::is_pointer< T >::value, int > = 0>
[[ nodiscard ]] auto zip (T&& t, Containers&&... containers)
{
    return detail::zip<T, Containers...>(std::forward<T>(t), std::forward<Containers>(containers)...);
}




/** These are facilities for calling 'zipit' more easily. You can simply
  * pass a container (or a pointer) with a defined 'std::begin' or
  *'std::end' and it will call the proper function. The 'zip_all' returns
  * a std::pair containing both the begin and end of the iterators
*/
template <typename T, typename... Containers, std::enable_if_t< !std::is_pointer< T >::value, int > = 0>
[[ nodiscard ]] auto zip_begin (T&& t, Containers&&... containers)
{
    return detail::zipit(detail::help::begin(std::forward<T>(t)), detail::help::begin(std::forward<Containers>(containers))...);
}

template <typename T, typename... Containers, std::enable_if_t< !std::is_pointer< T >::value, int > = 0>
[[ nodiscard ]] auto zip_end (T&& t, Containers&&... containers)
{
    return detail::zipit(detail::help::end(std::forward<T>(t)), detail::help::end(std::forward<Containers>(containers))...);
}

template <typename T, typename... Containers, std::enable_if_t< !std::is_pointer< T >::value, int > = 0>
[[ nodiscard ]] auto zip_all (T&& t, Containers&&... containers)
{
    return std::make_pair(zip_begin(std::forward<T>(t), std::forward<Containers>(containers)...),
                          zip_end  (std::forward<T>(t), std::forward<Containers>(containers)...));
}




namespace detail {

/** \class unzip
  *
  * This class has a single method that takes variadic arguments
  * including tuples and expand all of them, passing to a function.
  * This way you can handle the elements of a tuple separatelly.
  * It is very useful while iterating in the for range loops and in
  * stl functions. Please, see the folder examples for more.
*/
template <class Apply>
struct unzip {
    /// A single constructor takin a function as argument
    unzip ( const Apply& apply = Apply ( ) ) : apply ( apply ) { }



    /// These functions expand the arguments and aplly the function, using some helpers
    template <typename... Args>
    decltype( auto ) operator () ( Args&&... args ) {
        return operator()( std::make_index_sequence< detail::help::CountElements< std::decay_t< Args >... >::value > ( ),
            std::forward< Args > ( args )... );
    }

    template <std::size_t... Is, typename... Args>
    decltype( auto ) operator () ( std::index_sequence< Is... >, Args&&... args ) {
        return apply ( std::get< Is > ( help::packArgs ( std::forward< Args > ( args )... ) )... );
    }


    /// The function to apply
    Apply apply;
};

} // namespace detail


/** As in the 'zipit' and 'zip' cases, this function is much easier
  * than to call than to instantiate the class. The first function gets
  * only a function as parameter, and is intended to use in stl functions.
  * The other two are for the for range loop, and gets a tuple as parameter
  * as well.
*/
template <class F>
[[ nodiscard ]] auto unzip (F f)
{
    return detail::unzip<F>(f);
}

template <class Tuple, class Function, std::size_t... Is>
[[ nodiscard ]] decltype(auto) unzip (Tuple&& tup, Function function, std::index_sequence<Is...>)
{
    return function( std::get< Is >( std::forward<Tuple>(tup) )... );
}

template <class Tuple, class Function>
[[ nodiscard ]] decltype(auto) unzip (Tuple&& tup, Function function)
{
    return unzip(std::forward<Tuple>(tup), function, std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>());
}




/** This function makes a call to the for loop unpacking the parameters
  * with the 'unzip' function. A thing to notice is that the function
  * is actually the first parameter of the variadic arguments. The order
  * is changed with the 'help::reverse' function.
  */
template <typename... Args>
void zip_for_each (Args&&... args)
{
    detail::help::reverse<sizeof...(Args)-1>([](auto apply, auto&&... elems)
    {
        for(auto&& tup : zip(std::forward<decltype(elems)>(elems)...))
        {
            unzip(std::forward<decltype(tup)>(tup), apply);
        }

    }, std::forward<Args>(args)...);
}


} // namespace sax
