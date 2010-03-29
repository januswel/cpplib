/*
 * tconv.hpp
 * type converter by routing through std::string / std::wstring
 *
 * written by janus_wel<janus.wel.3@gmail.com>
 * This source code is in the public domain, and has NO WARRANTY.
 * */

#ifndef TCONV_HPP
#define TCONV_HPP

#include <sstream>
#include <string>

namespace util {
    namespace string {
        template<typename Ch> struct char_traits {};
        template<> struct char_traits<char> {
            typedef char              char_type;
            typedef std::string       string_type;
            typedef std::stringstream stream_type;
            static const char_type* null() { return ""; }
        };
        template<> struct char_traits<wchar_t> {
            typedef wchar_t            char_type;
            typedef std::wstring       string_type;
            typedef std::wstringstream stream_type;
            static const char_type* null() { return L""; }
        };

        template<typename Ch, typename Tr = char_traits<Ch> >
            class basic_tconv {
                private:
                    typename Tr::stream_type ss;

                public:
                    template<typename T>
                        typename Tr::string_type strfrom(T num) {
                            static const typename Tr::string_type null = Tr::null();
                            ss.clear(); ss.str(null);

                            ss << num;
                            return ss.str();
                        }

                    template<typename T>
                        T strto(const typename Tr::string_type& str) {
                            ss.clear();

                            ss.str(str);
                            T num;
                            ss >> num;
                            return num;
                        }
            };

        typedef basic_tconv<char>    tconv;
        typedef basic_tconv<wchar_t> wtconv;
    }
}

#endif // TCONV_HPP

