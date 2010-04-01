/*
 * dlogger.hpp
 * The macros and classes to log for debugging
 *
 * written by janus_wel<janus.wel.3@gmail.com>
 * This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef DLOGGER_HPP
#define DLOGGER_HPP

// for visual studio environment
#ifdef _DEBUG
#   ifdef _MSC_VER
#       define DEBUG
#   endif
#endif

// typical tricks
#ifdef DEBUG
#   ifdef _MSC_VER
#       define DBGLOG(str)  (util::log::dflogger::logger()              \
                                << __FILE__ << "(" << __LINE__ << "): " \
                                << __FUNCTION__ << " : " << str << std::endl)
#       define WDBGLOG(str) (util::log::wdflogger::logger()             \
                                << __FILE__ << "(" << __LINE__ << "): " \
                                << __FUNCTION__ << " : " << str << std::endl)
#   else
#       define DBGLOG(str)  (util::log::dflogger::logger()              \
                                << __FILE__ << "(" << __LINE__ << "): " \
                                << __func__ << " : " << str << std::endl)
#       define WDBGLOG(str) (util::log::wdflogger::logger()             \
                                << __FILE__ << "(" << __LINE__ << "): " \
                                << __func__ << " : " << str << std::endl)
#   endif
#else
#   define DBGLOG(str)
#   define WDBGLOG(str)
#endif

// class definition
#ifdef DEBUG

#include <fstream>

namespace util {
    namespace log {
        // In order to be used without building object,
        // this class has only typedefs and static functions.
        template<typename charT>
            struct basic_dflogger {
                // typedefs
                typedef charT   char_t;
                typedef std::basic_ofstream<char_t>  ofstream_t;

                // static functions
                // fixed filename
                inline static const char* filename(void) { return "debug.log"; }
                inline static ofstream_t& logger(void) {
                    static ofstream_t logger(filename(), std::ios::out | std::ios::trunc);
                    return logger;
                }
            };

        // for convenience
        typedef basic_dflogger<char>     dflogger;
        typedef basic_dflogger<wchar_t>  wdflogger;
    }
}

#endif // DEBUG

#endif // DLOGGER_HPP

