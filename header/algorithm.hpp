/*
 * algorithm.hpp
 *  some functions to comply with the concpet of STL, like <algorithm>
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <algorithm>
#include <iterator>

namespace util {
    namespace algorithm {
        // copy_if
        template<   typename InputIterator, typename OstreamIterator,
                    typename Predicate>
            inline OstreamIterator
            copy_if(InputIterator first, InputIterator last,
                    OstreamIterator result, Predicate pred) {
                for (; first != last; ++first)
                    if (pred(*first)) *result++ = *first;
                return result;
            }

        // print
        template<typename InputIterator, typename OstreamIterator>
            inline OstreamIterator
            print(  InputIterator first, InputIterator last,
                    OstreamIterator out) {
                return std::copy(first, last, out);
            }

        // the version that the type of delimeter is const char_type* const
        template<typename Value, typename InputIterator, typename Ostream>
            inline std::ostream_iterator<Value, typename Ostream::char_type>
            print(  InputIterator first, InputIterator last, Ostream& ostream,
                    const typename Ostream::char_type* const delimeter) {
                return print(
                        first, last,
                        std::ostream_iterator<  Value,
                                                typename Ostream::char_type>(
                            ostream, delimeter));
            }

        // print_op with unary oprator
        template<   typename InputIterator, typename OstreamIterator,
                    typename UnaryOperator>
            inline OstreamIterator
            print_op(   InputIterator first, InputIterator last,
                        OstreamIterator out, UnaryOperator op) {
                return std::transform(first, last, out, op);
            }

        // the version that the type of delimeter is const char_type* const
        template<   typename Value, typename InputIterator,
                    typename Ostream, typename UnaryOperator>
            inline std::ostream_iterator<Value, typename Ostream::char_type>
            print_op(   InputIterator first, InputIterator last,
                        Ostream& ostream,
                        const typename Ostream::char_type* const delimeter,
                        UnaryOperator op) {
                return print_op(
                        first, last,
                        std::ostream_iterator<  Value,
                                                typename Ostream::char_type>(
                            ostream, delimeter), op);
            }

        // print_op with binary oprator
        template<   typename InputIterator1, typename InputIterator2,
                    typename OstreamIterator, typename BinaryOperator>
            inline OstreamIterator
            print_op(   InputIterator1 first1, InputIterator1 last1,
                        InputIterator2 first2, OstreamIterator out,
                        BinaryOperator op) {
                return std::transform(first1, last1, first2, out, op);
            }

        // the version that the type of delimeter is const char_type>* const
        template<   typename Value, typename InputIterator1,
                    typename InputIterator2, typename Ostream,
                    typename BinaryOperator>
            inline std::ostream_iterator<Value, typename Ostream::char_type>
            print_op(   InputIterator1 first1, InputIterator1 last1,
                        InputIterator2 first2, Ostream& ostream,
                        const typename Ostream::char_type* const delimeter,
                        BinaryOperator op) {
                return print_op(
                        first1, last1, first2,
                        std::ostream_iterator<  Value,
                                                typename Ostream::char_type>(
                            ostream, delimeter), op);
            }

        // print_if
        template<   typename InputIterator, typename OstreamIterator,
                    typename Predicate>
            inline OstreamIterator
            print_if(   InputIterator first, InputIterator last,
                        OstreamIterator out, Predicate pred) {
                return copy_if(first, last, out, pred);
            }

        // the version that the type of delimeter is const char_type* const
        template<   typename Value, typename InputIterator,
                    typename Ostream, typename Predicate>
            inline std::ostream_iterator<Value, typename Ostream::char_type>
            print_if(   InputIterator first, InputIterator last,
                        Ostream& ostream,
                        const typename Ostream::char_type* const delimeter,
                        Predicate pred) {
                return print_if(
                        first, last,
                        std::ostream_iterator<  Value,
                                                typename Ostream::char_type>(
                            ostream, delimeter), pred);
            }
    }
}

#endif // ALGORITHM_HPP

