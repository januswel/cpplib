/*
 * main.cpp
 *  print the informations of some codecvt facets
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include <iomanip>
#include <iostream>
#include <locale>

using namespace std;

ostream&
operator<<(ostream& o, const locale& loc) {
    typedef codecvt<wchar_t, char, mbstate_t> codecvt_type;

    ostream out(o.rdbuf());
    out << setfill('.') << left;

    out << loc.name() << "\n";

    if (has_facet<codecvt_type>(loc)) {
        const codecvt_type& codecvt = use_facet<codecvt_type>(loc);
        string encoding;
        switch (codecvt.encoding()) {
            case -1: encoding = " - state-dependent"; break;
            case 0:  encoding = " - variable width"; break;
        }
        out
            << setw(20) << "always_noconv() " << ' '
                << codecvt.always_noconv() << "\n"
            << setw(20) << "encoding() "      << ' '
                << codecvt.encoding() << encoding << "\n"
            << setw(20) << "max_length() "    << ' '
                << codecvt.max_length() << "\n";

        if (       codecvt.encoding() > 0
                && codecvt.encoding() != codecvt.max_length()) {
            out
                << "It seems wrong with this locale: \n"
                << "The number of bytes for a character is fixed but there\n"
                << "is a difference between it and the max length of a\n"
                << "character.\n";
        }
    }
    else {
        out << "doesn't have codecvt facet." << "\n";
    }

    return o;
}

int main(void) {
    locale initial_locale  = locale();
    locale system_locale   = locale("");
    locale classic_locale  = locale::classic();

    cout
        << "initial locale: " << initial_locale << "\n"
        << "system locale: "  << system_locale << "\n"
        << "classic locale: " << classic_locale << "\n"
        << endl;

    return 0;
}

