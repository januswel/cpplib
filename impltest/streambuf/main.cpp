/*
 * main.cpp
 *  check implementations for std::(i|o|io)stream and std::streambuf
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <locale>
#include <streambuf>

class test_streambuf : public std::streambuf {
    protected:
        // locale
        void imbue(const std::locale&) {
            std::cout << " -> imbue(const locale&)";
        }

        // buffer
        std::streambuf* setbuf(char_type*, std::streamsize n) {
            std::cout << " -> setbuf(char*, " << n << ")";
            return this;
        }

        // positioning
        pos_type seekoff(off_type off, std::ios_base::seekdir way,
                std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) {
            std::cout << " -> seekoff(" << off << ", " << way << ", " << which << ")";
            return pos_type(off_type(-1));
        }
        pos_type seekpos(pos_type sp,
                std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) {
            std::cout << " -> seekpos(" << sp << ", " << which << ")";
            return pos_type(off_type(-1));
        }

        // get
        std::streamsize showmanyc(void) {
            std::cout << " -> showmanyc()";
            return 0;
        }
        std::streamsize xsgetn(char_type*, std::streamsize n) {
            std::cout << " -> xsgetn(char*, " << n << ")";
            return 0;
        }
        int_type underflow(void) {
            std::cout << " -> underflow()";
            return traits_type::eof();
        }
        int_type uflow() {
            std::cout << " -> uflow()";
            return underflow();
        }

        // putback
        int_type pbackfail(int_type c = traits_type::eof()) {
            std::cout << " -> pbackfail(" << c << ")";
            return traits_type::eof();
        }

        // put
        std::streamsize xsputn(const char_type*, std::streamsize n) {
            std::cout << " -> xsputn(const char*, " << n << ")";
            return 0;
        }
        int_type overflow(int_type c = traits_type::eof()) {
            std::cout << " -> overflow(" << c << ")";
            return traits_type::eof();
        }
        int sync(void) {
            std::cout << " -> sync()";
            return 0;
        }
};

struct test_iostream : public std::iostream {
    test_iostream(void) : std::iostream(new test_streambuf()) {}
    ~test_iostream(void) { delete rdbuf(); }
};

int main(void) {
    try {
        test_iostream tios;

        char c;
        int i;
        char s[2];

        // imbue
        std::cout << "imbue(const locale&)";
        tios.imbue(std::locale());
        std::cout << std::endl;

        // formatted input function
        std::cout << "operator>>(int&)";
        tios >> i;
        tios.clear();
        std::cout << std::endl;

        // counting
        std::cout << "gcount()";
        tios.gcount();
        tios.clear();
        std::cout << std::endl;

        // unformatted input function
        std::cout << "get()";
        tios.get();
        tios.clear();
        std::cout << std::endl;

        std::cout << "get(char&)";
        tios.get(c);
        tios.clear();
        std::cout << std::endl;

        std::cout << "get(char*, std::streamsize)";
        tios.get(s, 2);
        tios.clear();
        std::cout << std::endl;

        std::cout << "getline(char*, std::streamsize)";
        tios.getline(s, 2);
        tios.clear();
        std::cout << std::endl;

        std::cout << "ignore()";
        tios.ignore();
        tios.clear();
        std::cout << std::endl;

        std::cout << "peek()";
        tios.peek();
        tios.clear();
        std::cout << std::endl;

        std::cout << "read()";
        tios.read(s, 2);
        tios.clear();
        std::cout << std::endl;

        std::cout << "readsome()";
        tios.readsome(s, 2);
        tios.clear();
        std::cout << std::endl;

        // putback
        std::cout << "putback(char)";
        tios.putback(c);
        tios.clear();
        std::cout << std::endl;

        std::cout << "unget()";
        tios.unget();
        tios.clear();
        std::cout << std::endl;

        // formatted output function
        std::cout << "operator<<(int)";
        tios << i;
        tios.clear();
        std::cout << std::endl;

        // unformatted output function
        std::cout << "put(char)";
        tios.put(c);
        tios.clear();
        std::cout << std::endl;

        std::cout << "write(char*, std::streamsize)";
        tios.write(s, 2);
        tios.clear();
        std::cout << std::endl;

        std::cout << "flush()";
        tios.flush();
        tios.clear();
        std::cout << std::endl;

        // synchronize
        std::cout << "sync()";
        tios.sync();
        tios.clear();
        std::cout << std::endl;

        // positioning
        std::cout << "tellg()";
        tios.tellg();
        tios.clear();
        std::cout << std::endl;

        std::cout << "seekg(pos_type)";
        tios.seekg(0);
        tios.clear();
        std::cout << std::endl;

        std::cout << "seekg(off_type, std::ios_base::seekdir)";
        tios.seekg(0, std::ios_base::end);
        tios.clear();
        std::cout << std::endl;

        std::cout << "tellp()";
        tios.tellp();
        tios.clear();
        std::cout << std::endl;

        std::cout << "seekp(pos_type)";
        tios.seekp(0);
        tios.clear();
        std::cout << std::endl;

        std::cout << "seekp(off_type, std::ios_base::seekdir)";
        tios.seekp(0, std::ios_base::end);
        tios.clear();
        std::cout << std::endl;

        // iterator
        std::cout << "iterator";
        std::istream_iterator<int> iitr(tios), last;
        std::copy(iitr, last, std::ostream_iterator<int>(std::cout, "\n"));
        std::cout << std::endl;

        std::cout << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}

