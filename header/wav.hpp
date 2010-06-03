/*
 * wav.hpp
 *  A struct to read / write headers in the form of RIFF WAV linear PCM
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 *
 * refer
 *  RIFF WAV specifications
 *      http://msdn.microsoft.com/en-us/library/ms713231.aspx
 *      http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
 * */

#ifndef WAV_HPP
#define WAV_HPP

#include <cassert>
#include <fstream>
#include <istream>
#include <ostream>
#include <stdint.h>

#include "cast.hpp"
#include "dlogger.hpp"

namespace format {
    namespace riff_wav {
        // utilitie functions
        // This function return little-endian value.
        // RIFF WAV file is written as little-endian, then we've met our needs
        // by this.
        template<const char _1, const char _2, const char _3, const char _4>
        struct quartet2uint {
            static const uint32_t value =
                static_cast<uint32_t>(_1) + (_2 << 8)
                                          + (_3 << 16)
                                          + (_4 << 24);
        };

        // basic properties of linear PCM data
        struct elements_type {
            uint16_t channels;
            uint16_t bit_depth;
            uint32_t numof_samples;
            uint32_t sampling_rate;
        };

        /*
         *  The struct for RIFF WAV header
         *  that doesn't contain actual audio data(wav samples).
         *
         *  RIFF WAV file has:
         *      RIFF chunk
         *
         *  RIFF chunk has:
         *      id          "RIFF"
         *      size        4 + sizeof(data)
         *      format-kind "WAVE"
         *      data        fmt subchunk, data subchunk
         *
         *  fmt subchunk
         *      id          "fmt "
         *      size        16
         *      data        fmt-code, channels, sampling-rate, data-per-sec,
         *                  block-size, bit-depth
         *
         *  data subchunk
         *      id          "data"
         *      size        sizeof(data)
         *      data        samples         (excluded)
         *
         *  fmt id:       linear PCM(0x01)
         *        0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
         *      +---------------+---------------+---------------+---------------+
         * 0x00 |'R' 'I' 'F' 'F'|   riff size   |'W' 'A' 'V' 'E'|'f' 'm' 't' ' '|
         *      +---------------+-------+-------+---------------+---------------+
         * 0x10 |fmt subchk size| code  |  ch   | sampling rate | data per sec  |
         *      +-------+-------+-------+-------+---------------+---------------+
         * 0x20 |blk sz |bit dep|'d' 'a' 't' 'a'|   data size   | samples ...
         *      +-------+-------+---------------+---------------+
         * */
        struct header_type {
            // member variables
            uint32_t id;
            uint32_t size;
            uint32_t format_kind;
            struct fmt_subchunk_type {
                uint32_t id;
                uint32_t size;
                struct data_type {
                    uint16_t code;
                    uint16_t channels;
                    uint32_t sampling_rate;
                    uint32_t data_per_sec;
                    uint16_t block_size;
                    uint16_t bit_depth;

                    // constants
                    enum {
                        LINEAR_PCM = 1
                    };

                    // constructor
                    data_type(void) {}
                    explicit data_type(const elements_type& p)
                        : code(LINEAR_PCM),
                          channels(p.channels),
                          sampling_rate(p.sampling_rate),
                          data_per_sec(p.sampling_rate * p.channels * (p.bit_depth / 8)),
                          block_size(p.channels * (p.bit_depth / 8)),
                          bit_depth(p.bit_depth)
                    {}

                    // utility function
                    bool validate(void) const {
                        uint32_t calculated_block_size = channels * (bit_depth / 8);
                        if (block_size != calculated_block_size) {
                            DBGLOG("There is a difference between block size"
                                    " and calculated value from a number of"
                                    " channels and bit depth: "
                                    << block_size << " != "
                                    << calculated_block_size);
                            return false;
                        }

                        uint32_t calculated_data_per_sec =
                            block_size * sampling_rate;
                        if (data_per_sec != calculated_data_per_sec) {
                            DBGLOG("There is a difference between a number of"
                                    " data per second and calculated value"
                                    " from block size and sampling rate: "
                                    << data_per_sec << " != "
                                    << calculated_data_per_sec);
                            return false;
                        }

                        return true;
                    }
                } data;

                // constants
                static const uint32_t fmt_id =
                    quartet2uint<'f', 'm', 't', ' '>::value;

                // constructor
                fmt_subchunk_type(void) {}
                explicit fmt_subchunk_type(const elements_type& p)
                    : id(fmt_id),
                      size(sizeof(data_type)),
                      data(p)
                {}

                // utility function
                bool validate(void) const {
                    if (id != fmt_id) {
                        DBGLOG("The id of fmt subchunk is incorrect: "
                                << std::hex << "0x" << id << " != "
                                << "0x" << fmt_id << std::dec);
                        return false;
                    }
                    return data.validate();
                }
            } fmt_subchunk;

            struct data_subchunk_type {
                uint32_t id;
                uint32_t size;

                // constants
                static const uint32_t data_id =
                    quartet2uint<'d', 'a', 't', 'a'>::value;

                // constructor
                data_subchunk_type(void) {}
                explicit data_subchunk_type(const elements_type& p)
                    : id(data_id),
                      size(p.numof_samples * p.channels * (p.bit_depth / 8))
                {}

                // utility function
                bool validate(void) const {
                    if (id != data_id) {
                        DBGLOG("The id of data subchunk is incorrect: "
                                << std::hex << "0x" << id << " != "
                                << "0x" << data_id << std::dec);
                        return false;
                    }

                    return true;
                }
            } data_subchunk;

            // constants
            static const uint32_t riff_id =
                quartet2uint<'R', 'I', 'F', 'F'>::value;
            static const uint32_t wave_kind =
                quartet2uint<'W', 'A', 'V', 'E'>::value;
            static const uint32_t size_offset =
                  sizeof(uint32_t)    // size of format_kind
                + sizeof(fmt_subchunk_type)
                + sizeof(data_subchunk_type);

            // constructor
            header_type(void) {}
            explicit header_type(const elements_type& p)
                : id(riff_id),
                  size(p.numof_samples * p.channels * (p.bit_depth / 8) + size_offset),
                  format_kind(wave_kind),
                  fmt_subchunk(p),
                  data_subchunk(p)
            {}

            // utility function
            bool validate(void) const {
                if (id != riff_id) {
                    DBGLOG("Not RIFF: "
                            << std::hex << "0x" << id << " != "
                            << "0x" << riff_id << std::dec);
                    return false;
                }

                if (size != data_subchunk.size + size_offset) {
                    DBGLOG("Data size of the RIFF chunk is incorrect: "
                            << size << " != "
                            << data_subchunk.size + size_offset);
                    return false;
                }

                if (format_kind != wave_kind) {
                    DBGLOG("Not WAV: "
                            << std::hex << "0x" << format_kind << " != "
                            << "0x" << wave_kind << std::dec);
                    return false;
                }

                return fmt_subchunk.validate() && data_subchunk.validate();
            }

            template<typename Char>
            inline bool
            validate(std::basic_istream<Char>& in) const {
                typename std::basic_istream<Char>::streampos current =
                    in.tellg();
                in.seekg(0, std::ios::end);
                uint32_t file_size = static_cast<uint32_t>(in.tellg());
                in.seekg(current, std::ios::beg);
                uint32_t supposed_size =
                    data_subchunk.size + sizeof(header_type);

                if (file_size != supposed_size) {
                    DBGLOG("There is a difference between the wav file size"
                            " and the size of data that is written in the"
                            " header: "
                            << file_size << " != " << supposed_size);
                    return false;
                }

                return true;
            }

            template<typename Char>
            inline bool validate(std::basic_ostream<Char>& out) const {
                typename std::basic_ostream<Char>::streampos current =
                    out.tellp();
                out.seekp(0, std::ios::end);
                uint32_t file_size = static_cast<uint32_t>(out.tellp());
                out.seekp(current, std::ios::beg);
                uint32_t supposed_size =
                    data_subchunk.size + sizeof(header_type);

                if (file_size != supposed_size) {
                    DBGLOG("There is a difference between the wav file size"
                            " and the size of data that is written in the"
                            " header: "
                            << file_size << " != " << supposed_size);
                    return false;
                }

                return true;
            }

            // getter
            // expect NRVO
            elements_type elements(void) {
                const fmt_subchunk_type::data_type& d = fmt_subchunk.data;
                elements_type e = {
                    d.channels,
                    d.bit_depth,
                    (data_subchunk.size * 8) / (d.channels * d.bit_depth),
                    d.sampling_rate
                };
                return e;
            }
        };

        /*
         *  You should execute following expressions before reading header from
         *  [i]fstream
         *
         *      in.seekg(0, std::ios::beg);
         * */
        template<typename Char>
        inline std::basic_istream<Char>&
        operator >>(std::basic_istream<Char>& in, header_type& header) {
            in.read(
                    util::cast::pointer_cast<char*>(&header),
                    sizeof(header_type));
            return in;
        }

        /*
         *  You should execute following expressions before writing header to
         *  [o]fstream
         *
         *      out.seekp(0, std::ios::beg);
         * */
        template<typename Char>
        inline std::basic_ostream<Char>&
        operator <<(std::basic_ostream<Char>& out, const header_type& header) {
            out.write(
                    util::cast::constpointer_cast<const char*>(&header),
                    sizeof(header_type));
            return out;
        }

        enum channel_type {
            // for mono
            MONO = 0,
            // for stereo
            LEFT = 0,
            RIGHT,
            // for 5.1ch
            FRONT_LEFT = 0,
            FRONT_RIGHT,
            FRONT_CENTER,
            LOW_FREQUENCY,
            BACK_LEFT,
            BACK_RIGHT
        };

        /*
         *  the class to manipulate data
         * */
        template<const unsigned int Channels, const unsigned int Byte>
        class basic_sample {
            public:
                typedef basic_sample<1, Byte>   mono_type;
                typedef char                    value8_type;
                typedef uint16_t                value16_type;
                typedef uint32_t                value24_type;
                typedef uint32_t                value32_type;

            private:
                char data[Byte * Channels];

            public:
                // constructor
                basic_sample(void) {}
                basic_sample(const char* rhs) {
                    std::copy(rhs, rhs + Byte, data);
                }

                // to get data of a channel
                mono_type channel(channel_type ch) const {
                    assert(ch < Channels);
                    return mono_type(&data[Byte * ch]);
                }

                // to get value of a channel in the form of char
                char value8(channel_type ch) const {
                    assert(ch < Channels);
                    assert(Byte == 1);
                    return data[ch];
                }
                // to get value of a channel in the form of uint16_t
                uint16_t value16(channel_type ch) const {
                    assert(ch < Channels);
                    assert(Byte == 2);
                    return *(util::cast::constpointer_cast<const uint16_t*>(
                                &data[ch * 2]));
                }
                // to get value of a channel in the form of uint32_t
                uint32_t value24(channel_type ch) const {
                    assert(ch < Channels);
                    assert(Byte == 3);
                    return *(util::cast::constpointer_cast<const uint32_t*>(
                                &data[ch * 4])) & 0x00ffffff;
                }
                // to get value of a channel in the form of uint32_t
                uint32_t value32(channel_type ch) const {
                    assert(ch < Channels);
                    assert(Byte == 4);
                    return *(util::cast::constpointer_cast<const uint32_t*>(
                                &data[ch * 4]));
                }
        };

        template<const unsigned int Channels, const unsigned int Byte>
        inline std::istream&
        operator >>(std::istream& i, basic_sample<Channels, Byte>& m) {
            i.read(util::cast::pointer_cast<char*>(&m), sizeof(m));
            return i;
        }

        template<const unsigned int Channels, const unsigned int Byte>
        inline std::ostream&
        operator <<(std::ostream& o, const basic_sample<Channels, Byte>& m) {
            o.write(util::cast::constpointer_cast<const char*>(&m), sizeof(m));
            return o;
        }
    }
}

#endif // WAV_HPP

