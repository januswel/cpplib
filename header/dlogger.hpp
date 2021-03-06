/*
 * dlogger.hpp
 *  the macros and classes to log for debugging
 *
 *  In order to disable, define the symbol NDEBUG:
 *
 *      > g++ -Wall --pedantic -DNDEBUG main.cpp
 *      > cl /EHsc /W4 /Za /DNDEBUG main.cpp
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#ifndef DLOGGER_HPP
#define DLOGGER_HPP

// typical tricks
#ifndef NDEBUG
#   define DBGLOG(str)  (util::log::dflogger::logger()              \
                            << __FILE__ << "(" << __LINE__ << "): " \
                            << str << std::endl)
#   define WDBGLOG(str) (util::log::wdflogger::logger()             \
                            << __FILE__ << "(" << __LINE__ << "): " \
                            << str << std::endl)
#else
#   define DBGLOG(str)
#   define WDBGLOG(str)
#endif

// class definition
#ifndef NDEBUG

#include <fstream>

namespace util {
    namespace log {
        // In order to be used without building object,
        // this class has only typedefs and static functions.
        template<typename Char>
            struct basic_dflogger {
                // typedefs
                typedef Char                            char_type;
                typedef std::basic_ofstream<char_type>  ofstream_type;

                // static functions
                // fixed filename
                static const char* filename(void) { return "debug.log"; }
                static ofstream_type& logger(void) {
                    static ofstream_type logger(filename(),
                            std::ios::out | std::ios::trunc);
                    return logger;
                }
            };

        // for convenience
        typedef basic_dflogger<char>     dflogger;
        typedef basic_dflogger<wchar_t>  wdflogger;
    }
}

#endif // NDEBUG

#endif // DLOGGER_HPP

