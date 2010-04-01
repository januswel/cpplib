/*
 * wexcept.hpp
 * Classes to handle exceptions with const wchar_t* messages
 *
 * written by janus_wel<janus.wel.3@gmail.com>
 * This source code is in the public domain, and has NO WARRANTY.
 * */

#ifndef WEXCEPT_HPP
#define WEXCEPT_HPP

#include <exception>
#include <string>

namespace util {
    namespace exception {
        class wexception {
            public:
                virtual ~wexception(void) throw() {}
                virtual const wchar_t* what(void) const throw() = 0;
        };

        class wlogic_error : public wexception {
            protected:
                std::wstring errmsg;

            public:
                wlogic_error(std::wstring errmsg = std::wstring(L"")) throw() : errmsg(errmsg) {}
                virtual ~wlogic_error(void) throw() {}
                virtual const wchar_t* what(void) const throw() {
                    return errmsg.c_str();
                }
        };

        class wruntime_error : public wexception {
            protected:
                std::wstring errmsg;

            public:
                wruntime_error(std::wstring errmsg = std::wstring(L"")) throw() : errmsg(errmsg) {}
                virtual ~wruntime_error(void) throw() {}
                virtual const wchar_t* what(void) const throw() {
                    return errmsg.c_str();
                }
        };
    }
}

#endif // WEXCEPT_HPP

