/*
 * about.cpp
 *  A definition of function to give informations "about" the program
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#include "sample.hpp"

#include <ostream>

void about(std::ostream& out) {
    out
        << name() << " version " << version() << "\n"
        << "Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>\n"
        << "see LICENSE for redistributing, modifying, and so on.\n"
        << std::endl;
}

