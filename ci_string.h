#pragma once

/* Copyright (C) 2013-2014 Brendan Leber <brendan@brendanleber.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.
 * 
 * See http://www.wtfpl.net/ for more details.
 */

#include <vector>
#include <string>

struct ci_char_traits : public std::char_traits<char>
    // just inherit all the other functions
    //  that we don't need to override
{
    static bool eq(char c1, char c2)
    {
        return toupper(c1) == toupper(c2);
    }

    static bool ne(char c1, char c2)
    {
        return toupper(c1) != toupper(c2);
    }

    static bool lt(char c1, char c2)
    {
        return toupper(c1) <  toupper(c2);
    }

    static int compare(const char* s1, const char* s2, size_t n)
    {
        return _memicmp(s1, s2, n);
    }

    static const char* find(const char* s, int n, char a)
    {
        while (n-- > 0 && toupper(*s) != toupper(a)) {
            ++s;
        }
        
        return s;
    }

private:
    // TODO implement in a more generic C++11 way
    static int _memicmp(const void *s1, const void *s2, size_t n)
    {
        if (n != 0) {
            auto p1 = reinterpret_cast<const unsigned char*>(s1);
            auto p2 = reinterpret_cast<const unsigned char*>(s2);

            do {
                if (toupper(*p1) != toupper(*p2))
                    return *p1 - *p2;
                p1++;
                p2++;
            } while (--n != 0);
        }

        return 0;
    }
};

typedef std::basic_string<char, ci_char_traits> ci_string;
