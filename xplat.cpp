/* Copyright (C) 2013-2014 Brendan Leber <brendan@brendanleber.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.
 * 
 * See http://www.wtfpl.net/ for more details.
 */

#include <ios>
#include <iostream>
#include <memory>
#include <stdexcept>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "options.h"
#include "xplat.h"


#if 0
// this isn't working as we had hoped...
static mode_t xgetumask()
{
    mode_t mask = umask(0);
    umask(mask);

    if (debug) {
        auto flags = std::cout.flags();
        std::cout.setf(std::ios::oct);
        std::cout << "mode: " << static_cast<int>(mask) << std::endl;
        std::cout.setf(flags);
    }

    return mask;
}
#endif


int xchdir(const std::string& dir)
{
    int rv = chdir(dir.c_str());

    if (debug && rv) {
        std::cout
            << "xchdir('" << dir << "') rv = " << rv
            << " errno = " << errno
            << std::endl;
    }

    return rv;
}


// TODO rewrite to use std::vector or something like that instead
std::string xgetcwd()
{
    const size_t chunkSize = 255;
    const int maxChunks = 10240; // 2550 KiBs of current path are more than enough

    char stackBuffer[chunkSize]; // Stack buffer for the "normal" case
    if (getcwd(stackBuffer, sizeof(stackBuffer)) != NULL)
        return stackBuffer;
    
    if (errno != ERANGE) {
        // It's not ERANGE, so we don't know how to handle it
        throw std::runtime_error("cannot determine the current path.");
        // Of course you may choose a different error reporting method
    }

    // Ok, the stack buffer isn't long enough; fallback to heap allocation
    for (int chunks = 2; chunks < maxChunks; ++chunks) {
        // With boost use scoped_ptr; in C++0x, use unique_ptr
        // If you want to be less C++ but more efficient you may want to use realloc
        std::auto_ptr<char> cwd(new char[chunkSize*chunks]); 
        if (getcwd(cwd.get(), chunkSize * chunks) != NULL)
            return cwd.get();

        if (errno != ERANGE)
        {
            // It's not ERANGE, so we don't know how to handle it
            throw std::runtime_error("cannot determine the current path.");
            // Of course you may choose a different error reporting method
        }   
    }

    throw std::runtime_error("cannot determine the current path; the path is apparently unreasonably long");
}


int xmkdir(const std::string& dir)
{
#if 0
    mode_t mask = xgetumask();
#else
    mode_t mask = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
#endif

    auto rv = mkdir(dir.c_str(), mask);

    if (debug && rv) {
        std::cout
            << "xmkdir('" << dir << "')"
            << " mask = " << std::oct << mask << std::dec
            << " rv = " << rv
            << " errno = " << errno
            << std::endl;
    }
    
    return rv;
}
