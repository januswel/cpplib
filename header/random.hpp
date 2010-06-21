/*
 * random.hpp
 *  a class to generate pseudo-random floating point numbers (double)
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <cstdlib>  // for std::srand(1) and std::rand(0)
#include <ctime>    // for std::time(1)

namespace util {
    namespace math {
        class random {
            public:
                // constructor
                random(void) {
                    static bool is_initialized = false;
                    if (!is_initialized) {
                        std::srand(static_cast<unsigned int>(std::time(NULL)));
                        is_initialized = true;
                    }

#ifdef _MSC_VER
                    // discard first one to get better value
                    // for Visual C++
                    std::rand();
#endif
                }

                // getter
                double operator()(void) {
                    return static_cast<double>(std::rand()) / RAND_MAX;
                }
        };
    }
}

#endif // RANDOM_HPP

