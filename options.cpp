/* Copyright (C) 2013-2014 Brendan Leber <brendan@brendanleber.com>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.
 *
 * See http://www.wtfpl.net/ for more details.
 */

#include <iostream>
#include <string>

#include <getopt.h>
#include <stdlib.h>

#include "appinfo.h"
#include "display.h"
#include "options.h"

std::string program_name;
int debug;
std::string base_dir;
std::mt19937::result_type seed;
int num_folders;

static void error_exit();
static void set_program_name(const char* argv0);

static int do_help = 0;
static int do_version = 0;

static const struct option longopts[] =
{
    { "help",    no_argument,       &do_help,    1   },
    { "version", no_argument,       &do_version, 1   },
    { "debug",   no_argument,       &debug,      1   },
    { "base",    required_argument, NULL,        'b' },
    { "seed",    required_argument, NULL,        's' },
    { NULL,      0,                 NULL, 0          }
};

static void error_exit()
{
    std::cerr << "Try `" << program_name << " --help' for more information." << std::endl;
    exit(EXIT_FAILURE);
}

// on error, print message and exit
void parse_options(int argc, char** argv)
{
    // set initial state of global variables
    debug = 0;
    seed = static_cast<std::mt19937::result_type>(time(nullptr));
    base_dir = ".";
    num_folders = -1;

#if !defined(NDEBUG)
    // always output debug information for debug builds
    debug = 1;
#endif

    set_program_name(argv[0]);
    setlocale(LC_ALL, "");

    // we will display our own error messages, thanks.
    opterr = 0;

    int optc;
    while ((optc = getopt_long(argc, argv, ":b:s:", longopts, NULL)) != -1)
        switch (optc) {
        case 0:
            // getopt_long() set a variable, ignore this
            break;

        case 'b':
            base_dir = optarg;
            break;

        case 's': {
            char* endptr;
            long value = strtol(optarg, &endptr, 0);
            if (*endptr != '\0') {
                std::cerr << program_name << ": invalid integer (" << optarg << ") given for the seed." << std::endl;
                error_exit();
            }
            seed = static_cast<std::mt19937::result_type>(value);
            break;
        }

        case ':':
            std::cerr << program_name << ": option '" << static_cast<char>(optopt) << "' requires an argument." << std::endl;
            error_exit();
            break;

        case '?':
        default:
            std::cerr << program_name << ": invalid command line option." << std::endl;
            error_exit();
            break;
        }

    if (do_version) {
        display_version();
        exit(EXIT_SUCCESS);
    }
    else if (do_help) {
        display_help();
        exit(EXIT_SUCCESS);
    }
    else if (argc - optind == 0) {
        std::cerr << program_name << ": missing number of folders to create." << std::endl;
        error_exit();
    }
    else if (argc - optind != 1) {
        std::cerr << program_name << ": too many options given." << std::endl;
        error_exit();
    }

    char *endptr;
    num_folders = strtol(argv[optind], &endptr, 0);
    if (*endptr != '\0') {
        std::cerr << program_name << ": invalid integer (" << argv[optind] << ") for the number of folders to create." << std::endl;
        error_exit();
    }

    if (num_folders < 1) {
        std::cerr << program_name << ": the number of folders must be greater than zero." << std::endl;
        error_exit();
    }

    if (debug) {
        std::cout
            << "seed: " << seed << std::endl
            << "number of folders: " << num_folders << std::endl
            << "base directory: " << base_dir << std::endl;
    }
}

// get invocation name without directory or extension
static void set_program_name(const char* argv0)
{
    program_name = argv0;
    auto pend = program_name.find_last_of("\\/");
    pend = pend == std::string::npos ? 0 : pend + 1;
    auto eend = program_name.find_last_of('.');
    program_name = program_name.substr(pend, eend - pend);
}
