/*
 * endian.hpp
 * Some functions for handling endian.
 *
 * written by janus_wel<janus.wel.3@gmail.com>
 * This source code is NOT MY COPYRIGHTED WORK, and has NO WARRANTY.
 *
 * Refer - a source of an idea:
 *  http://www.kijineko.co.jp/tech/cpptempls/endian
 * */

#ifndef ENDIAN_H
#define ENDIAN_H

#include <algorithm>
#include "cast.hpp"

namespace util {
    namespace endian {
        inline bool is_le() {
            // in a 32-bit machine, the representation of 1 is:
            //
            //      little endian: 01 00 00 00
            //      big endian:    00 00 00 01
            //
            // therefore the machine is little endian
            // if the first byte is 1 (actually 01).
            int t = 1;
            return *(util::cast::pointer_cast<char*>(&t)) == 1;
        }
        inline bool is_be() { return !is_le(); }

        // this is destructive but fast
        template<typename T> void fast_reverse(T& value) {
            char volatile* first = util::cast::pointer_cast<char volatile*>(&value);
            char volatile* last = first + sizeof(T);
            std::reverse(first, last);
        }

        // this is slow but nondestructive
        template<typename T> T reverse(const T& value) {
            T temp = value;
            char volatile* first = util::cast::pointer_cast<char volatile*>(&temp);
            char volatile* last = first + sizeof(T);
            std::reverse(first, last);
            return temp;
        }
    }
}

#endif // ENDIAN_H

