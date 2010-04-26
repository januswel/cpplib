/*
 * nwconv.hpp
 *  a class to convert between std::string and std::wstring
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in the public domain, and has NO WARRANTY.
 * */

#ifndef NWCONV_HPP
#define NWCONV_HPP

#include <locale>
#include <string>
#include <vector>
#include <stdexcept>
#include "wexcept.hpp"

namespace util {
    namespace string {
        template<typename Internal, typename External, typename State>
            class basic_nwconv {
                private:
                    // typedefs
                    typedef std::codecvt<Internal, External, State> codecvt_type;
                    typedef typename codecvt_type::state_type       state_type;
                    typedef typename codecvt_type::intern_type      intern_type;
                    typedef typename codecvt_type::extern_type      extern_type;
                    typedef std::basic_string<intern_type>          intern_string_type;
                    typedef std::basic_string<extern_type>          extern_string_type;

                    // cache a reference and objects
                    const codecvt_type& codecvt(void) const {
                        static const codecvt_type& conv = std::use_facet<codecvt_type>(loc);
                        return conv;
                    }
                    state_type& in_state(void) const {
                        static state_type in_stat;
                        return in_stat;
                    }
                    state_type& out_state(void) const {
                        static state_type out_stat;
                        return out_stat;
                    }

                    // member variables
                    std::locale loc;

                public:
                    // constructor
                    explicit basic_nwconv(std::locale loc = std::locale("")) : loc(loc) {
                        if (!std::has_facet<codecvt_type>(loc))
                            throw std::logic_error("Specified locale doesn't have codecvt facet.");
                    }

                    // narrow to wide
                    intern_string_type ntow(const extern_string_type& src) const {
                        // preparations
                        // a size of src string
                        const std::size_t size = src.length();
                        // buffers
                        std::vector<intern_type> dst_vctr(size);
                        intern_type* const dst = &dst_vctr[0];
                        // a dummy variable and a pointer to the next position
                        const extern_type* dummy;
                        intern_type* next;
                        // cache the first position of c style characters
                        const extern_type* const s = src.c_str();

                        // do it
                        if (codecvt().in(
                                    in_state(),
                                    s,   s   + size, dummy,
                                    dst, dst + size, next) == codecvt_type::ok) {
                            return std::wstring(dst, next - dst);
                        }

                        std::string errmsg("failed: ");
                        errmsg.append(src);
                        throw std::logic_error(errmsg);
                    }

                    // wide to narrow
                    extern_string_type wton(const intern_string_type& src) const {
                        // preparations
                        // a size of src string
                        const std::size_t src_size = src.length();
                        // calc a size of dst string
                        // This value is expected maximum.
                        const std::size_t dst_size = codecvt().max_length() * src_size;
                        // buffers
                        std::vector<extern_type> dst_vctr(dst_size);
                        extern_type* const dst = &dst_vctr[0];
                        // a dummy variable and a pointer to the next position
                        const intern_type* dummy;
                        extern_type* next;
                        // cache the first position of c style characters
                        const intern_type* const s = src.c_str();

                        // do it
                        if (codecvt().out(
                                    out_state(),
                                    s,   s   + src_size, dummy,
                                    dst, dst + dst_size, next) == codecvt_type::ok) {
                            return std::string(dst, next - dst);
                        }

                        std::wstring errmsg(L"failed: ");
                        errmsg.append(src);
                        throw util::exception::wlogic_error(errmsg);
                    }
            };

        // for convenience
        typedef basic_nwconv<wchar_t, char, mbstate_t> nwconv;
    }
}

#endif // NWCONV_HPP

