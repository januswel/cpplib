/*
 * main.cpp
 *  Definitions for main flow of the program
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#include "main.hpp"

#include <iomanip>
#include <iostream>
#include <locale>
#include <stdexcept>

int Main::main(void) {
    if (input.empty()) {
        throw sample_error(BAD_ARGUMENT, "specify input\n");
    }

    std::cout << std::left << std::boolalpha
        << std::setw(10) << "size"  << size << "\n"
        << std::setw(10) << "input" << input << "\n"
        << std::endl;

    return OK;
}

int main(const int argc, const char* const argv[]) {
    try {
        std::locale::global(std::locale(""));
        Main main;
        main.analyze_option(argc, argv);
        main.preparation();
        return main.start();
    }
    catch (const sample_error& ex) {
        std::cerr << ex.what() << std::endl;
        if (ex.return_value() == BAD_ARGUMENT) usage(std::cerr);
        return ex.return_value();
    }
    catch (const std::exception& ex) {
        std::cerr << "error: " << ex.what() << std::endl;
        return UNKNOWN;
    }
}

