#pragma once

/* Copyright (C) 2013-2014 Brendan Leber <brendan@brendanleber.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.
 * 
 * See http://www.wtfpl.net/ for more details.
 */

#include <map>
#include <string>
#include <vector>

// a very simple command line argument object.
class Options
{
private:
    std::string name;
    std::vector<std::string> args;
    std::map<std::string, std::string> named_args;

    bool helpf, versionf, licensef;

public:
    Options(int argc, char** argv)
        : helpf(false), versionf(false), licensef(false)
    {
        if (argc < 1)
            throw std::runtime_error("invalid number of command line arguments!");

        // get invocation name without directory or extension
        name = argv[0];
        auto pend = name.find_last_of("\\/");
        pend = pend == std::string::npos ? 0 : pend + 1;
        auto eend = name.find_last_of('.');
        name = name.substr(pend, eend - pend);

        // save the arguments in an vector for later processing
        args.reserve(argc - 1);
        args.assign(argv + 1, argv + argc);
    }


    bool process()
    {
        for (auto it = args.begin(); it != args.end(); /*null*/) {
            // skip any argument that isn't an option (doesn't start with '/')
            if (it->at(0) != '/') {
                ++it;
                continue;
            }

            // get the option without the leading slash
            ci_string arg = it->c_str() + 1;

            if (arg == ci_string("help") || arg == ci_string("?")) {
                helpf = true;
                it = args.erase(it);
            }
            else if (arg == ci_string("version")) {
                versionf = true;
                it = args.erase(it);
            }
            else if (arg == ci_string("license")) {
                licensef = true;
                it = args.erase(it);
            }
            else {
                // capture the named argument and its value
                std::string name = it->c_str() + 1;
                it = args.erase(it);
                if (it == args.end()) {
                    std::cerr << "ERROR: named argument '" << name << "' must have a value following it!" << std::endl;
                    return false;
                }
                named_args.insert(std::make_pair(name, *it));
                it = args.erase(it);
            }
        }

        return true;
    }

    // accessors
    bool help_wanted() const { return helpf; }
    bool version_wanted() const { return versionf; }
    bool license_wanted() const { return licensef; }

    std::string invocation_name() const { return name; }

    size_t remaining_args() const { return args.size(); }
    std::string argument(size_t arg) const { return args[arg]; }
    int argument_as_int(size_t arg) const { return atoi(args[arg].c_str()); }

    bool named_arg_exists(const std::string& narg) const
    {
        auto it = named_args.find(narg);
        return it == named_args.end() ? false : true;
    }

    std::string named_arg(const std::string& narg) const
    {
        auto it = named_args.find(narg);
        if (it == named_args.end())
            throw std::runtime_error(std::string("named argument '") + narg + std::string("' does not exist!"));
        else
            return it->second;
    }

    int named_arg_as_int(const std::string& narg) const
    {
        return atoi(named_arg(narg).c_str());
    }
};
