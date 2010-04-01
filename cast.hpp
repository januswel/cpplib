/*
 * cast.hpp
 * Additional casts.
 *
 * written by janus_wel<janus.wel.3@gmail.com>
 * This source code is NOT MY COPYRIGHTED WORK, and has NO WARRANTY.
 *
 * Refer - a source of an idea:
 *  http://www.geocities.jp/KY_webid/cpp/language/024.html#use_template_func
 * */

#ifndef CAST_H
#define CAST_H

namespace util {
    namespace cast {
        template<typename T> inline T constpointer_cast(const void* const p) {
            return static_cast<T>(p);
        }

        template<typename T> inline T pointer_cast(void* const p) {
            return static_cast<T>(p);
        }
    }
}

#endif // CAST_H

