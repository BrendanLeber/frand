#pragma once

/* Copyright (C) 2013-2014 Brendan Leber <brendan@brendanleber.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.
 * 
 * See http://www.wtfpl.net/ for more details.
 */

#include <string>

// cross platform functions to work directories
int xchdir(const std::string& dir);
std::string xgetcwd();
int xmkdir(const std::string& dir);
