#pragma once

/* Copyright (C) 2013-2017 Brendan Leber <brendan@brendanleber.com>
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

class Options {
public:
    static std::string m_program_name;
    static int m_debug;
    static std::string m_base_dir;
    static std::mt19937::result_type m_seed;
    static long m_num_folders;

public:
    static void parse(int argc, char** argv);
    static void set_program_name(const char* argv0);

    static std::string program_name() { return m_program_name; }
    static bool debug() { return m_debug != 0; }
    static int debug_level() { return m_debug; }
    static std::string base_dir() { return m_base_dir; }
    static std::mt19937::result_type seed() { return m_seed; }
    static long num_folders() { return m_num_folders; }
};
