// ----------------------------
// projects/deque/TestDeque.c++
// Copyright (C) 2013
// Glenn P. Downing
// ----------------------------

/*
To test the program:
    % ls -al /usr/include/gtest/
    ...
    gtest.h
    ...

    % locate libgtest.a
    /usr/lib/libgtest.a

    % locate libpthread.a
    /usr/lib/x86_64-linux-gnu/libpthread.a
    /usr/lib32/libpthread.a

    % locate libgtest_main.a
    /usr/lib/libgtest_main.a

    % g++ -pedantic -std=c++0x -Wall TestDeque.c++ -o TestDeque -lgtest -lpthread -lgtest_main

    % valgrind TestDeque > TestDeque.out
*/

// --------
// includes
// --------

#include <algorithm> // equal
#include <cstring>   // strcmp
#include <deque>     // deque
#include <sstream>   // ostringstream
#include <stdexcept> // invalid_argument
#include <string>    // ==

#include "gtest/gtest.h"

#include "Deque.h"
#include <deque>

// ---------------
// DEQUE_FUNCTIONS
// ---------------

class DequeTest : public testing::Test {
    public:
        std::deque<int> d1;
        std::deque<int> d2;
};

TEST_F(DequeTest, equal_empty) 
{
    ASSERT_TRUE(d1 == d2);
}

TEST_F(DequeTest, equal_add) 
{
    d1.push_back(1);
    d1.push_back(2);
    d1.push_back(3);
    d2.push_back(1);
    d2.push_back(2);
    d2.push_back(3);   
    ASSERT_TRUE(d1 == d2);
}

TEST_F(DequeTest, equal_misorder) 
{
    d1.push_back(1);
    d1.push_back(3);
    d1.push_back(2);
    d2.push_back(2);
    d2.push_back(3);   
    d2.push_back(1);
    ASSERT_FALSE(d1 == d2);
}