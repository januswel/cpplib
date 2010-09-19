/*
 * file.hpp
 *  utility functions for file operations
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef FILE_HPP
#define FILE_HPP

#include <istream>

namespace util {
    namespace file {
        template<typename Char>
        bool is_file(std::basic_istream<Char>& in) {
            return (in.tellg() < 0);
        }

        template<typename Char>
        std::streampos size(std::basic_istream<Char>& in) {
            std::streampos current = in.tellg();
            if (in.tellg() < 0) return current;

            in.seekg(0, std::ios_base::end);
            std::streampos size = in.tellg();
            in.seekg(current);

            return size;
        }
    }
}

#endif // FILE_HPP

