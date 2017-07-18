# frand 

Create a random collection of folders.

## Syntax

    frand [OPTION]... <number-of-folders>

Possible options:

      --help     display this help and exit
      --version  display version information and exit

      -b, --base=<dir>   set the base directory for generated folder tree
      -s, --seed=<seed>  set the value used to seed the RNG

      --debug    display extra information for debugging

# Project Code of Conduct

This project adheres to the Contributor Covenant [code of
conduct](CODE_OF_CONDUCT.md).  By participating, you are expected to
uphold this code.  Please report unnaceptable behavior to
brendan@brendanleber.com.

# Contributing

First of all, thanks for taking the time to contribute.

## Submitting Issues

You can create an issue
[here](https://github.com/BrendanLeber/frand/issues/new).  Please
include as many details as possible with your report.  If you can,
please include:

* The version of frand you are using.
* The operating system you are using.
* The command line parameters you used.
* Any error output or alerts.

Please perform a cursory search to see if a similar issue has already
been submitted.  If it has please add your details to the exiting
issue to help with diagnosing the problem.

## Submitting Pull Requests

Bug fixes and enhancements are welcome.  Please submit pull requests
for your changes.

Your pull request should: 

* Include a description of what your change intends to do

* Be a child commit of a reasonably recent commit in the **master**
  branch

  * Requests need not be a single commit, but should be a linear
    sequence of commits (i.e. no merge commits in your PR)

* Have clear commit messages 

  * e.g. "Fix issue...", "Add tests for type", etc.

* Include appropriate tests 
    * Tests should include reasonable permutations of the target fix/change
    * Include baseline changes with your change
    * All changed code must have 100% code coverage

* To avoid line ending issues, set `autocrlf = input` and `whitespace
  = cr-at-eol` in your git configuration

# Future Improvements and Features

## Improvements

- [x] Create base directory if it doesn't exist.
- [ ] Use std::shared_ptr<> instead of raw pointers.

## Future Features

- [ ] Accept `--prefix=<string>` argument to add a prefix to all the directories created.
- [ ] Add `--fill=<int>` argument to create zero-filled path names.

# LICENSE AND COPYRIGHT

Copyright (C) 2013-2017 Brendan Leber <brendan@brendanleber.com>

This program is free software; you can redistribute it and/or modify it under
the terms of the DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE.
See copying.txt for more information.
