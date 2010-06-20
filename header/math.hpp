/*
 * math.hpp
 *  functions to use in the mathematical domain
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <stdint.h>

namespace util {
    namespace math {
        /*
         *  a function to move the decimal point n places to the right
         *  E.g.:
         *      move_decimal_point(3.141592, 1)     //   31.41592
         *      move_decimal_point(3.141592, 0)     //   3.141592
         *      move_decimal_point(3.141592, -1)    // 0.3141592
         * */
        template<typename T>
        inline T move_decimal_point(const T& src, const int32_t n) {
            return src * std::pow(10.0, n);
        }

        /*
         *  a function to round to nearest integer
         *  E.g.:
         *      round(3.14)     // 3
         *      round(-3.14)    // -3
         * */
        template<typename T>
        inline T round(const T& src) {
            return ((0 <= src) ? std::floor(src + 0.5) : std::ceil(src - 0.5));
        }

        /*
         *  a function to round to nearest even integer
         *  E.g.:
         *      round_even(3.14)    // 3
         *      round_even(0.5)     // 0.0
         *      round_even(1.5)     // 2.0
         *      round_even(-0.5)    // 0.0
         *      round_even(-1.5)    // -2.0
         * */
        template<typename T>
        inline T round_even(const T& src) {
            long double intpart;
            long double fracpart = std::modf(src, &intpart);
            if (static_cast<int64_t>(fracpart * 10) % 5 == 0) {
                return (static_cast<int64_t>(intpart) % 2 == 0)
                    ? intpart
                    : intpart + 1 * (0 <= src ? 1 : -1);
            }

            return round(src);
        }

        /*
         *  a function to round to nearest 10 ** -(n - 1) th place
         *  E.g.:
         *      round_nth(3.141592, 3)      // 3.140000
         *      round_nth(-273.15)          // -273.00
         *      round_nth(299792458, -4)    // 299800000
         * */
        template<typename T>
        inline T round_nth(const T& src, const int32_t n = 1) {
            long double s = static_cast<long double>(src);
            s = move_decimal_point(s, n - 1);
            return static_cast<T>(move_decimal_point(round(s), -(n - 1)));
        }

        /*
         *  a function to round to nearest even of 10 ** -(n - 1) th place
         *  E.g.:
         *      round_even_nth(3.141592, 3) // 3.140000
         *      round_even_nth(3.141592, 4) // 3.142000
         *      round_even_nth(3.142592, 4) // 3.142000
         * */
        template<typename T>
        inline T round_even_nth(const T& src, const int32_t n = 1) {
            long double s = static_cast<long double>(src);
            s = move_decimal_point(s, n - 1);
            return static_cast<T>(move_decimal_point(round_even(s), -(n - 1)));
        }
    }
}

#endif // MATH_HPP

