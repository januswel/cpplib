/*
 * main.cpp
 *  check if the implementation gives RVO - Return Value Optimization - or not
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include <iostream>

struct Foo {
    bool is_optimized;

    Foo(void) : is_optimized(true) {
        std::cout << "Foo::Foo(void)" << std::endl;
    }
    Foo(const Foo&) {
        std::cout << "Foo::Foo(const Foo&)" << std::endl;
        is_optimized = false;
    }
    const Foo& operator =(const Foo&) {
        std::cout << "Foo::operator =(const Foo& rhs)" << std::endl;
        return *this;
    }
};

std::ostream& operator<<(std::ostream& out, const Foo& foo) {
    return out << (foo.is_optimized ? "optimized" : "not optimized");
}

// return value optimization
Foo rvo(void) {
    return Foo();
}

// named return value optimization
Foo nrvo(void) {
    Foo foo;
    return foo;
}

int main(void) {
    std::cout << "RVO\n";
    Foo foo1 = rvo();
    std::cout
        << foo1 << "\n"
        << std::endl;

    std::cout << "NRVO\n";
    Foo foo2 = nrvo();
    std::cout
        << foo2 << "\n"
        << std::endl;

    std::cout << "binding temporary object by const reference\n";
    // A next expression is evaluated as followings.
    //  01. Evaluating "rvo()"
    //  02. Returning the instance of class "Foo" as rhs (This is temporary object)
    //  03. Labeling temprary object as "foo3" (Defining alias "foo3")
    const Foo& foo3 = rvo();
    std::cout
        << foo3 << "\n"
        << std::endl;

    std::cout << "binding named temporary object const reference\n";
    const Foo& foo4 = nrvo();
    std::cout
        << foo4 << "\n"
        << std::endl;

    std::cout << "assignment" << std::endl;
    Foo foo5;
    foo5 = rvo();
    std::cout << std::endl;

    return 0;
}

