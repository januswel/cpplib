/*
 * wexcept.hpp
 *  classes to handle exceptions with const wchar_t* messages
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in the public domain, and has NO WARRANTY.
 * */

#ifndef WEXCEPT_HPP
#define WEXCEPT_HPP

#include <string>

// TODO?: Define other exceptions that are corresponding ones defined by <exception>.
namespace util {
    namespace exception {
        // base class
        class wexception {
            public:
                virtual ~wexception(void) throw() {}
                virtual const std::wstring& what(void) const throw() = 0;
        };

        // corresponding to std::logic_error
        class wlogic_error : public wexception {
            protected:
                std::wstring errmsg;

            public:
                wlogic_error(std::wstring errmsg = std::wstring(L"")) throw() : errmsg(errmsg) {}
                virtual ~wlogic_error(void) throw() {}
                virtual const std::wstring& what(void) const throw() {
                    return errmsg;
                }
        };

        // corresponding to std::runtime_error
        class wruntime_error : public wexception {
            protected:
                std::wstring errmsg;

            public:
                wruntime_error(std::wstring errmsg = std::wstring(L"")) throw() : errmsg(errmsg) {}
                virtual ~wruntime_error(void) throw() {}
                virtual const std::wstring& what(void) const throw() {
                    return errmsg;
                }
        };
    }
}

#endif // WEXCEPT_HPP

