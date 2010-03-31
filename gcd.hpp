/*
 * The function to get Greatest Common Divisor
 *
 * written by janus_wel<janus.wel.3@gmail.com>
 * This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef GCD_HPP
#define GCD_HPP

namespace util {
    namespace math {
        // "gcd" is abbr for greatest common divisor.
        // This is Euclidean algorithm.
        template<typename T>
            unsigned int calc_gcd(T a, T b) {
                if (b == 0) return a;
                for (;;) {
                    a %= b;
                    if (a == 0) return b;
                    b %= a;
                    if (b == 0) return a;
                }
            }

        // We can't define functions for these types.
        template<> unsigned int calc_gcd<bool>(bool a, bool b);
        template<> unsigned int calc_gcd<float>(float a, float b);
        template<> unsigned int calc_gcd<double>(double a, double b);
        template<> unsigned int calc_gcd<long double>(long double a, long double b);
    }
}

#endif // GCD_HPP

