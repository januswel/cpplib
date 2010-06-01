/*
 * main.cpp
 *  sample codes for wav.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <stdint.h>

#include "../../header/cast.hpp"
#include "../../header/io.hpp"
#include "../../header/wav.hpp"

template<const unsigned int Channels, const unsigned int Byte>
inline void
channel_filter( std::istream& in, std::ostream& out,
                format::riff_wav::channel_type ch) {
    typedef format::riff_wav::basic_sample<Channels, Byte> sample_type;
    std::istream_iterator<sample_type> eof, iitr(in);
    std::transform(iitr, eof,
            std::ostream_iterator<typename sample_type::mono_type>(out),
            std::bind2nd(std::mem_fun_ref(&sample_type::channel), ch));
}

int main(const int argc, const char* const argv[]) {
    if (argc < 2) {
        // creating
        format::riff_wav::elements_type elements = {
            2,      // stereo
            16,     // 16bits
            1,      // 1 set of samples
            44100   // 44.1kHz
        };
        format::riff_wav::header_type outheader(elements);
        util::io::set_stdout_binary();
        std::cout << outheader;

        // data is dummy
        std::cout
            << static_cast<char>(-128)
            << static_cast<char>(0)
            << static_cast<char>(127)
            << static_cast<char>(0);

        return 0;
    }

    // mono-ize
    // reading
    std::ifstream win(argv[1], std::ios::binary);
    if (!win.good()) {
        std::cerr
            << "bad file.\n"
            << std::endl;
        return 1;
    }

    format::riff_wav::header_type inheader;
    win >> inheader;
    if (!inheader.validate() || !inheader.validate(win)) {
        std::cerr
            << "bad wav file: " << argv[1] << "\n"
            << std::endl;
        return 1;
    }

    format::riff_wav::elements_type elements = inheader.elements();
    if (elements.channels == 1) {
        std::cerr
            << "already mono wav file: " << argv[1] << "\n"
            << std::endl;
        return 0;
    }

    util::io::set_stdout_binary();

    format::riff_wav::elements_type outelements = elements;
    outelements.channels = 1;
    format::riff_wav::header_type outheader(outelements);
    std::cout << outheader;

    switch (elements.channels) {
        using namespace format::riff_wav;
        case 2:
            switch (elements.bit_depth) {
                case 8:     channel_filter<2, 1>(win, std::cout, LEFT);
                            break;
                case 16:    channel_filter<2, 2>(win, std::cout, LEFT);
                            break;
                case 24:    channel_filter<2, 3>(win, std::cout, LEFT);
                            break;
                case 32:    channel_filter<2, 4>(win, std::cout, LEFT);
                            break;
            }
            break;
        case 6:
            switch (elements.bit_depth) {
                case 8:     channel_filter<6, 1>(win, std::cout, FRONT_LEFT);
                            break;
                case 16:    channel_filter<6, 2>(win, std::cout, FRONT_LEFT);
                            break;
                case 24:    channel_filter<6, 3>(win, std::cout, FRONT_LEFT);
                            break;
                case 32:    channel_filter<6, 4>(win, std::cout, FRONT_LEFT);
                            break;
            }
            break;
    }

    return 0;
}

