/* Copyright (C) 2013-2017 Brendan Leber <brendan@brendanleber.com>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.
 *
 * See http://www.wtfpl.net/ for more details.
 */

#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// TODO(bml) remove after Microsoft/GSL fixes this warning
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#include "gsl/gsl"
#pragma clang diagnostic pop

// TODO(bml) remove after hsutter/gcpp fixes these warnings
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wextra-semi"
#pragma clang diagnostic ignored "-Wstring-conversion"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#include "deferred_allocator.h"
#pragma clang diagnostic pop

#include "options.hpp"

std::vector<std::string> split(const std::string& str, char delim);

int xchdir(const std::string& dir);
std::string xgetcwd();
int xmkdir(const std::string& dir);
int xmkpath(const std::string& path);

template <typename T>
struct tree_node {
    T data;
    tree_node* parent;
    std::vector<tree_node*> children;

    explicit tree_node(T value) : data(value), parent(nullptr) {}
};

struct random_generator : std::unary_function<unsigned, unsigned> {
    std::mt19937 gen;

    unsigned operator()(unsigned i)
    {
        std::uniform_int_distribution<unsigned> rng(0, i - 1);
        return rng(gen);
    }

    explicit random_generator(std::mt19937::result_type s)
    {
        gen.seed(s);
    }

    // private:
    //     random_generator(const random_generator& src) = delete;
    //     random_generator& operator=(const random_generator& rhs) = delete;
};

template <typename Iter>
Iter random_element(Iter begin, Iter end, random_generator& gen) // NOLINT
{
    const typename Iter::difference_type n = std::distance(begin, end);
    const typename Iter::difference_type k = gen(static_cast<unsigned>(n));

    Iter ret = begin;
    std::advance(ret, k);
    return ret;
}

template <typename T>
T num_digits(T value)
{
    return static_cast<T>(floor(log10(fabs(static_cast<double>(value))))) + 1;
}

std::vector<std::string>
split(const std::string& str, char delim)
{
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

template <typename T>
void traverse(T* node)
{
    if (node == nullptr) {
        return;
    }

    std::stringstream ss;
    ss << node->data;
    const std::string& dir_name = ss.str();

    // TODO(bml) error checking
    xmkdir(dir_name);
    xchdir(dir_name);

    for (auto it = node->children.begin(); it != node->children.end(); ++it) {
        traverse(*it);
    }

    // TODO(bml) error checking
    xchdir("..");
}

int xchdir(const std::string& dir)
{
    if (Options::debug()) {
        std::cout << "--- xchdir('" << dir << "')\n";
    }

    int rv = chdir(dir.c_str());

    if (rv != 0) {
        std::cout << "*** " << strerror(errno) << '\n';
    }

    return rv;
}

std::string xgetcwd()
{
    // 255 chars should be enough for the normal case
    std::vector<char> cwd(255);
    while (true) {
        if (getcwd(cwd.data(), cwd.size() * sizeof(char)) != nullptr) {
            return std::string(std::begin(cwd), std::end(cwd));
        }

        if (errno != ERANGE) {
            // It's not ERANGE, so we don't know how to handle it
            throw std::runtime_error("cannot determine the current path.");
            // Of course you may choose a different error reporting method
        }

        cwd.resize(cwd.size() * 2);
    }
}

int xmkdir(const std::string& dir)
{
    if (Options::debug()) {
        std::cout << "--- xchdir('" << dir << "')\n";
    }

#if 0
    mode_t mask = xgetumask();
#else
    mode_t mask = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
#endif

    // attempt to create the directory
    auto rv = mkdir(dir.c_str(), mask);

    if (rv != 0 && errno == EEXIST) {
        // silently accept the error if the directory exists
        rv = 0;
    }
    else if (rv != 0) {
        // display an error to our user if debug mode is active
        std::cerr << "*** " << strerror(errno) << '\n';
    }

    return rv;
}

int xmkpath(const std::string& path)
{
    auto elements = split(path, '/');

    std::string p;
    for (const auto& element : elements) {
        if (p.empty()) {
            p = element;
        }
        else {
            p += "/" + element;
        }

        if (Options::debug()) {
            std::cerr
                << __FUNCTION__ // NOLINT
                << ": path '" << p << "'" << std::endl;
        }

        auto rv = xmkdir(p);
        if (rv != 0) {
            std::cerr
                << "xmkdir('" << p << "')"
                << " rv = " << rv
                << " errno = " << errno
                << std::endl;
            return rv;
        }
    }

    return 0;
}

int main(int argc, char** argv)
{
    // exits program on error
    Options::parse(argc, argv);

    using int_node = tree_node<int>;

    // setup rng for shuffling the nodes
    random_generator gen(Options::seed());

    // generate a vector of nodes for tree
    std::vector<int_node*> dst;
    for (int i = 0; i < Options::num_folders(); ++i) {
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
    // TODO(bml) error checking
    auto current_dir = xgetcwd();
    xmkpath(Options::base_dir());
    xchdir(Options::base_dir());

    // depth first visit
    traverse(root);

    // TODO(bml) error checking
    xchdir(current_dir);

    // clean up our objects
    // TODO(bml) replace with generic tree traversal delete delete_tree()
    for (auto& it : src) {
        delete it;
    }

    return 0;
}
