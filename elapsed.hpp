/*
 * elapsed.hpp
 * a timer that has second precision
 *
 * written by janus_wel<janus.wel.3@gmail.com>
 * This source code is in the public domain, and has NO WARRANTY.
 * */

#ifndef ELAPSED_HPP
#define ELAPSED_HPP

#include <ctime>

namespace util {
    namespace time {
        class elapsed {
            private:
                std::time_t base;

            public:
                inline elapsed(void) { reset(); }
                inline void reset(void) { std::time(&base); }

                /*
                 *  A return type of this function is __int64
                 *  (maybe, long int), because:
                 *
                 *      - The return type of std::difftime(2) is double.
                 *      - The significand of double is represented by 52 bits.
                 *
                 *  So the smallest type that represents all values of 52 bits
                 *  is __int64.
                 *  */
                inline unsigned __int64 operator()(void) const {
                    std::time_t current;
                    std::time(&current);
                    return static_cast<unsigned __int32>(std::difftime(current, base));
                }
        };
    }
}

#endif // ELAPSED_HPP

