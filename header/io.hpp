/*
 * io.hpp
 *  utility functions for input/output
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef IO_HPP
#define IO_HPP

#include <cstdio>       // for isatty(1) or _fileno(1)

#ifdef _MSC_VER
#   include <io.h>      // for _isatty(1), _setmode(2)
#   include <fcntl.h>   // for _setmode(2)
#else
#   include <unistd.h>  // for fileno(1)
#endif

namespace util {
    namespace io {
        // utilities
        inline bool is_redirected(void) {
#ifdef _MSC_VER
            return _isatty(_fileno(stdout)) == 0;
#else
            return isatty(fileno(stdout)) == 0;
#endif
        }

        inline void set_stdout_binary(void) {
#ifdef _MSC_VER
            _setmode(_fileno(stdout), _O_BINARY);
#endif
        }
    }
}

#endif // IO_HPP

