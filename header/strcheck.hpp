/*
 * strcheck.hpp
 *  The class to check if strings are integer, decimal, or real number
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#ifndef STRCHECK_HPP
#define STRCHECK_HPP

#include <cstring>
#include <locale>
#include <string>

namespace util {
    namespace string {
        template<typename Char> struct numeric_traits;
        template<> struct numeric_traits<char> {
            typedef char    char_type;
            static const char_type positive = '+';
            static const char_type negative = '-';
        };
        template<> struct numeric_traits<wchar_t> {
            typedef wchar_t char_type;
            static const char_type positive = L'+';
            static const char_type negative = L'-';
        };

        template<   typename Char,
                    typename CharTraits = std::char_traits<Char>,
                    typename NumTraits = numeric_traits<Char> >
        class basic_check {
            public:
                typedef Char                            char_type;
                typedef std::basic_string<char_type>    string_type;
                typedef std::ctype<char_type>           ctype_type;
                typedef std::numpunct<char_type>        numpunct_type;

            private:
                std::locale loc;

            public:
                // constructor
                explicit basic_check(void) : loc(std::locale()) {}
                explicit basic_check(const std::locale& loc) : loc(loc) {}

            private:
                const ctype_type& ctype(void) const {
                    static const ctype_type& ctype =
                        std::use_facet<ctype_type>(loc);
                    return ctype;
                }

                char_type decimal_point(void) const {
                    static const char_type decimal_point =
                        std::use_facet<numpunct_type>(loc).decimal_point();
                    return decimal_point;
                }

            public:
                bool is_positive(const char_type* const str) const {
                    return !is_negative(str);
                }

                bool is_positive(const string_type& str) const {
                    return is_positive(str.c_str());
                }

                bool is_negative(const char_type* const str) const {
                    return str[0] == NumTraits::negative;
                }

                bool is_negative(const string_type& str) const {
                    return is_negative(str.c_str());
                }

                bool is_integer(const char_type* const str) const {
                    const char_type* const first =
                        (  (str[0] == NumTraits::positive)
                         | (str[0] == NumTraits::negative)) ? str + 1 : str;
                    const char_type* const last =
                        first + CharTraits::length(first);
                    const char_type* const found =
                        ctype().scan_not(ctype_type::digit, first, last);

                    return found == last;
                }

                bool is_integer(const string_type& str) const {
                    return is_integer(str.c_str());
                }

                bool is_decimal(const char_type* const str) const {
                    const char_type* const first =
                        (  (str[0] == NumTraits::positive)
                         | (str[0] == NumTraits::negative)) ? str + 1 : str;

                    const char_type* const point =
                        CharTraits::find(   first,
                                            CharTraits::length(first),
                                            decimal_point());

                    if (point == 0 || point == first) return false;

                    const char_type* integer =
                        ctype().scan_not(ctype_type::digit, first, point);
                    return ((integer == point) & is_integer(point + 1));
                }

                bool is_decimal(const string_type& str) const {
                    return is_decimal(str.c_str());
                }

                bool is_real(const char_type* const str) const {
                    return (is_integer(str) | is_decimal(str));
                }

                bool is_real(const string_type& str) const {
                    return is_real(str.c_str());
                }
        };

        typedef basic_check<char>       check;
        typedef basic_check<wchar_t>    wcheck;
    }
}

#endif // STRCHECK_HPP

