/*
 * main.cpp
 *  sample codes for wav.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>

#include "../../header/wav.hpp"

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
        if (!outheader.validate()) {
            std::cerr
                << "It seems that something is wrong.\n"
                << std::endl;
            return 1;
        }

        std::ofstream wout("test.wav");
        wout << outheader;
        // data is dummy
        wout
            << static_cast<char>(-128)
            << static_cast<char>(0)
            << static_cast<char>(127)
            << static_cast<char>(0);

        std::cout
            << "creating wav file succeeded: test.wav\n"
            << std::endl;

        return 0;
    }

    // reading
    std::ifstream win(argv[1]);
    format::riff_wav::header_type inheader;
    win >> inheader;
    if (!inheader.validate() || !inheader.validate(win)) {
        std::cerr << "bad wav file: " << argv[1] << std::endl;
        return 1;
    }

    // copying
    // preparing header
    format::riff_wav::elements_type elements = inheader.elements();
    elements.numof_samples = 1;
    format::riff_wav::header_type outheader(elements);
    if (!outheader.validate()) {
        std::cerr
            << "It seems that something is wrong.\n"
            << std::endl;
        return 1;
    }

    std::ofstream wout("foo.wav");

    // write header
    wout << outheader;

    // write data
    unsigned int block_size = elements.channels * elements.bit_depth / 8;
    std::vector<char> buf(block_size);
    char* buffer = &buf[0];
    for (unsigned int i = 0; i < elements.numof_samples ; ++i) {
        win.read(buffer, block_size);
        wout.write(buffer, block_size);
    }

    // validation
    if (!outheader.validate(wout)) {
        std::cerr
            << "copying failed: " << argv[1] << " -> foo.wav\n"
            << std::endl;
        return 1;
    }

    std::cout
        << "copying wav file succeeded: foo.wav\n"
        << std::endl;

    return 0;
}

