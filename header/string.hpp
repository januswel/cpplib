/*
 * string.hpp
 *  utility functions for std::string
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef STRING_HPP
#define STRING_HPP

#include <string>

namespace util {
    namespace string {
        // count
        template<typename Char>
            unsigned int
            count(  const std::basic_string<Char>& str,
                    const Char* const target) {
                unsigned int count = 0;
                typename std::basic_string<Char>::size_type found =
                    str.find(target);
                while (found != std::basic_string<Char>::npos) {
                    ++count;
                    found = str.find(target, found + 1);
                }
                return count;
            }
    }
}

#endif // STRING_HPP

