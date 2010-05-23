/*
 * usage.cpp
 *  A definition of function to give usage of the program
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include "sample.hpp"

#include <ostream>

void usage(std::ostream& out) {
    out
        << "Usage:\n"
        << name() << "\n"
        << "\n"
        << "Options:\n"
        << std::endl;
}

