/*
 * about.cpp
 *  A definition of function to give informations "about" the program
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include "sample.hpp"

#include <ostream>

void about(std::ostream& out) {
    out
        << name() << " version " << version() << "\n"
        << "written by janus_wel<janus.wel.3@gmail.com>\n"
        << "This program is in public domain, and has NO WARRANTY\n"
        << std::endl;
}

