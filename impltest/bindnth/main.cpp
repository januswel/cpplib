/*
 * main.cpp
 *  implementation test codes for std::bind2nd()
 *
 *  in vs9, this codes can't be passed by C2529
 *  http://msdn.microsoft.com/ja-jp/library/kf78kcyw.aspx
 *
 *  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
 *  see LICENSE for redistributing, modifying, and so on.
 * */

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <vector>

// The '&' of the second argument causes the error in some implementations.
inline double mul(const int d, const double& e) { return e * d; }

int main(void) {
    const int upper = 10;
    std::vector<int> ds;
    ds.reserve(upper);
    for (int i = 0; i < upper; ++i) {
        ds.push_back(i);
    }

    std::ostream_iterator<double> oitr(std::cout, "\n");

    std::cout << "source\n";
    std::copy(ds.begin(), ds.end(), oitr);
    std::cout << std::endl;

    const double m = 3.3f;
    std::cout << "mul by " << m << "\n";
    std::transform(
            ds.begin(), ds.end(), oitr,
            std::bind2nd(std::ptr_fun(mul), m));
    std::cout << std::endl;

    return 0;
}

