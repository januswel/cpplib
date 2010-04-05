/*
 * main.cpp
 *  Sample codes for observer.hpp
 *
 *  written by janus_wel<janus.wel.3@gmail.com>
 *  This source code is in public domain, and has NO WARRANTY.
 * */

#include <iostream>
#include <vector>
#include "../../header/observer.hpp"

enum language_t {
    CHINESE,
    ENGLISH,
    GERMAN,
    ITALIAN,
    JAPANESE,
    RUSSIAN,

    NUMOF_LANG
};

const char* language_name(const language_t lang) {
    switch (lang) {
        case CHINESE:   return "Chinese";
        case ENGLISH:   return "English";
        case GERMAN:    return "German";
        case ITALIAN:   return "Italian";
        case JAPANESE:  return "Japanese";
        case RUSSIAN:   return "Russian";
        default:        return NULL;
    }
}

class Language : public pattern::observer::basic_subject<language_t> {
    private:
        language_t lang;

    protected:
        // implementation of virtual function
        // tell what object represents a state to basic_subject
        const state_t& state(void) const { return lang; }

    public:
        Language(language_t lang = ENGLISH) : lang(lang) { notify(); }
        void set(language_t lang) { this->lang = lang; notify(); }
        const char* what(void) const { return language_name(lang); }
};

std::ostream& operator<< (std::ostream& out, const Language& l) {
    return out << l.what();
}

class Fruit : public pattern::observer::basic_observer<language_t> {
    protected:
        language_t lang;

    public:
        Fruit() : lang(ENGLISH) {}
        virtual ~Fruit() {}

        void language(const language_t lang) { this->lang = lang; }
        const char* language(void) const { return language_name(lang); }
        virtual const char* name(void) const = 0;

        // implementation of virtual function
        void update(const state_t& s) { lang = s; }
};

std::ostream& operator<< (std::ostream& out, const Fruit& f) {
    return out << f.name() << "(" << f.language() << ")";
}

class Apple : public Fruit {
    public:
        const char* name(void) const {
            switch (lang) {
                case CHINESE:   return "\u82f9\u679c";
                case ENGLISH:   return "apple";
                case GERMAN:    return "apfel";
                case ITALIAN:   return "mela";
                case JAPANESE:  return "\u308a\u3093\u3054";
                case RUSSIAN:   return "\u044f\u0431\u043b\u043e\u043a\u043e";
                default:        return NULL;
            }
        }
};

class Orange : public Fruit {
    public:
        const char* name(void) const {
            switch (lang) {
                case CHINESE:   return "\u6a58\u5b50";
                case ENGLISH:   return "orange";
                case GERMAN:    return "orange";
                case ITALIAN:   return "mandarino";
                case JAPANESE:  return "\u307f\u304b\u3093";
                case RUSSIAN:   return "\u043c\u0430\u043d\u0434\u0430\u0440\u0438\u043d";
                default:        return NULL;
            }
        }
};

class Banana : public Fruit {
    public:
        const char* name(void) const {
            switch (lang) {
                case CHINESE:   return "\u9999\u8549";
                case ENGLISH:   return "banana";
                case GERMAN:    return "banane";
                case ITALIAN:   return "banana";
                case JAPANESE:  return "\u3070\u306a\u306a";
                case RUSSIAN:   return "\u0431\u0430\u043d\u0430\u043d";
                default:        return NULL;
            }
        }
};

int main(void) {
    // create fruits
    Apple apple;
    Orange orange;
    Banana banana;

    // print default state
    std::cout
        << apple << "\n"
        << orange << "\n"
        << banana << "\n"
        << std::endl;

    // build language selector
    Language lang;
    // register fruits
    lang.attach(apple).attach(orange).attach(banana);

    // register all languages
    typedef std::vector<language_t> lang_list_t;
    typedef lang_list_t::iterator   lang_it;
    lang_list_t langs;
    langs.push_back(CHINESE);
    langs.push_back(ENGLISH);
    langs.push_back(GERMAN);
    langs.push_back(ITALIAN);
    langs.push_back(JAPANESE);
    langs.push_back(RUSSIAN);

    // print all languages
    for (lang_it it = langs.begin(); it < langs.end(); ++it) {
        lang.set(*it);
        std::cout
            << apple << "\n"
            << orange << "\n"
            << banana << "\n"
            << std::endl;
    }

    // release apple
    lang.detach(apple);

    // print in Japanese
    lang.set(JAPANESE);
    std::cout
        << apple << "\n"    // in Russian!!
        << orange << "\n"
        << banana << "\n"
        << std::endl;

    return 0;
}

