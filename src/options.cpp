/* Copyright (C) 2013-2017 Brendan Leber <brendan@brendanleber.com>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.
 *
 * See http://www.wtfpl.net/ for more details.
 */

#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <string>

#include "appinfo.hpp"
#include "options.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"

// TODO(bml) remove NOLINT after upgrading to a version of clang-tidy that's been fixed
std::string Options::m_program_name; // NOLINT
int Options::m_debug = 0; // NOLINT
std::string Options::m_base_dir; // NOLINT
std::mt19937::result_type Options::m_seed{0}; // NOLINT
long Options::m_num_folders{0}; // NOLINT

#pragma clang diagnostic pop

[[noreturn]] static void error_exit()
{
    std::cerr << "Try `" << Options::program_name() << " --help' for more information.\n";
    exit(EXIT_FAILURE);
}

[[noreturn]] static void display_help()
{
    std::cout
        << "Usage: " << Options::program_name() << " [OPTION]... num-folders\n"
        << APPLICATION_SHORT_DESC << '\n'
        << '\n'
        << "      --help     display this help and exit\n"
        << "      --version  display version information and exit\n"
        << '\n'
        << "  -b, --base=<dir>   set the base directory for generated folder tree\n"
        << "  -s, --seed=<seed>  set the value used to seed the RNG\n"
        << '\n'
        << "      --debug    display extra information for debugging\n"
        << '\n'
        << "Report bugs to: " << APPLICATION_BUGREPORT << '\n';

#if defined(APPLICATION_URL)
    std::cout << APPLICATION_NAME " home page: <" APPLICATION_URL << ">\n";
#endif

    exit(EXIT_SUCCESS);
}

[[noreturn]] static void display_version()
{
    std::cout
        << APPLICATION_NAME " v" APPLICATION_VERSION_STRING " - " APPLICATION_SHORT_DESC << '\n'
        << '\n'
        << "Copyright (C) " APPLICATION_COPYRIGHT_YEARS " Brendan Leber <brendan@brendanleber.com>\n"
        << '\n'
        << "This work is free. You can redistribute it and/or modify it under the\n"
        << "terms of the Do What The Fuck You Want To Public License, Version 2,\n"
        << "as published by Sam Hocevar.\n"
        << '\n'
        << "See <http://www.wtfpl.net/> for more details.\n";

    exit(EXIT_SUCCESS);
}

// on error, print message and exit
void Options::parse(int argc, char** argv)
{
    static int do_help{0};
    static int do_version{0};

    static const struct option longopts[] = {
        {"help", no_argument, &do_help, 1},
        {"version", no_argument, &do_version, 1},
        {"debug", no_argument, &m_debug, 1},
        {"base", required_argument, nullptr, 'b'},
        {"seed", required_argument, nullptr, 's'},
        {nullptr, 0, nullptr, 0}};

    // set initial state of global variables
    m_debug = 0;
    m_seed = static_cast<std::mt19937::result_type>(time(nullptr));
    m_base_dir = ".";
    m_num_folders = -1;

#if !defined(NDEBUG)
    // always output debug information for debug builds
    m_debug = 1;
#endif

    set_program_name(argv[0]); // NOLINT
    setlocale(LC_ALL, "");

    // we will display our own error messages, thanks.
    opterr = 0;

    int optc;
    auto plongopts = static_cast<const struct option*>(&longopts[0]);
    while ((optc = getopt_long(argc, argv, ":b:s:", plongopts, nullptr)) != -1) {
        switch (optc) {
        case 0:
            // getopt_long() set a variable, ignore this
            break;

        case 'b':
            m_base_dir = optarg;
            break;

        case 's': {
            char* endptr;
            auto value = strtol(optarg, &endptr, 0);
            if (*endptr != '\0') {
                std::cerr << m_program_name << ": invalid integer (" << optarg << ") given for the seed.\n";
                error_exit();
            }
            m_seed = static_cast<std::mt19937::result_type>(value);
            break;
        }

        case ':':
            std::cerr << m_program_name << ": option '" << static_cast<char>(optopt) << "' requires an argument.\n";
            error_exit();

        case '?':
        default:
            std::cerr << m_program_name << ": invalid command line option.\n";
            error_exit();
        }
    }

    if (do_version != 0) {
        display_version();
    }
    else if (do_help != 0) {
        display_help();
    }
    else if (argc - optind == 0) {
        std::cerr << m_program_name << ": missing number of folders to create.\n";
        error_exit();
    }
    else if (argc - optind != 1) {
        std::cerr << m_program_name << ": too many options given.\n";
        error_exit();
    }

    char* endptr;
    m_num_folders = strtol(argv[optind], &endptr, 0); // NOLINT
    if (*endptr != '\0') {
        std::cerr
            << m_program_name << ": invalid integer ("
            << argv[optind] // NOLINT
            << ") for the number of folders to create.\n";
        error_exit();
    }

    if (m_num_folders < 1) {
        std::cerr << m_program_name << ": the number of folders must be greater than zero.\n";
        error_exit();
    }

    if (m_debug != 0) {
        std::cout
            << "seed: " << m_seed << std::endl
            << "number of folders: " << m_num_folders << std::endl
            << "base directory: " << m_base_dir << std::endl;
    }
}

// get invocation name without directory or extension
void Options::set_program_name(const char* argv0)
{
    m_program_name = argv0;
    auto pend = m_program_name.find_last_of(R"(\/)");
    pend = pend == std::string::npos ? 0 : pend + 1;
    auto eend = m_program_name.find_last_of('.');
    m_program_name = m_program_name.substr(pend, eend - pend);
}
