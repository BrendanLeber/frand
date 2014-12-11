/* Copyright (C) 2013-2014 Brendan Leber <brendan@brendanleber.com>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.
 *
 * See http://www.wtfpl.net/ for more details.
 */

#include <iostream>

#include "appinfo.h"
#include "display.h"
#include "options.h"

void display_help()
{
    std::cout
        << "Usage: " << program_name << " [OPTION]... num-folders" << std::endl
        << APPLICATION_SHORT_DESC << std::endl
        << std::endl
        << "      --help     display this help and exit" << std::endl
        << "      --version  display version information and exit" << std::endl
        << std::endl
        << "  -b, --base=<dir>   set the base directory for generated folder tree" << std::endl
        << "  -s, --seed=<seed>  set the value used to seed the RNG" << std::endl
        << std::endl
        << "      --debug    display extra information for debugging" << std::endl
        << std::endl
        << "Report bugs to: " << APPLICATION_BUGREPORT << std::endl;

#if defined(APPLICATION_URL)
    std::cout << APPLICATION_NAME " home page: <" APPLICATION_URL << ">" << std::endl;
#endif
}

void display_version()
{
    std::cout
        << APPLICATION_NAME " v" APPLICATION_VERSION_STRING " - " APPLICATION_SHORT_DESC << std::endl
        << std::endl
        << "Copyright (C) " APPLICATION_COPYRIGHT_YEARS " Brendan Leber <brendan@brendanleber.com>" << std::endl
        << std::endl
        << "This work is free. You can redistribute it and/or modify it under the" << std::endl
        << "terms of the Do What The Fuck You Want To Public License, Version 2," << std::endl
        << "as published by Sam Hocevar." << std::endl
        << std::endl
        << "See <http://www.wtfpl.net/> for more details." << std::endl;
}
