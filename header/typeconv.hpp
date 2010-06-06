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
#include <vector>

#include "string.hpp"

namespace util {
    namespace string {
        // definitions of character traits
        template<typename Char> struct char_traits {};
        template<> struct char_traits<char> {
            typedef char                char_type;
            static const char_type* null() { return ""; }
            static const char_type* space() { return " "; }
        };
        template<> struct char_traits<wchar_t> {
            typedef wchar_t             char_type;
            static const char_type* null() { return L""; }
            static const char_type* space() { return L" "; }
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

                    // some elements of any type -> std::basic_string
                    template<typename InputIterator>
                        string_type join(
                                InputIterator first, InputIterator last,
                                const string_type& delimiter = Traits::space()) {
                            if (first == last)
                                return string_type(Traits::null());

                            this->clear();
                            this->str(Traits::null());

                            *this << *first++;
                            while (first != last) *this << delimiter << *first++;
                            return this->str();
                        }

                    // std::basic_string -> some elements of any type
                    template<typename SequenceContainer>
                        SequenceContainer&
                        split(  const string_type& src,
                                SequenceContainer& result,
                                const char_type* const delimiter) {
                            unsigned int n =
                                util::string::count(src, delimiter);

                            result.resize(n + 1);
                            search_push(src, delimiter, n, result.begin());

                            return result;
                        }

                    // for std::vector
                    template<typename Value>
                        std::vector<Value>&
                        split(  const string_type& src,
                                std::vector<Value>& result,
                                const char_type* const delimiter) {
                            unsigned int n =
                                util::string::count(src, delimiter);

                            result.resize(n + 1);
                            result.reserve(n + 1);
                            search_push(src, delimiter, n, result.begin());

                            return result;
                        }

                protected:
                    // common procedure for split
                    template<typename OutputIterator>
                    void search_push(const string_type& src,
                                const char_type* const delimiter,
                                const unsigned int n,
                                OutputIterator first) {
                        typename string_type::size_type head = 0;
                        typename string_type::size_type tail;
                        for (unsigned int i = 0; i < n + 1; ++i) {
                            tail = src.find(delimiter, head);

                            this->clear();
                            this->str(src.substr(head, tail - head));
                            *this >> *first++;

                            head = tail + 1;
                        }
                    }
            };

        // for convenience
        typedef basic_typeconverter<char>    typeconverter;
        typedef basic_typeconverter<wchar_t> wtypeconverter;
    }
}

#endif // TYPECONV_HPP

