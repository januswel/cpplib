/*
 * nwconv.hpp
 * convert between std::string and std::wstring
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
#include "../../header/wexcept.hpp"

namespace util {
    namespace string {
        template<typename inT, typename exT, typename statT>
            class basic_nwconv {
                private:
                    typedef std::codecvt<inT, exT, statT>   codecvt_t;
                    typedef typename codecvt_t::state_type  state_t;
                    typedef typename codecvt_t::intern_type intern_t;
                    typedef typename codecvt_t::extern_type extern_t;

                    std::locale loc;

                    const codecvt_t& get_codecvt(void) {
                        static const codecvt_t& conv = std::use_facet<codecvt_t>(loc);
                        return conv;
                    }
                    state_t& get_in_state(void) {
                        static state_t in_stat;
                        return in_stat;
                    }
                    state_t& get_out_state(void) {
                        static state_t out_stat;
                        return out_stat;
                    }

                public:
                    basic_nwconv(std::locale loc = std::locale("")) : loc(loc) {
                        if (!std::has_facet<codecvt_t>(loc))
                            throw std::logic_error("Specified locale doesn't have codecvt facet.");
                    }

                    // narrow to wide
                    std::basic_string<intern_t> ntow(const std::basic_string<extern_t>& src) {
                        const std::size_t size = src.length();

                        std::vector<intern_t> dst_vctr(size);
                        intern_t* const dst = &dst_vctr[0];

                        const extern_t* dummy;
                        intern_t* next;

                        const extern_t* const s = src.c_str();

                        if (get_codecvt().in(
                                    get_in_state(),
                                    s,   s   + size, dummy,
                                    dst, dst + size, next) == codecvt_t::ok) {
                            return std::wstring(dst, next - dst);
                        }

                        std::string errmsg("failed: ");
                        errmsg.append(src);
                        throw std::logic_error(errmsg);
                    }

                    // wide to narrow
                    std::basic_string<extern_t> wton(const std::basic_string<intern_t>& src) {
                        const std::size_t src_size = src.length();
                        const std::size_t dst_size = get_codecvt().max_length() * src_size;

                        std::vector<extern_t> dst_vctr(dst_size);
                        extern_t* const dst = &dst_vctr[0];

                        const intern_t* dummy;
                        extern_t* next;

                        const intern_t* const s = src.c_str();

                        if (get_codecvt().out(
                                    get_out_state(),
                                    s,   s   + src_size, dummy,
                                    dst, dst + dst_size, next) == codecvt_t::ok) {
                            return std::string(dst, next - dst);
                        }

                        std::wstring errmsg(L"failed: ");
                        errmsg.append(src);
                        throw util::exception::wlogic_error(errmsg);
                    }
            };

        typedef basic_nwconv<wchar_t, char, mbstate_t> nwconv;
    }
}

#endif // NWCONV_HPP

