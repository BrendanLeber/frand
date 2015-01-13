/* Copyright (C) 2013-2015 Brendan Leber <brendan@brendanleber.com>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.
 *
 * See http://www.wtfpl.net/ for more details.
 */

#include <string>
#include <sstream>
#include <vector>
#include "utility.h"

static std::vector<std::string>&
split(const std::string str, char delim, std::vector<std::string>& elems)
{
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string>
split(const std::string& str, char delim)
{
    std::vector<std::string> elems;
    return split(str, delim, elems);
}
