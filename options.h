#pragma once

/* Copyright (C) 2013-2015 Brendan Leber <brendan@brendanleber.com>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.
 *
 * See http://www.wtfpl.net/ for more details.
 */

#include <random>
#include <stdexcept>
#include <string>

extern std::string program_name;
extern int debug;
extern std::string base_dir;
extern std::mt19937::result_type seed;
extern int num_folders;

void parse_options(int argc, char** argv);
