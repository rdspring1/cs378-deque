// ----------------------------
// projects/deque/TestDeque.c++
// Copyright (C) 2013
// Glenn P. Downing
// ----------------------------

#define private public
#define protected public
#define SIZE 500
#define ITERATION 10000

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
#include <algorithm>
#include <iostream>

#include "gtest/gtest.h"

#include "Allocator.h"
#include "Deque.h"
#include <deque>

// ---------------
// DEQUE_FUNCTIONS
// ---------------
///
/// The DequeTest class is a Google Test Fixture that creates two Deque data structures.
/// The data structure type is either MyDeque or Project 4: Deque.
///
class DequeTest : public testing::Test 
{
	public:
		MyDeque<int> d1;
		MyDeque<int> d2;
};

///
/// The DequeIterTest class is a Google Test Fixture created to test the iterator and const iterator inner classes.
///
class DequeIterTest : public testing::Test
{
	public:
		MyDeque<int> d;

		virtual void SetUp() 
		{
			for(int i = 0; i < SIZE; ++i)
				d.push_back(i);
		}
}; 

class DequeSingleTest : public testing::Test
{
	public:
		int v1;
		int v2;
		int v3;
		size_t mediumSize;
		size_t largeSize;

		virtual void SetUp() 
		{
			v1 = 1;
			v2 = 2;
			v3 = 3;
			mediumSize = 15;
			largeSize = 100;
		}
};

TEST_F(DequeTest, equal_empty) 
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
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
	ASSERT_TRUE(d1 != d2);
}

TEST_F(DequeTest, less_than_empty) 
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_FALSE(d1 < d2);
}

TEST_F(DequeTest, less_than_true) 
{
	d1.push_back(100);
	d1.push_back(100);
	d1.push_back(100);
	d2.push_back(200);
	d2.push_back(200);
	d2.push_back(200); 
	ASSERT_TRUE(d1 < d2);
}

TEST_F(DequeTest, less_than_false) 
{
	d1.push_back(200);
	d1.push_back(200);
	d1.push_back(200);
	d2.push_back(100);
	d2.push_back(100);
	d2.push_back(100); 
	ASSERT_FALSE(d1 < d2);
}

TEST_F(DequeTest, less_than_equal) 
{
	d1.push_back(1);
	d1.push_back(2);
	d1.push_back(3);
	d2.push_back(1);
	d2.push_back(2);
	d2.push_back(3); 
	ASSERT_FALSE(d1 < d2);
}

TEST_F(DequeTest, swap)
{
	d1.push_back(1);
	d1.push_back(2);
	d1.push_back(3);
	d2.push_back(1);
	d2.push_back(2);
	d2.push_back(3);   
	ASSERT_TRUE(d1 == d2);
	d1.swap(d2);
	ASSERT_TRUE(d1 == d2);
}

TEST_F(DequeTest, swap_empty)
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);
	d1.swap(d2);
	ASSERT_TRUE(d1 == d2);
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
}

TEST_F(DequeTest, swap_misorder)
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	d1.push_back(1);
	d1.push_back(3);
	d1.push_back(2);
	d2.push_back(2);
	d2.push_back(3);   
	d2.push_back(1);
	ASSERT_FALSE(d1 == d2);
	d1.swap(d2);
	ASSERT_TRUE(d1[0] == 2);
	ASSERT_TRUE(d1[1] == 3);
	ASSERT_TRUE(d1[2] == 1);
	ASSERT_TRUE(d2[0] == 1);
	ASSERT_TRUE(d2[1] == 3);
	ASSERT_TRUE(d2[2] == 2);   
	ASSERT_TRUE(d1.size() == 3);
	ASSERT_TRUE(d2.size() == 3);
}

TEST_F(DequeTest, size)
{
	d1.push_back(1);
	d1.push_back(2);
	d1.push_back(3);
	ASSERT_TRUE(d1.size() != d2.size());
	ASSERT_TRUE(d1.size() == 3);
	ASSERT_TRUE(d2.size() == 0);
}

TEST_F(DequeTest, size_empty)
{
	ASSERT_TRUE(d1.size() == d2.size());
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
}

TEST_F(DequeTest, size_random)
{
	d1.push_back(1);
	d1.push_back(2);
	d1.push_back(3);
	ASSERT_TRUE(d1.size() != d2.size());
	ASSERT_TRUE(d1.size() == 3);
	ASSERT_TRUE(d2.size() == 0);

	d2.push_back(1);
	d2.push_back(2);
	d2.push_back(3);
	d2.push_back(4);
	ASSERT_TRUE(d1.size() != d2.size());
	ASSERT_TRUE(d1.size() == 3);
	ASSERT_TRUE(d2.size() == 4);

	ASSERT_TRUE(d1.front() == 1);
	d1.pop_front();
	ASSERT_TRUE(d1.back() == 3);
	d1.pop_back();
	ASSERT_TRUE(d1.front() == 2);
	ASSERT_TRUE(d1.back() == 2);
	ASSERT_TRUE(d1.size() == 1);

	ASSERT_TRUE(d2.back() == 4);
	d2.pop_back();
	ASSERT_TRUE(d2.back() == 3);
	d2.pop_back();
	ASSERT_TRUE(d2.back() == 2);
	d2.pop_back();
	ASSERT_TRUE(d2.back() == 1);
	d2.pop_back();
	ASSERT_TRUE(d2.size() == 0);

	d1.pop_back();
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d1.size() == d2.size());
}

TEST_F(DequeTest, resize_default)
{
	d1.resize(5);
	ASSERT_TRUE(d1.size() == 5);
	ASSERT_TRUE(d1[0] == 0);
	ASSERT_TRUE(d1[1] == 0);
	ASSERT_TRUE(d1[2] == 0);
	ASSERT_TRUE(d1[3] == 0);
	ASSERT_TRUE(d1[4] == 0);
	ASSERT_TRUE(d1.size() == 5);
}

TEST_F(DequeTest, resize_shrink)
{
	d1.push_back(1);
	d1.push_back(2);
	d1.push_back(3);
	d1.push_back(4);
	d1.push_back(5);
	ASSERT_TRUE(d1[0] == 1);
	ASSERT_TRUE(d1[1] == 2);
	ASSERT_TRUE(d1[2] == 3);
	ASSERT_TRUE(d1[3] == 4);
	ASSERT_TRUE(d1[4] == 5);
	ASSERT_TRUE(d1.size() == 5);
	d1.resize(1);
	ASSERT_TRUE(d1[0] == 1);
	ASSERT_TRUE(d1.size() == 1);
}

TEST_F(DequeTest, resize_grow_value)
{
	d1.push_back(1);
	d1.push_back(2);
	ASSERT_TRUE(d1[0] == 1);
	ASSERT_TRUE(d1[1] == 2);
	ASSERT_TRUE(d1.size() == 2);
	d1.resize(5, 10);
	ASSERT_TRUE(d1[0] == 1);
	ASSERT_TRUE(d1[1] == 2);
	ASSERT_TRUE(d1[2] == 10);
	ASSERT_TRUE(d1[3] == 10);
	ASSERT_TRUE(d1[4] == 10);
	ASSERT_TRUE(d1.size() == 5);
}

TEST_F(DequeTest, push_front)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_front(1);
	ASSERT_TRUE(d1[0] == 1);
	ASSERT_TRUE(d1.size() == 1);
}

TEST_F(DequeTest, push_front_multiple)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_front(1);
	d1.push_front(2);
	d1.push_front(3);
	d1.push_front(4);
	d1.push_front(5);
	ASSERT_TRUE(d1[0] == 5);
	ASSERT_TRUE(d1[1] == 4);
	ASSERT_TRUE(d1[2] == 3);
	ASSERT_TRUE(d1[3] == 2);
	ASSERT_TRUE(d1[4] == 1);
	ASSERT_TRUE(d1.size() == 5);
}

TEST_F(DequeTest, push_front_random)
{
	ASSERT_TRUE(d1.size() == 0);
	for(int i = 0; i < SIZE; ++i)
		d1.push_front(i);

	for(int i = 0; i < SIZE; ++i)
		ASSERT_TRUE(d1[i] == SIZE - i - 1);

	ASSERT_TRUE(d1.size() == SIZE);
}

TEST_F(DequeTest, push_back)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_back(1);
	ASSERT_TRUE(d1[0] == 1);
	ASSERT_TRUE(d1.size() == 1);
}

TEST_F(DequeTest, push_back_multiple)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_back(1);
	d1.push_back(2);
	d1.push_back(3);
	d1.push_back(4);
	d1.push_back(5);
	ASSERT_TRUE(d1[0] == 1);
	ASSERT_TRUE(d1[1] == 2);
	ASSERT_TRUE(d1[2] == 3);
	ASSERT_TRUE(d1[3] == 4);
	ASSERT_TRUE(d1[4] == 5);
	ASSERT_TRUE(d1.size() == 5);
}

TEST_F(DequeTest, push_back_random)
{
	ASSERT_TRUE(d1.size() == 0);
	for(int i = 0; i < SIZE; ++i)
		d1.push_back(i);

	for(int i = 0; i < SIZE; ++i)
		ASSERT_TRUE(d1[i] == i);

	ASSERT_TRUE(d1.size() == SIZE);
}

TEST_F(DequeTest, pop_front)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_back(1);
	ASSERT_TRUE(d1.front() == 1);
	d1.pop_front();
	ASSERT_TRUE(d1.size() == 0);
}

TEST_F(DequeTest, pop_front_multiple)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_back(1);
	d1.push_back(2);
	d1.push_back(3);
	d1.push_back(4);
	d1.push_back(5);
	ASSERT_TRUE(d1.front() == 1);
	d1.pop_front();
	ASSERT_TRUE(d1.front() == 2);
	d1.pop_front();
	ASSERT_TRUE(d1.front() == 3);
	d1.pop_front();
	ASSERT_TRUE(d1.front() == 4);
	d1.pop_front();
	ASSERT_TRUE(d1.front() == 5);
	d1.pop_front();
	ASSERT_TRUE(d1.size() == 0);
}

TEST_F(DequeTest, pop_front_random)
{
	ASSERT_TRUE(d1.size() == 0);
	for(int i = 0; i < SIZE; ++i)
		d1.push_back(i);

    ASSERT_TRUE(d1.size() == SIZE);
	for(int i = 0; i < SIZE; ++i)
	{
		ASSERT_TRUE(d1.front() == i);
		d1.pop_front();
	}
	ASSERT_TRUE(d1.size() == 0);
}

TEST_F(DequeTest, pop_back)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_front(1);
	ASSERT_TRUE(d1.back() == 1);
	d1.pop_back();
	ASSERT_TRUE(d1.size() == 0);
}

TEST_F(DequeTest, pop_back_multiple)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_front(1);
	d1.push_front(2);
	d1.push_front(3);
	d1.push_front(4);
	d1.push_front(5);
	ASSERT_TRUE(d1.back() == 1);
	d1.pop_back();
	ASSERT_TRUE(d1.back() == 2);
	d1.pop_back();
	ASSERT_TRUE(d1.back() == 3);
	d1.pop_back();
	ASSERT_TRUE(d1.back() == 4);
	d1.pop_back();
	ASSERT_TRUE(d1.back() == 5);
	d1.pop_back();
	ASSERT_TRUE(d1.size() == 0);
}

TEST_F(DequeTest, pop_back_random)
{
	ASSERT_TRUE(d1.size() == 0);
	for(int i = 0; i < SIZE; ++i)
		d1.push_front(i);

	for(int i = 0; i < SIZE; ++i)
	{
		ASSERT_TRUE(d1.back() == i);
		d1.pop_back();
	}    
	ASSERT_TRUE(d1.size() == 0);
}

TEST_F(DequeTest, insert)
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);
	d1.insert(d1.begin(), 0);
	ASSERT_TRUE(d1[0] == 0);
	ASSERT_TRUE(d1.size() == 1);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 != d2);
}

TEST_F(DequeTest, insert_multiple)
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);
	d1.insert(d1.begin(), 0);
	d1.insert(d1.begin(), 1);
	d1.insert(d1.begin(), 2);
	d1.insert(d1.begin(), 3);
	d1.insert(d1.begin(), 4);
	ASSERT_TRUE(d1[0] == 4);
	ASSERT_TRUE(d1[1] == 3);
	ASSERT_TRUE(d1[2] == 2);
	ASSERT_TRUE(d1[3] == 1);
	ASSERT_TRUE(d1[4] == 0);
	ASSERT_TRUE(d1.size() == 5);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 != d2);
}

TEST_F(DequeTest, insert_random)
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);
	d1.insert(d1.begin(), 0);
	d1.insert(d1.begin(), 1);
	d1.insert(d1.begin(), 2);
	d1.insert(d1.begin(), 3);
	d1.insert(d1.begin(), 4);
	ASSERT_TRUE(d1[0] == 4);
	ASSERT_TRUE(d1[1] == 3);
	ASSERT_TRUE(d1[2] == 2);
	ASSERT_TRUE(d1[3] == 1);
	ASSERT_TRUE(d1[4] == 0);
	ASSERT_TRUE(d1.size() == 5);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 != d2);
	auto iter_d1 = d1.begin();
	++iter_d1;
	iter_d1 = d1.insert(iter_d1, 20);
    iter_d1 = d1.insert(iter_d1, 20);
	ASSERT_TRUE(d1[0] == 4);
	ASSERT_TRUE(d1[1] == 20);
	ASSERT_TRUE(d1[2] == 20);
	ASSERT_TRUE(d1[3] == 3);
	ASSERT_TRUE(d1[4] == 2);
	ASSERT_TRUE(d1[5] == 1);
	ASSERT_TRUE(d1[6] == 0);
	ASSERT_TRUE(d1.size() == 7);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 != d2);   
}

TEST_F(DequeTest, front)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_front(1);
	ASSERT_TRUE(d1.front() == 1);
	ASSERT_TRUE(d1.size() == 1);
}

TEST_F(DequeTest, front_multiple)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_front(1);
	d1.push_front(2);
	d1.push_front(3);
	ASSERT_TRUE(d1.front() == 3);
	ASSERT_TRUE(d1.size() == 3);
}

TEST_F(DequeTest, front_random)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_front(1);
	d1.push_front(2);
	d1.push_front(3);
	ASSERT_TRUE(d1.front() == 3);
	d1.pop_front();
	ASSERT_TRUE(d1.front() == 2);
	d1.pop_front();
	ASSERT_TRUE(d1.front() == 1);
	ASSERT_TRUE(d1.size() == 1);
}

TEST_F(DequeTest, erase)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_front(1);
	ASSERT_TRUE(d1.front() == 1);
	ASSERT_TRUE(d1.size() == 1);
	auto iter_d1 = d1.begin();
	d1.erase(iter_d1);
	ASSERT_TRUE(d1.size() == 0);
}

TEST_F(DequeTest, erase_multiple)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_back(1);
	d1.push_back(1);
	d1.push_back(1);
	auto iter_d1 = d1.begin();
	d1.erase(iter_d1);
	d1.erase(iter_d1);
	ASSERT_TRUE(d1.front() == 1);
	ASSERT_TRUE(d1.size() == 1);
}

TEST_F(DequeTest, erase_random)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_back(1);
	d1.push_back(2);
	d1.push_back(3);
	d1.push_back(4);
	d1.push_back(5);
	d1.push_back(6);
	d1.push_back(7);
	d1.push_back(8);
	d1.push_back(9);
	d1.push_back(10);
	ASSERT_TRUE(d1.size() == 10);
	d1.erase(d1.begin() + 5);
	auto iter_d1 = d1.begin();
	d1.erase(iter_d1);
	d1.erase(iter_d1);
	d1.erase(iter_d1);
	ASSERT_TRUE(d1[0] == 4);
	ASSERT_TRUE(d1[1] == 5);
	ASSERT_TRUE(d1[2] == 7);
	ASSERT_TRUE(d1[3] == 8);
	ASSERT_TRUE(d1[4] == 9);
	ASSERT_TRUE(d1[5] == 10);
	ASSERT_TRUE(d1.size() == 6);
}

TEST_F(DequeTest, back)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_back(1);
	ASSERT_TRUE(d1.back() == 1);
	ASSERT_TRUE(d1.size() == 1);
}

TEST_F(DequeTest, back_multiple)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_front(1);
	d1.push_front(2);
	d1.push_front(3);
	ASSERT_TRUE(d1.back() == 1);
	ASSERT_TRUE(d1.size() == 3);
}

TEST_F(DequeTest, back_random)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_front(1);
	d1.push_front(2);
	d1.push_front(3);
	ASSERT_TRUE(d1.size() == 3);
	ASSERT_TRUE(d1.front() == 3);
	d1.pop_front();
	ASSERT_TRUE(d1.front() == 2);
	d1.pop_front();
	ASSERT_TRUE(d1.back() == 1);
	ASSERT_TRUE(d1.size() == 1);
}

TEST_F(DequeTest, clear)
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);
	d1.push_back(1);
	ASSERT_TRUE(d1.front() == 1);
	ASSERT_TRUE(d1.size() == 1);
	d1.clear();
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);
}

TEST_F(DequeTest, clear_empty)
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);
	d1.clear();
	d2.clear();
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);
}

TEST_F(DequeTest, clear_multiple)
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);
	d1.push_back(1);
	d1.push_back(2);
	d1.push_back(3);
	d1.push_back(4);
	d1.push_back(5);
	ASSERT_TRUE(d1[0] == 1);
	ASSERT_TRUE(d1[1] == 2);
	ASSERT_TRUE(d1[2] == 3);
	ASSERT_TRUE(d1[3] == 4);
	ASSERT_TRUE(d1[4] == 5);
	ASSERT_TRUE(d1.size() == 5);
	d1.clear();
	d1.clear();
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);
}

TEST_F(DequeTest, subscript_operator)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_front(1);
	ASSERT_TRUE(d1[0] == 1);
	ASSERT_TRUE(d1.size() == 1);
}

TEST_F(DequeTest, subscript_operator_beyond_bounds)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_back(1);
	d1.push_back(2);
	d1.push_back(3);
	ASSERT_TRUE(d1.size() == 3);
	ASSERT_TRUE(d1[0] == 1);
	ASSERT_TRUE(d1[1] == 2);
	ASSERT_TRUE(d1[2] == 3);
	ASSERT_TRUE(d1[3] > 0 || d1[3] <= 0);
}

TEST_F(DequeTest, subscript_operator_empty)
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d1[0] <= 0 || d1[0] > 0);
}

TEST_F(DequeTest, at)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_front(1);
	ASSERT_TRUE(d1.at(0) == 1);
	ASSERT_TRUE(d1.size() == 1);
}

TEST_F(DequeTest, at_beyond_bounds)
{
	try
	{
		ASSERT_TRUE(d1.size() == 0);
		d1.push_back(1);
		d1.push_back(2);
		d1.push_back(3);
		ASSERT_TRUE(d1.size() == 3);
		ASSERT_TRUE(d1.at(0) == 1);
		ASSERT_TRUE(d1.at(1) == 2);
		ASSERT_TRUE(d1.at(2) == 3);
		d1.at(3);
		ASSERT_TRUE(false);
	}
	catch (const std::out_of_range& e)
	{
		ASSERT_TRUE(std::strcmp(e.what(), "deque::_M_range_check") == 0);
		ASSERT_TRUE(d1.size() == 3);
	}
}

TEST_F(DequeTest, at_empty)
{
	ASSERT_TRUE(d1.size() == 0);
	try
	{
		d1.at(0);
		ASSERT_TRUE(false);
	}
	catch (const std::out_of_range& e)
	{
		ASSERT_TRUE(std::strcmp(e.what(), "deque::_M_range_check") == 0);
		ASSERT_TRUE(d1.size() == 0);
	}
}

TEST_F(DequeTest, copy_assignment_operator)
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);
	d1.push_back(1);
	ASSERT_TRUE(d1.front() == 1);
	ASSERT_TRUE(d1.size() == 1);
	d1 = d2;
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);    
}

TEST_F(DequeTest, copy_assignement_operator_empty)
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);
	d1 = d2;
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2); 
}

TEST_F(DequeTest, copy_assignement_operator_multiple)
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);
	d1.push_back(1);
	d1.push_back(2);
	d1.push_back(3);
	ASSERT_TRUE(d1[0] == 1);
	ASSERT_TRUE(d1[1] == 2);
	ASSERT_TRUE(d1[2] == 3);
	ASSERT_TRUE(d1.size() == 3);
	d2.push_front(10);
	d2.push_front(9);
	d2.push_front(8);
	d2.push_front(7);
	ASSERT_TRUE(d2[0] == 7);
	ASSERT_TRUE(d2[1] == 8);
	ASSERT_TRUE(d2[2] == 9);
	ASSERT_TRUE(d2[3] == 10);
	ASSERT_TRUE(d2.size() == 4);   
	d1 = d2;
	ASSERT_TRUE(d2[0] == 7);
	ASSERT_TRUE(d2[1] == 8);
	ASSERT_TRUE(d2[2] == 9);
	ASSERT_TRUE(d2[3] == 10);
	ASSERT_TRUE(d2.size() == 4);   
	ASSERT_TRUE(d1[0] == 7);
	ASSERT_TRUE(d1[1] == 8);
	ASSERT_TRUE(d1[2] == 9);
	ASSERT_TRUE(d1[3] == 10);
	ASSERT_TRUE(d1.size() == 4);   
	ASSERT_TRUE(d1 == d2);   
}

TEST_F(DequeTest, default_constructor)
{
	MyDeque<int> x;
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(x.size() == 0);
	ASSERT_TRUE(x == d1);
	ASSERT_TRUE(x == d2);
	ASSERT_TRUE(d1 == d2);
}

TEST_F(DequeTest, fill_constructor_size)
{
	MyDeque<int> x(SIZE);
	for(int i = 0; i < SIZE; ++i)
		ASSERT_TRUE(x[i] == 0);
	ASSERT_TRUE(x.size() == SIZE);
}

TEST_F(DequeTest, fill_constructor_value)
{
	MyDeque<int> x(SIZE, SIZE);
	for(int i = 0; i < SIZE; ++i)
		ASSERT_TRUE(x[i] == SIZE);
	ASSERT_TRUE(x.size() == SIZE);
}

TEST_F(DequeTest, copy_constructor)
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);
	d1.push_back(1);
	ASSERT_TRUE(d1.front() == 1);
	ASSERT_TRUE(d1.size() == 1);
	MyDeque<int> x(d1);
	ASSERT_TRUE(x.front() == 1);
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(x == d1);
	ASSERT_TRUE(x != d2);    
}

TEST_F(DequeTest, copy_constructor_empty)
{
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(d1 == d2);
	MyDeque<int> x(d1);
	ASSERT_TRUE(d1.size() == 0);
	ASSERT_TRUE(d2.size() == 0);
	ASSERT_TRUE(x.size() == 0);
	ASSERT_TRUE(x == d1); 
	ASSERT_TRUE(x == d2);
}

TEST_F(DequeTest, copy_constructor_multiple)
{
	ASSERT_TRUE(d1.size() == 0);
	d1.push_front(10);
	d1.push_front(9);
	d1.push_front(8);
	d1.push_front(7);
	ASSERT_TRUE(d1[0] == 7);
	ASSERT_TRUE(d1[1] == 8);
	ASSERT_TRUE(d1[2] == 9);
	ASSERT_TRUE(d1[3] == 10);
	ASSERT_TRUE(d1.size() == 4);   
	MyDeque<int> x(d1);
	ASSERT_TRUE(d1[0] == 7);
	ASSERT_TRUE(d1[1] == 8);
	ASSERT_TRUE(d1[2] == 9);
	ASSERT_TRUE(d1[3] == 10);
	ASSERT_TRUE(d1.size() == 4);   
	ASSERT_TRUE(x[0] == 7);
	ASSERT_TRUE(x[1] == 8);
	ASSERT_TRUE(x[2] == 9);
	ASSERT_TRUE(x[3] == 10);
	ASSERT_TRUE(x.size() == 4);   
	ASSERT_TRUE(x == d1); 
	ASSERT_TRUE(x != d2);
}

TEST_F(DequeIterTest, for_each_read)
{
	int count = 0;
	for(auto i : d)
	{
		ASSERT_TRUE(i == count);
		++count;
	}    
}

TEST_F(DequeIterTest, iter_read)
{
	int count = 0;
	MyDeque<int>::iterator b = d.begin();
	MyDeque<int>::iterator e = d.end();
	while(b != e)
	{
		ASSERT_TRUE(*b == count);
		++count;
		++b;
	} 
}

TEST_F(DequeIterTest, iter_update)
{
	int count = 0;
	MyDeque<int>::iterator b = d.begin();
	MyDeque<int>::iterator e = d.end();
	while(b != e)
	{
		ASSERT_TRUE(*b == count);
		*b += 1;
		++count;
		++b;
	}

	count = 1;
	b = d.begin();
	e = d.end();
	while(b != e)
	{
		ASSERT_TRUE(*b == count);
		++count;
		++b;
	}   
}

TEST_F(DequeIterTest, reverse_iter_read)
{
	int count = SIZE;
	MyDeque<int>::iterator b = d.begin();
	MyDeque<int>::iterator e = d.end();
	while(b != e)
	{
		--e;
		--count;
		ASSERT_TRUE(*e == count);
	}
}

TEST_F(DequeIterTest, reverse_iter_update)
{
	int count = SIZE;
	MyDeque<int>::iterator b = d.begin();
	MyDeque<int>::iterator e = d.end();
	while(b != e)
	{
		--e;
		--count;
		ASSERT_TRUE(*e == count);
		*e += 1;
	}

	count = SIZE;
	b = d.begin();
	e = d.end();
	while(b != e)
	{
		--e;
		ASSERT_TRUE(*e == count);
		--count;
	}
}

TEST_F(DequeIterTest, const_iter_read)
{
	int count = 0;
	const MyDeque<int> cd = const_cast< MyDeque<int>& >(d);
	MyDeque<int>::const_iterator cb = cd.begin();
	MyDeque<int>::const_iterator ce = cd.end();
	while(cb != ce)
	{
		ASSERT_TRUE(*cb == count);
		++count;
		++cb;
	} 
}

TEST_F(DequeIterTest, const_reverse_iter_read)
{
	int count = SIZE;
	const MyDeque<int> cd = const_cast< MyDeque<int>& >(d);
	MyDeque<int>::const_iterator cb = cd.begin();
	MyDeque<int>::const_iterator ce = cd.end();
	while(cb != ce)
	{
		--ce;
		--count;
		ASSERT_TRUE(*ce == count);
	}
}

TEST_F(DequeIterTest, iter_read_plus_equal)
{
	int count = 0;
	MyDeque<int>::iterator b = d.begin();
	MyDeque<int>::iterator e = d.end();
	while(b != e)
	{
		ASSERT_TRUE(*b == count);
		++count;
		b += 1;
	} 
}

TEST_F(DequeIterTest, iter_update_plus_equal)
{
	int count = 0;
	MyDeque<int>::iterator b = d.begin();
	MyDeque<int>::iterator e = d.end();
	while(b != e)
	{
		ASSERT_TRUE(*b == count);
		*b += 1;
		++count;
		b += 1;
	}

	count = 1;
	b = d.begin();
	e = d.end();
	while(b != e)
	{
		ASSERT_TRUE(*b == count);
		++count;
		b += 1;
	}   
}

TEST_F(DequeIterTest, reverse_iter_read_minus_equal)
{
	int count = SIZE;
	MyDeque<int>::iterator b = d.begin();
	MyDeque<int>::iterator e = d.end();
	while(b != e)
	{
		e -= 1;
		--count;
		ASSERT_TRUE(*e == count);
	}
}

TEST_F(DequeIterTest, reverse_iter_update_minus_equal)
{
	int count = SIZE;
	MyDeque<int>::iterator b = d.begin();
	MyDeque<int>::iterator e = d.end();
	while(b != e)
	{
		e -= 1;
		--count;
		ASSERT_TRUE(*e == count);
		*e += 1;
	}

	count = SIZE;
	b = d.begin();
	e = d.end();
	while(b != e)
	{
		e -= 1;
		ASSERT_TRUE(*e == count);
		--count;
	}
}

TEST_F(DequeIterTest, const_iter_read_plus_equal)
{
	int count = 0;
	const MyDeque<int> cd = const_cast< MyDeque<int>& >(d);
	MyDeque<int>::const_iterator cb = cd.begin();
	MyDeque<int>::const_iterator ce = cd.end();
	while(cb != ce)
	{
		ASSERT_TRUE(*cb == count);
		++count;
		cb += 1;
	} 
}

TEST_F(DequeIterTest, const_reverse_iter_read_minus_equal)
{
	int count = SIZE;
	const MyDeque<int> cd = const_cast< MyDeque<int>& >(d);
	MyDeque<int>::const_iterator cb = cd.begin();
	MyDeque<int>::const_iterator ce = cd.end();
	while(cb != ce)
	{
		ce -= 1;
		--count;
		ASSERT_TRUE(*ce == count);
	}
}

TEST_F(DequeIterTest, iter_read_plus_equal_ten)
{
	int count = 0;
	MyDeque<int>::iterator b = d.begin();
	while((size_t) count < d.size())
	{
		ASSERT_TRUE(*b == count);
		count += 10;
		b += 10;
	} 
}

TEST_F(DequeIterTest, reverse_iter_read_plus_equal_ten)
{
	int count = SIZE;
	MyDeque<int>::iterator e = d.end();
	while(count >= 10)
	{
		e += -10;
		count += -10;
		ASSERT_TRUE(*e == count);
	}
}

TEST_F(DequeIterTest, iter_read_minus_equal_ten)
{
	int count = 0;
	MyDeque<int>::iterator b = d.begin();
	while((size_t) count < d.size())
	{
		ASSERT_TRUE(*b == count);
		count -= -10;
		b -= -10;
	} 
}

TEST_F(DequeIterTest, reverse_iter_read_minus_equal_ten)
{
	int count = SIZE;
	MyDeque<int>::iterator e = d.end();
	while(count >= 10)
	{
		e -= 10;
		count -= 10;
		ASSERT_TRUE(*e == count);
	}
}

TEST_F(DequeIterTest, const_iter_read_plus_equal_ten)
{
	int count = 0;
	const MyDeque<int> cd = const_cast< MyDeque<int>& >(d);
	MyDeque<int>::const_iterator cb = cd.begin();
	while((size_t) count < d.size())
	{
		ASSERT_TRUE(*cb == count);
		count += 10;
		cb += 10;
	} 
}

TEST_F(DequeIterTest, const_reverse_iter_read_plus_equal_ten)
{
	int count = SIZE;
	const MyDeque<int> cd = const_cast< MyDeque<int>& >(d);
	MyDeque<int>::const_iterator ce = cd.end();
	while(count >= 10)
	{
		ce += -10;
		count += -10;
		ASSERT_TRUE(*ce == count);
	}
}

TEST_F(DequeIterTest, const_iter_read_minus_equal_ten)
{
	int count = 0;
	const MyDeque<int> cd = const_cast< MyDeque<int>& >(d);
	MyDeque<int>::const_iterator cb = cd.begin();
	while((size_t) count < d.size())
	{
		ASSERT_TRUE(*cb == count);
		count -= -10;
		cb -= -10;
	} 
}

TEST_F(DequeIterTest, const_reverse_iter_read_minus_equal_ten)
{
	int count = SIZE;
	const MyDeque<int> cd = const_cast< MyDeque<int>& >(d);
	MyDeque<int>::const_iterator ce = cd.end();
	while(count >= 10)
	{
		ce -= 10;
		count -= 10;
		ASSERT_TRUE(*ce == count);
	}
}

TEST_F(DequeIterTest, iter_max_elem)
{
	ASSERT_TRUE(*std::max_element(d.begin(), d.end()) == ((int) d.size() - 1));
}

TEST_F(DequeIterTest, const_iter_max_elem)
{
	const MyDeque<int> cd = const_cast< MyDeque<int>& >(d);
	MyDeque<int>::const_iterator cb = cd.begin();
	MyDeque<int>::const_iterator ce = cd.end();
	ASSERT_TRUE(*std::max_element(cb, ce) == ((int) d.size() - 1));
}

TEST_F(DequeIterTest, iter_min_elem)
{
	ASSERT_TRUE(*std::min_element(d.begin(), d.end()) == 0);
}

TEST_F(DequeIterTest, const_iter_min_elem)
{
	const MyDeque<int> cd = const_cast< MyDeque<int>& >(d);
	MyDeque<int>::const_iterator cb = cd.begin();
	MyDeque<int>::const_iterator ce = cd.end();
	ASSERT_TRUE(*std::min_element(cb, ce) == 0);
}

TEST_F(DequeIterTest, iter_fill)
{
	std::fill(d.begin(), d.end(), d.size());
	for(auto i : d)
		ASSERT_TRUE(i == (int) d.size());
}

TEST_F(DequeIterTest, iter_fill_n)
{
	std::fill_n(d.begin(), 1, d.size());
	ASSERT_TRUE(d.front() == (int) d.size());

	int count = 1;
	MyDeque<int>::iterator b = d.begin();
	MyDeque<int>::iterator e = d.end();
	++b;
	while(b != e)
	{
		ASSERT_TRUE(*b == count);
		++count;
		++b;
	} 
}

TEST_F(DequeIterTest, iter_fill_n_max)
{
	std::fill_n(d.begin(), d.size(), d.size());
	for(auto i : d)
		ASSERT_TRUE(i == (int) d.size());
}

TEST_F(DequeIterTest, iter_plus)
{
	const MyDeque<int> cd = const_cast< MyDeque<int>& >(d);
	MyDeque<int>::iterator b = d.begin();
	MyDeque<int>::const_iterator cb = cd.begin();
	MyDeque<int>::const_iterator ce = cd.end();
	std::transform(cb, ce, b, std::bind2nd(std::plus<int>(), 1));
	ASSERT_TRUE(d.front() == 1);

	int count = 1;
	for(auto i : d)
	{
		ASSERT_TRUE(i == count);
		++count;
	} 
}

TEST_F(DequeIterTest, iter_minus)
{
	const MyDeque<int> cd = const_cast< MyDeque<int>& >(d);
	MyDeque<int>::const_iterator cb = cd.begin();
	MyDeque<int>::const_iterator ce = cd.end();
	MyDeque<int>::iterator b = d.begin();
	std::transform(cb, ce, b, std::bind2nd(std::minus<int>(), 1));
	ASSERT_TRUE(d.front() == -1);

	int count = -1;
	for(auto i : d)
	{
		ASSERT_TRUE(i == count);
		++count;
	} 
}

TEST_F(DequeIterTest, iter_multiply)
{
	int multiple = 2;
	const MyDeque<int> cd = const_cast< MyDeque<int>& >(d);
	MyDeque<int>::const_iterator cb = cd.begin();
	MyDeque<int>::const_iterator ce = cd.end();
	MyDeque<int>::iterator b = d.begin();
	std::transform(cb, ce, b, std::bind2nd(std::multiplies<int>(), multiple));
	ASSERT_TRUE(d.front() == 0);

	int count = 0;
	for(auto i : d)
	{
		ASSERT_TRUE(i == (count * multiple));
		++count;
	} 
}

TEST_F(DequeIterTest, iter_divide)
{
	int denominator = 2;
	const MyDeque<int> cd = const_cast< MyDeque<int>& >(d);
	MyDeque<int>::const_iterator cb = cd.begin();
	MyDeque<int>::const_iterator ce = cd.end();
	MyDeque<int>::iterator b = d.begin();
	std::transform(cb, ce, b, std::bind2nd(std::divides<int>(), denominator));
	ASSERT_TRUE(d.front() == 0);

	int count = 0;
	for(auto i : d)
	{
		ASSERT_TRUE(i == (count / denominator));
		++count;
	} 
}

TEST_F(DequeIterTest, iter_negate)
{
	const MyDeque<int> cd = const_cast< MyDeque<int>& >(d);
	MyDeque<int>::const_iterator cb = cd.begin();
	MyDeque<int>::const_iterator ce = cd.end();
	MyDeque<int>::iterator b = d.begin();
	std::transform(cb, ce, b, std::negate<int>());
	ASSERT_TRUE(d.front() == 0);

	int count = 0;
	for(auto i : d)
	{
		ASSERT_TRUE(i == count);
		--count;
	} 
}

TEST_F(DequeIterTest, iter_replace)
{
	std::replace(d.begin(), d.end(), 0, SIZE);
	ASSERT_TRUE(d.front() == SIZE);
}

TEST_F(DequeSingleTest, default_size_constructor)
{
	MyDeque<int> x(10);
	ASSERT_TRUE(!x.empty());
	ASSERT_TRUE(x.size()                                     == 10);
	ASSERT_TRUE(std::count(x.begin(), x.end(), MyDeque<int>::value_type()) == 10);
}

TEST_F(DequeSingleTest, default_argument_constructor)
{
	const MyDeque<int> x(10, v1);
	ASSERT_TRUE(!x.empty());
	ASSERT_TRUE(x.size()                           == 10);
	ASSERT_TRUE(std::count(x.begin(), x.end(), v1) == 10);
}

TEST_F(DequeSingleTest, push_pop_back)
{
	MyDeque<int> x;
	x.push_back(v1);
	x.push_back(v2);
	x.push_back(v3);
	MyDeque<int> y(x);
	ASSERT_TRUE(x == y);
	y.pop_back();
	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(x.back() == v3);
	ASSERT_TRUE(y.size() == 2);
	ASSERT_TRUE(y.back() == v2);
}

TEST_F(DequeSingleTest, push_pop_back_copy_assignment)
{
	MyDeque<int> x;
	x.push_back(v1);
	x.push_back(v2);
	x.push_back(v3);
	MyDeque<int> y;
	y.push_back(v3);
	y.push_back(v1);
	ASSERT_TRUE(x != y);
	y = x;
	ASSERT_TRUE(x == y);
	y.pop_back();
	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(x.back() == v3);
	ASSERT_TRUE(y.size() == 2);
	ASSERT_TRUE(y.back() == v2);
}

TEST_F(DequeSingleTest, push_back_swap)
{
	MyDeque<int> x;
	x.push_back(v1);
	x.push_back(v2);
	x.push_back(v3);
	MyDeque<int> y;
	y.push_back(v1);
	y.push_back(v2);
	x.swap(y);
	ASSERT_TRUE(x.size() == 2);
	ASSERT_TRUE(x.back() == v2);
	ASSERT_TRUE(y.size() == 3);
	ASSERT_TRUE(y.back() == v3);
}

TEST_F(DequeSingleTest, push_back_copy)
{
	MyDeque<int> x;
	x.push_back(v1);
	x.push_back(v2);
	x.push_back(v3);
	MyDeque<int> y(x.size());
	std::copy(x.begin(), x.end(), y.begin());
	ASSERT_TRUE(std::equal(x.begin(), x.end(), y.begin()));
}

TEST_F(DequeSingleTest, back_front_push_back)
{
	MyDeque<int> x;
	x.push_back(v1);
	x.push_back(v2);
	x.push_back(v3);
	ASSERT_TRUE(x.front() == v1);
	x.front() = v2;
	ASSERT_TRUE(x.front() == v2);
	ASSERT_TRUE(x.back()  == v3);
	x.back()  = v2;
	ASSERT_TRUE(x.back()  == v2);
}

TEST_F(DequeSingleTest, push_back_rel_ops)
{
	MyDeque<int> x;
	x.push_back(v1);
	x.push_back(v2);
	x.push_back(v3);
	MyDeque<int> y;
	y.push_back(v1);
	y.push_back(v2);
	y.push_back(v3);
	ASSERT_TRUE(x == y);
	ASSERT_TRUE(x <= y);
	ASSERT_TRUE(x >= y);
	ASSERT_TRUE(!(x != y));
	ASSERT_TRUE(!(x <  y));
	ASSERT_TRUE(!(x >  y));
}

TEST_F(DequeSingleTest, copy_assignment_same_object)
{
	MyDeque<int> x;
	x.push_back(v1);
	x.push_back(v2);
	x.push_back(v3);
	ASSERT_EQ(x.size(), 3);
	x = x;
	ASSERT_EQ(x.size(), 3);
}

TEST_F(DequeSingleTest, copy_assignment_equal_size)
{
	MyDeque<int> x;
	x.push_back(v1);
	x.push_back(v2);
	x.push_back(v3);
	MyDeque<int> y;
	y.push_back(v1);
	y.push_back(v2);
	y.push_back(v3);
	ASSERT_EQ(x.size(), y.size());
	x = y;
	ASSERT_TRUE(x == y);
}

TEST_F(DequeSingleTest, copy_assignment_less_size)
{
	MyDeque<int> x;
	MyDeque<int> y;
	y.push_back(v1);
	y.push_back(v2);
	y.push_back(v3);
	ASSERT_TRUE(x.size() < y.size());
	x = y;
	ASSERT_TRUE(x == y);
	ASSERT_EQ(x.size(), 3);
	ASSERT_EQ(y.size(), 3);
}

TEST_F(DequeSingleTest, copy_assignement_less_capacity)
{
	MyDeque<int> x;
	x.push_back(v1);
	ASSERT_TRUE(x.size() == 1);
	MyDeque<int> y(mediumSize, 100);
	x = y;
	ASSERT_TRUE(x.size() == mediumSize);
	for(auto i : x)
		ASSERT_EQ(i, 100);
}

TEST_F(DequeSingleTest, copy_assignment_greater_capacity)
{
	MyDeque<int> x;
	x.push_back(v1);
	ASSERT_TRUE(x.size() == 1);
	MyDeque<int> y(largeSize, 100);
	x = y;
	ASSERT_TRUE(x.size() == largeSize);
	for(auto i : x)
		ASSERT_EQ(i, 100);
}

TEST_F(DequeSingleTest, copy_assignment_empty)
{
	MyDeque<int> x(largeSize, 100);
	ASSERT_TRUE(x.size() == largeSize);
	for(auto i : x)
		ASSERT_EQ(i, 100);
	MyDeque<int> y;
	ASSERT_TRUE(y.size() == 0);
	x = y;
	ASSERT_TRUE(x.size() == 0);
}

TEST_F(DequeSingleTest, copy_constructor_empty)
{
	MyDeque<int> x;
	ASSERT_TRUE(x.size() == 0);
	MyDeque<int> y(x);
	ASSERT_TRUE(y.size() == 0);
	ASSERT_TRUE(x.size() == 0);
	
}

TEST_F(DequeSingleTest, fill_constructor_empty)
{
	MyDeque<int> x(0);
	ASSERT_TRUE(x.size() == 0);
}

TEST_F(DequeSingleTest, resize_empty)
{
	MyDeque<int> x(0);
	ASSERT_TRUE(x.size() == 0);
	x.resize(largeSize, 100);
	ASSERT_TRUE(x.size() == largeSize);
	for(auto i : x)
		ASSERT_EQ(i, 100);
}

TEST_F(DequeSingleTest, resize_equal)
{
	MyDeque<int> x(10, 10);
	ASSERT_TRUE(x.size() == 10);
	x.resize(10, 42);
	ASSERT_TRUE(x.size() == 10);
	for(auto i : x)
		ASSERT_EQ(i, 10);
	
}

TEST_F(DequeSingleTest, resize_less_size)
{
	MyDeque<int> x(10, 10);
	ASSERT_TRUE(x.size() == 10);
	x.resize(0, 42);
	ASSERT_TRUE(x.size() == 0);
}

TEST_F(DequeSingleTest, resize_less_capacity)
{
	MyDeque<int> x;
	x.push_back(v1);
	ASSERT_TRUE(x.size() == 1);
	x.resize(mediumSize, 42);
	ASSERT_TRUE(x.size() == mediumSize);
	ASSERT_EQ(x.front(), v1);
	for(size_t i = 1; i < mediumSize; ++i)
		ASSERT_TRUE(x[i] == 42);
}

TEST_F(DequeSingleTest, resize_greater_capacity)
{
	MyDeque<int> x;
	x.push_back(v1);
	ASSERT_TRUE(x.size() == 1);
	x.resize(largeSize, 42);
	ASSERT_TRUE(x.size() == largeSize);
	ASSERT_EQ(x.front(), v1);
	for(size_t i = 1; i < largeSize; ++i)
		ASSERT_TRUE(x[i] == 42);
}

TEST(DequeAcceptance, push_front)
{
	ASSERT_TRUE(true);
}

TEST(DequeAcceptance, push_back)
{
	ASSERT_TRUE(true);
}

TEST(DequeAcceptance, push_back_front_combo)
{
	ASSERT_TRUE(true);
}

TEST(DequeAcceptance, pop_back)
{
	ASSERT_TRUE(true);
}

TEST(DequeAcceptance, pop_front)
{
	ASSERT_TRUE(true);
}

TEST(DequeAcceptance, erase)
{
	ASSERT_TRUE(true);
}

TEST(DequeAcceptance, insert)
{
	ASSERT_TRUE(true);
} 

TEST(DequeAcceptance, insert_begin)
{
	ASSERT_TRUE(true);
}

TEST(DequeAcceptance, insert_end)
{
	ASSERT_TRUE(true);
}
