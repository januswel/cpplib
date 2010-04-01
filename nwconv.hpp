/*
 * nwconv.hpp
 * A class to convert between std::string and std::wstring
 *
 * written by janus_wel<janus.wel.3@gmail.com>
 * This source code is in the public domain, and has NO WARRANTY.
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
        template<typename inT, typename exT, typename statT>
            class basic_nwconv {
                private:
                    // typedefs
                    typedef std::codecvt<inT, exT, statT>   codecvt_t;
                    typedef typename codecvt_t::state_type  state_t;
                    typedef typename codecvt_t::intern_type intern_t;
                    typedef typename codecvt_t::extern_type extern_t;
                    typedef std::basic_string<intern_t>     instring_t;
                    typedef std::basic_string<extern_t>     exstring_t;

                    // cache a reference and objects
                    inline const codecvt_t& codecvt(void) const {
                        static const codecvt_t& conv = std::use_facet<codecvt_t>(loc);
                        return conv;
                    }
                    inline state_t& in_state(void) const {
                        static state_t in_stat;
                        return in_stat;
                    }
                    inline state_t& out_state(void) const {
                        static state_t out_stat;
                        return out_stat;
                    }

                    // member variables
                    std::locale loc;

                public:
                    // constructor
                    explicit basic_nwconv(std::locale loc = std::locale("")) : loc(loc) {
                        if (!std::has_facet<codecvt_t>(loc))
                            throw std::logic_error("Specified locale doesn't have codecvt facet.");
                    }

                    // narrow to wide
                    instring_t ntow(const exstring_t& src) const {
                        // preparations
                        // a size of src string
                        const std::size_t size = src.length();
                        // buffers
                        std::vector<intern_t> dst_vctr(size);
                        intern_t* const dst = &dst_vctr[0];
                        // a dummy variable and a pointer to the next position
                        const extern_t* dummy;
                        intern_t* next;
                        // cache the first position of c style characters
                        const extern_t* const s = src.c_str();

                        // do it
                        if (codecvt().in(
                                    in_state(),
                                    s,   s   + size, dummy,
                                    dst, dst + size, next) == codecvt_t::ok) {
                            return std::wstring(dst, next - dst);
                        }

                        std::string errmsg("failed: ");
                        errmsg.append(src);
                        throw std::logic_error(errmsg);
                    }

                    // wide to narrow
                    exstring_t wton(const instring_t& src) const {
                        // preparations
                        // a size of src string
                        const std::size_t src_size = src.length();
                        // calc a size of dst string
                        // This value is expected maximum.
                        const std::size_t dst_size = codecvt().max_length() * src_size;
                        // buffers
                        std::vector<extern_t> dst_vctr(dst_size);
                        extern_t* const dst = &dst_vctr[0];
                        // a dummy variable and a pointer to the next position
                        const intern_t* dummy;
                        extern_t* next;
                        // cache the first position of c style characters
                        const intern_t* const s = src.c_str();

                        // do it
                        if (codecvt().out(
                                    out_state(),
                                    s,   s   + src_size, dummy,
                                    dst, dst + dst_size, next) == codecvt_t::ok) {
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

