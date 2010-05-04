/*
 * typeconv.hpp
 *  a class to convert types by routing through std::string / std::wstring
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in the public domain, and has NO WARRANTY.
 * */

#ifndef TYPECONV_HPP
#define TYPECONV_HPP

#include <locale>
#include <sstream>
#include <string>

namespace util {
    namespace string {
        // definitions of character traits
        template<typename Char> struct char_traits {};
        template<> struct char_traits<char> {
            typedef char                char_type;
            static const char_type* null() { return ""; }
        };
        template<> struct char_traits<wchar_t> {
            typedef wchar_t             char_type;
            static const char_type* null() { return L""; }
        };

        // type converter
        template<typename Char, typename Traits = char_traits<Char> >
            class basic_typeconverter : public std::basic_stringstream<Char> {
                private:
                    // for convenience
                    typedef Char                            char_type;
                    typedef std::basic_string<char_type>    string_type;

                public:
                    // constructor
                    basic_typeconverter(void) {}
                    explicit basic_typeconverter(std::locale loc) {
                        this->imbue(loc);
                    }

                    // specified type -> std::basic_string
                    // This can be omitted specifying T because tye type
                    // inference always works.
                    template<typename T>
                        string_type strfrom(T src) {
                            this->clear();
                            this->str(Traits::null());

                            *this << src;
                            return this->str();
                        }

                    // std::basic_string -> specified type
                    // This must be specified T
                    template<typename T>
                        T strto(const string_type& src) {
                            this->clear();

                            this->str(src);
                            T dst;
                            *this >> dst;
                            return dst;
                        }
            };

        // for convenience
        typedef basic_typeconverter<char>    typeconverter;
        typedef basic_typeconverter<wchar_t> wtypeconverter;
    }
}

#endif // TYPECONV_HPP

