/* Copyright (C) 2013-2014 Brendan Leber <brendan@brendanleber.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.
 * 
 * See http://www.wtfpl.net/ for more details.
 */

#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "appinfo.h"
#include "ci_string.h"
#include "display.h"
#include "options.h"


Options goptions;


template<typename T>
struct tree_node
{
    T data;
    tree_node* parent;
    std::vector<tree_node*> children;

    tree_node(T value) : data(value), parent(nullptr) {}
};


struct random_generator : std::unary_function<unsigned, unsigned>
{
    std::mt19937 gen;

    unsigned operator()(unsigned i)
    {
        std::uniform_int_distribution<> rng(0, i - 1);
        return rng(gen);
    }
    
    random_generator(std::mt19937::result_type seed) : gen()
    {
        gen.seed(seed);
    }

private:
    random_generator(const random_generator& src);
    random_generator& operator=(const random_generator& rhs);
};


template<typename Iter>
Iter random_element(Iter begin, Iter end, random_generator& gen)
{
    const typename Iter::difference_type n = std::distance(begin, end);
    const typename Iter::difference_type k = gen(static_cast<unsigned>(n));

    Iter ret = begin;
    std::advance(ret, k);
    return ret;
}


template<typename T>
T num_digits(T value)
{
    return static_cast<T>(floor(log10(abs(static_cast<double>(value))))) + 1;
}


template<typename T>
void traverse(T* node)
{
    if (node == nullptr)
        return;

    std::stringstream ss;
    ss << node->data;
    const std::string& dir_name = ss.str();

    // TODO error checking
    mkdir(dir_name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    chdir(dir_name.c_str());

    for (auto it = node->children.begin(); it != node->children.end(); ++it)
        traverse(*it);
    
    // TODO error checking
    chdir("..");
}

// ===================================================================================================

std::string _getcwd();

// ===================================================================================================

std::string _getcwd()
{
    const size_t chunkSize = 255;
    const int maxChunks = 10240; // 2550 KiBs of current path are more than enough

    char stackBuffer[chunkSize]; // Stack buffer for the "normal" case
    if (getcwd(stackBuffer, sizeof(stackBuffer)) != NULL)
        return stackBuffer;
    
    if (errno != ERANGE) {
        // It's not ERANGE, so we don't know how to handle it
        throw std::runtime_error("Cannot determine the current path.");
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
            throw std::runtime_error("Cannot determine the current path.");
            // Of course you may choose a different error reporting method
        }   
    }

    throw std::runtime_error("Cannot determine the current path; the path is apparently unreasonably long");
}

// ===================================================================================================

int main(int argc, char** argv)
{
    goptions = Options(argc, argv);

    typedef tree_node<int> int_node;

    if (!goptions.process())
        return 1;

    // handle the information options that stop the program after being displayed
    if (goptions.help_wanted()) {
        display_help();
        return 0;
    }
    else if (goptions.version_wanted()) {
        display_version();
        return 0;
    }

    // make sure we have one argument left for the number of folders to create
    if (goptions.remaining_args() != 1) {
        // TODO display_syntax();
        return 1;
    }

    int folders = goptions.argument_as_int(0);
    if (!folders) {
        std::cerr << "Invalid value (" << goptions.argument(0) << ") for number of folders!" << std::endl;
        return 2;
    }

    // default to time for rng seed unless user specified one on the command line
    auto seed = static_cast<std::mt19937::result_type>(time(nullptr));
    if (goptions.named_arg_exists("seed"))
        seed = static_cast<std::mt19937::result_type>(goptions.named_arg_as_int("seed"));

    // display the options so that the run can be repeated for testing purposes
    // TODO - option (/debug | /verbose) to display diagnostic information
    std::cout
        << "number of folders: " << folders << '\n'
        << "seed: " << seed << '\n'
        << "base directory: '" << (goptions.named_arg_exists("base") ? goptions.named_arg("base") : ".") << "'\n"
        << std::endl;

    // setup rng for shuffling the nodes
    random_generator gen(seed);

    // generate a vector of nodes for tree
    std::vector<int_node*> dst;
    for (int i = 0; i < folders; ++i) {
        dst.push_back(new int_node(i));
    }

    // dst := random permutation of all nodes;
    std::random_shuffle(dst.begin(), dst.end(), gen);

    // src.push(dst.pop()); % picks the root
    std::vector<int_node*> src;
    src.push_back(*dst.rbegin());
    dst.pop_back();

    while (!dst.empty()) {
        // a := random element from src;
        auto a = random_element(src.begin(), src.end(), gen);

        // b := dst.pop();
        auto b = dst.rbegin();

        // add the edge (a, b)
        (*a)->children.push_back(*b);
        (*b)->parent = *a;

        // src.push(b);
        src.push_back(*b);

        // pop here so we don't invalidate our iterator earlier
        dst.pop_back();
    }

    // our shiny new random tree
    int_node* root = *src.begin();

    // if the user specified a base directory, save current, change to base, execute and restore current
    // TODO error checking
    auto current_dir = _getcwd();
    if (goptions.named_arg_exists("base"))
        chdir(goptions.named_arg("base").c_str());

    // depth first visit
    traverse(root);

    // TODO error checking
    if (goptions.named_arg_exists("base"))
        chdir(current_dir.c_str());

    // clean up our objects
    // TODO - replace with generic tree traversal delete delete_tree()
    for (auto it = src.begin(); it != src.end(); ++it)
        delete *it;

    return 0;
}
