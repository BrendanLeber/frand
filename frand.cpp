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

#include "options.h"
#include "xplat.h"


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
    xmkdir(dir_name);
    xchdir(dir_name);

    for (auto it = node->children.begin(); it != node->children.end(); ++it)
        traverse(*it);

    // TODO error checking
    xchdir("..");
}


int main(int argc, char** argv)
{
    // exits program on error
    parse_options(argc, argv);

    typedef tree_node<int> int_node;

    // setup rng for shuffling the nodes
    random_generator gen(seed);

    // generate a vector of nodes for tree
    std::vector<int_node*> dst;
    for (int i = 0; i < num_folders; ++i) {
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
    auto current_dir = xgetcwd();
    xchdir(base_dir);

    // depth first visit
    traverse(root);

    // TODO error checking
    xchdir(current_dir);

    // clean up our objects
    // TODO - replace with generic tree traversal delete delete_tree()
    for (auto it = src.begin(); it != src.end(); ++it)
        delete *it;

    return 0;
}
