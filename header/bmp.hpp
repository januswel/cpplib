/*
 * bmp.hpp
 *  The class to treat 24bit Windows Bitmap file
 *  refer: http://msdn.microsoft.com/en-us/library/dd183392(VS.85).aspx
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#ifndef BMP_HPP
#define BMP_HPP

#include <istream>
#include <ostream>
#include <stdint.h>

#include "cast.hpp"
#include "dlogger.hpp"

namespace format {
    namespace windows_bitmap {
        // utilitie functions
        // This function return little-endian value.
        // 24bit Windows Bitmap file is written as little-endian, then we've
        // met our needs by this.
        template<const char _1, const char _2>
        struct double2uint {
            static const uint32_t value =
                static_cast<uint32_t>(_1) + (_2 << 8);
        };

        // basic properties of 24bit Windows Bitmap
        struct elements_type {
            int32_t width;
            int32_t height;
        };

#pragma pack(push, 2)
        struct header_type {
            uint16_t kind;
            uint32_t file_bytes;
            uint16_t reserved01;
            uint16_t reserved02;
            uint32_t offset;
            struct info_header_type {
                uint32_t header_bytes;
                int32_t width;
                int32_t height;
                uint16_t numof_planes;
                uint16_t bits_per_pixel;
                uint32_t compression_kind;
                uint32_t image_bytes;
                int32_t horizontal_resolution;
                int32_t vertical_resolution;
                uint32_t numof_colors;
                uint32_t needed_colors;

                // enumerations
                enum compression_type {
                    NONE = 0,
                    RUN_LENGTH_8BPP,
                    RUN_LENGTH_4BPP,
                    BIT_FIELDS,
                    JPEG,
                    PNG
                };

                // constants
                static const uint32_t info_header_bytes =
                      sizeof(uint32_t)  // header_bytes
                    + sizeof(int32_t)   // width
                    + sizeof(int32_t)   // height
                    + sizeof(uint16_t)  // numof_planes
                    + sizeof(uint16_t)  // bits_per_pixel
                    + sizeof(uint32_t)  // compression_kind
                    + sizeof(uint32_t)  // image_bytes
                    + sizeof(int32_t)   // horizontal_resolution
                    + sizeof(int32_t)   // vertical_resolution
                    + sizeof(uint32_t)  // numof_colors
                    + sizeof(uint32_t); // needed_colors
                static const uint16_t one_plane = 1;
                static const uint16_t bit = 8;
                static const int32_t zero_resolution = 0;
                static const uint32_t no_use_color_palette = 0;

                // constructor
                info_header_type(void) {}
                explicit info_header_type(const elements_type& e)
                    : header_bytes(info_header_bytes),
                      width(e.width),
                      height(e.height),
                      numof_planes(one_plane),
                      bits_per_pixel(bytes_per_pixel * bit),
                      compression_kind(NONE),
                      image_bytes((e.width * bytes_per_pixel + alignment
                                  - ((e.width * bytes_per_pixel) % alignment))
                              * e.height),
                      horizontal_resolution(zero_resolution),
                      vertical_resolution(zero_resolution),
                      numof_colors(no_use_color_palette),
                      needed_colors(no_use_color_palette)
                {}

                // utility function
                bool validate(void) {
                    if (header_bytes != info_header_bytes) {
                        DBGLOG( "There is a difference between a size of the"
                                " information header and expected it"
                                << header_bytes << " != "
                                << info_header_bytes);
                        return false;
                    }

                    if (compression_kind != NONE) {
                        DBGLOG( "I don't treat compressed Windows Bitmap file:"
                                << compression_kind);
                        return false;
                    }

                    uint32_t calculated_image_bytes =
                        width * height * bits_per_pixel / bit;
                    if (image_bytes != calculated_image_bytes) {
                        DBGLOG( "A size of image data and calculated value"
                                " from width, height and bits per pixel: "
                                << image_bytes << " != "
                                << calculated_image_bytes);
                        return false;
                    }

                    if (numof_colors != no_use_color_palette) {
                        DBGLOG( "I don't treat color palette: "
                                << numof_colors);
                        return false;
                    }

                    return true;
                }
            } info_header;

            // constants
            static const uint16_t bmp_kind = double2uint<'B', 'M'>::value;
            static const uint16_t bytes_per_pixel = 3;
            static const uint16_t alignment = 4;
            static const uint16_t reserved_padding = 0;
            static const uint32_t header_all_bytes =
                  sizeof(uint16_t)          // kind
                + sizeof(uint32_t)          // file_bytes
                + sizeof(uint16_t)          // reserved01
                + sizeof(uint16_t)          // reserved02
                + sizeof(uint32_t)          // offset
                + sizeof(info_header_type);

            // constructor
            header_type(void) {}
            explicit header_type(const elements_type& e)
                : kind(bmp_kind),
                  file_bytes(header_all_bytes
                          + (e.width * bytes_per_pixel + alignment
                              - ((e.width * bytes_per_pixel) % alignment))
                          * e.height),
                  reserved01(reserved_padding),
                  reserved02(reserved_padding),
                  offset(header_all_bytes),
                  info_header(e)
            {}

            // utility function
            bool validate(void) {
                if (kind != bmp_kind) {
                    DBGLOG("The file type is not Windows Bitmap"
                            << std::hex << "0x" << kind << " != "
                            << "0x" << bmp_kind << std::dec);
                    return false;
                }

                if (offset != header_all_bytes) {
                    DBGLOG( "There is a difference between a size of the"
                            " header and expected it: "
                            << offset << " != " << header_all_bytes);
                    return false;
                }

                return true;
            }

            template<typename Char>
            bool validate(std::basic_istream<Char>& in) const {
                typename std::basic_istream<Char>::streampos current =
                    in.tellg();
                in.seekg(0, std::ios::end);
                uint32_t file_size = static_cast<uint32_t>(in.tellg());
                in.seekg(current);

                if (file_size != file_bytes) {
                    DBGLOG( "There is a difference between the bmp file size"
                            " and the size of data that is written in the"
                            " header: "
                            << file_size << " != " << file_bytes);
                    return false;
                }

                return true;
            }

            template<typename Char>
            bool validate(std::basic_ostream<Char>& out) const {
                typename std::basic_ostream<Char>::streampos current =
                    out.tellp();
                out.seekp(0, std::ios::end);
                uint32_t file_size = static_cast<uint32_t>(out.tellp());
                out.seekp(current);

                if (file_size != file_bytes) {
                    DBGLOG( "There is a difference between the bmp file size"
                            " and the size of data that is written in the"
                            " header: "
                            << file_size << " != " << file_bytes);
                    return false;
                }

                return true;
            }

            // getter
            // expect NRVO
            elements_type elements(void) {
                elements_type e = {
                    info_header.width,
                    info_header.height
                };
                return e;
            }
        };
#pragma pack(pop)

        /*
         *  You should execute following expressions before reading header from
         *  [i]fstream
         *
         *      in.seekg(0);
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
         *      out.seekp(0);
         * */
        template<typename Char>
        inline std::basic_ostream<Char>&
        operator <<(std::basic_ostream<Char>& out, const header_type& header) {
            out.write(
                    util::cast::constpointer_cast<const char*>(&header),
                    sizeof(header_type));
            return out;
        }
    }
}

#endif // BMP_HPP

