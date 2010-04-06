/*
 * tconv.hpp
 *  a class to convert types by routing through std::string / std::wstring
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in the public domain, and has NO WARRANTY.
 * */

#ifndef TCONV_HPP
#define TCONV_HPP

#include <locale>
#include <sstream>
#include <string>

namespace util {
    namespace string {
        // definitions of character traits
        template<typename Ch> struct char_traits {};
        template<> struct char_traits<char> {
            typedef char                char_t;
            typedef std::string         string_t;
            typedef std::stringstream   stream_t;
            static const char_t* null() { return ""; }
        };
        template<> struct char_traits<wchar_t> {
            typedef wchar_t             char_t;
            typedef std::wstring        string_t;
            typedef std::wstringstream  stream_t;
            static const char_t* null() { return L""; }
        };

        // type converter
        template<typename Ch, typename Tr = char_traits<Ch> >
            class basic_tconv {
                private:
                    // for convenience
                    typedef typename Tr::char_t     char_t;
                    typedef typename Tr::string_t   string_t;
                    typedef typename Tr::stream_t   stream_t;

                    // member variables
                    stream_t ss;

                public:
                    // constructor
                    basic_tconv(void) {}
                    explicit basic_tconv(std::locale loc) { ss.imbue(loc); }

                    // specified type -> std::basic_string
                    // This can be omitted specifying T because tye type
                    // inference always works.
                    template<typename T>
                        string_t strfrom(T num) {
                            ss.clear(); ss.str(Tr::null());

                            ss << num;
                            return ss.str();
                        }

                    // std::basic_string -> specified type
                    // This must be specified T
                    template<typename T>
                        T strto(const string_t& str) {
                            ss.clear();

                            ss.str(str);
                            T num;
                            ss >> num;
                            return num;
                        }
            };

        // for convenience
        typedef basic_tconv<char>    tconv;
        typedef basic_tconv<wchar_t> wtconv;
    }
}

#endif // TCONV_HPP

