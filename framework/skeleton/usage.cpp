/*
 * usage.cpp
 *  A definition of function to give usage of the program
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
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

