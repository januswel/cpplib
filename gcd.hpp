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
        const unsigned int calc_gcd(unsigned int a, unsigned int b) {
            if (b == 0) return a;
            for (;;) {
                a %= b;
                if (a == 0) return b;
                b %= a;
                if (b == 0) return a;
            }
        }
    }
}

#endif // GCD_HPP

