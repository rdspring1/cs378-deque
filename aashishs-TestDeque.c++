// ----------------------------
// projects/deque/TestDeque.c++
// Copyright (C) 2013
// Glenn P. Downing
// ----------------------------

/*
To test the program:
    % ls /usr/include/cppunit/
    ...
    TestFixture.h
    ...
    % g++ -pedantic -std=c++0x -Wall Deque.h TestDeque.c++ -o TestDeque -lgtest -lpthread -lgtest_main
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
#include <gtest/gtest.h>
#include "Deque.h"

// ---------
// TestDeque
// ---------

template <class C>
class DequeTest : public testing::Test {
protected:
    DequeTest() : aDequeLHS(C()) ,aDequeRHS(C()), aDequeT(C(5,5)) {}

    virtual void SetUp() { 

    }

    C blah(){
        return aDequeLHS;
    }

    virtual void TearDown() {
        // aDequeLHS.~C();
        // aDequeRHS.~C();
    }


    C aDequeLHS,aDequeRHS,aDequeT;
    typename C::iterator it;
    typename C::iterator itOther;
    typename C::const_iterator cit;
    typename C::const_iterator citOther;
    typename C::difference_type dt;
    typename C::size_type st;
};

using testing::Types;
typedef Types<std::deque<int>, MyDeque<int>, std::deque<short>, MyDeque<short>, std::deque<long>, MyDeque<long>, std::deque<unsigned>, MyDeque<unsigned> > Implementations;
TYPED_TEST_CASE(DequeTest, Implementations);

TYPED_TEST(DequeTest, valConstructor_1){
    ASSERT_EQ(this->aDequeT[0],5);
    ASSERT_EQ(this->aDequeT.size(),5);
}

TYPED_TEST(DequeTest, valConstructor_2){
    ASSERT_EQ(this->aDequeT[1],5);
    ASSERT_EQ(this->aDequeT.size(),5);
}

TYPED_TEST(DequeTest, valConstructor_3){
    ASSERT_EQ(this->aDequeT[2],5);
    ASSERT_EQ(this->aDequeT.size(),5);
}

TYPED_TEST(DequeTest, push_back) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->aDequeLHS.push_back(4);
    this->aDequeLHS.push_back(5);
    this->aDequeLHS.push_back(6);
    this->aDequeLHS.push_back(7);
    this->aDequeLHS.push_back(8);
    this->aDequeLHS.push_back(9);
    this->aDequeLHS.push_back(10);
    this->aDequeLHS.push_back(11);
    this->aDequeLHS.push_back(12);
    this->aDequeLHS.push_back(13);
    this->aDequeLHS.push_back(14);
    this->aDequeLHS.push_back(15);
    this->aDequeLHS.push_back(16);  
    this->aDequeLHS.push_back(17);
    this->aDequeLHS.push_back(18);
    ASSERT_EQ(this->aDequeLHS[0], 1);
    ASSERT_EQ(this->aDequeLHS[1], 2);
    this->aDequeLHS.clear();
}

TYPED_TEST(DequeTest, push_back_1) {
    this->aDequeLHS.push_front(1);
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->aDequeLHS.push_back(4);
    this->aDequeLHS.push_back(5);
    ASSERT_EQ(this->aDequeLHS[1], 1);
    ASSERT_EQ(this->aDequeLHS[5], 5);
    this->aDequeLHS.clear();
}

TYPED_TEST(DequeTest, push_back_2) {
    this->aDequeLHS.push_front(1);
    this->aDequeLHS.push_front(1);
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(3);
    this->aDequeLHS.push_back(4);
    this->aDequeLHS.push_back(5);
    ASSERT_EQ(this->aDequeLHS[2], 1);
    ASSERT_EQ(this->aDequeLHS[5], 5);
    this->aDequeLHS.clear();
}

TYPED_TEST(DequeTest, push_back_big) {
    for (int i = 0; i < 50; i++) {
        this->aDequeLHS.push_back(50);
    }
    for (int i = 0; i < 50; i++) {
        ASSERT_EQ(this->aDequeLHS[i], 50);
    }
} 

TYPED_TEST(DequeTest, push_front) {
    this->aDequeLHS.push_front(1);
    this->aDequeLHS.push_front(2);
    this->aDequeLHS.push_front(3);
    this->aDequeLHS.push_front(4);
    this->aDequeLHS.push_front(5);
    this->aDequeLHS.push_front(6);
    this->aDequeLHS.push_front(7);
    this->aDequeLHS.push_front(8);
    this->aDequeLHS.push_front(9);
    this->aDequeLHS.push_front(10);
    this->aDequeLHS.push_front(11);
    this->aDequeLHS.push_front(12);
    this->aDequeLHS.push_front(13);
    this->aDequeLHS.push_front(14);
    this->aDequeLHS.push_front(15);
    this->aDequeLHS.push_front(16);
    this->aDequeLHS.push_front(17);
    this->aDequeLHS.push_front(18);
    ASSERT_EQ(this->aDequeLHS[0], 18);
    ASSERT_EQ(this->aDequeLHS[1], 17);
} 

TYPED_TEST(DequeTest, push_front_big) {
    for (int i = 0; i < 50; i++) {
        this->aDequeLHS.push_front(50);
    }
    for (int i = 0; i < 50; i++) {
        ASSERT_EQ(this->aDequeLHS[i], 50);
    }
} 

TYPED_TEST(DequeTest, size) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    int size = this->aDequeLHS.size();
    ASSERT_EQ(size, 2);
}

TYPED_TEST(DequeTest, size_empty) {
    int size = this->aDequeLHS.size();
    ASSERT_EQ(size, 0);
}

TYPED_TEST(DequeTest, equal_equal) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);

    this->aDequeRHS.push_back(1);
    this->aDequeRHS.push_back(2);

    EXPECT_TRUE(this->aDequeLHS == this->aDequeRHS);
}  

TYPED_TEST(DequeTest, less_than) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);

    this->aDequeRHS.push_back(2);
    this->aDequeRHS.push_back(1);

    EXPECT_TRUE(this->aDequeLHS < this->aDequeRHS);
}

TYPED_TEST(DequeTest, starIt) {
    this->aDequeLHS.push_back(1);
    this->it = this->aDequeLHS.begin();
    EXPECT_TRUE(*(this->it) == 1);
}

TYPED_TEST(DequeTest, begin) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->it = this->aDequeLHS.begin();
    EXPECT_TRUE(*(this->it) == 1);
}

TYPED_TEST(DequeTest, equalIt) {
    this->aDequeLHS.push_back(1);
    this->it = this->aDequeLHS.begin();
    this->itOther = this->aDequeLHS.begin();
    EXPECT_TRUE(this->it == this->itOther);
}

TYPED_TEST(DequeTest, incrementIt) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->aDequeLHS.push_back(4);
    this->aDequeLHS.push_back(5);
    this->it = this->aDequeLHS.begin();
    this->it+=4;
    EXPECT_TRUE(*(this->it) == 5);
}

TYPED_TEST(DequeTest, decrementIt) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->it = this->aDequeLHS.end();
    --this->it;
    EXPECT_TRUE(*(this->it) == 3);
}

TYPED_TEST(DequeTest, plus_equalIt) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->it = this->aDequeLHS.begin();
    this->dt = 2;
    this->it += this->dt;
    EXPECT_TRUE(*(this->it) == 3);
}

TYPED_TEST(DequeTest, minus_equalIt) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->it = this->aDequeLHS.end();
    this->dt = 2;
    this->it -= this->dt;
    EXPECT_TRUE(*(this->it) == 2);
}

// TYPED_TEST(DequeTest, equalCIt) {
//    this->aDequeLHS.push_back(1);
//     this->cit = this->aDequeLHS.begin();
//     this->citOther = this->aDequeLHS.begin();
//     EXPECT_TRUE(this->cit == this->citOther);
// }

// No tests for const iterator

TYPED_TEST(DequeTest, equal) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->aDequeRHS = this->aDequeLHS;
    EXPECT_TRUE(&this->aDequeLHS != &this->aDequeRHS);
    EXPECT_TRUE(this->aDequeLHS == this->aDequeRHS);
}

TYPED_TEST(DequeTest, equal_noData) {
    EXPECT_TRUE(&this->aDequeLHS != &this->aDequeRHS);
    EXPECT_TRUE(this->aDequeLHS == this->aDequeRHS);
}

TYPED_TEST(DequeTest, index) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->st = 2;
    EXPECT_TRUE((this->aDequeLHS)[this->st] == 3);
}

TYPED_TEST(DequeTest, index_2) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_front(3);
    this->st = 2;
    ASSERT_EQ((this->aDequeLHS)[this->st],2);
}

TYPED_TEST(DequeTest, at_1) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->st = 2;
    EXPECT_TRUE((this->aDequeLHS).at(this->st) == 3);
}

TYPED_TEST(DequeTest, at_bad_over) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_front(3);
    try {
        this->st = 4;
        this->aDequeLHS.at(this->st);
        EXPECT_TRUE(false);
    } catch (std::out_of_range &e) {

    }
}

TYPED_TEST(DequeTest, at_bad_under) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_front(3);
    try {
        this->st = -1;
        this->aDequeLHS.at(this->st);
        EXPECT_TRUE(false);
    } catch (std::out_of_range &e) {

    }
}

TYPED_TEST(DequeTest, back) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    EXPECT_TRUE((this->aDequeLHS).back() == 3);
}

TYPED_TEST(DequeTest, clear) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->aDequeLHS.clear();
    EXPECT_TRUE((this->aDequeLHS).size() == 0);
}

TYPED_TEST(DequeTest, clear_noData) {
    this->aDequeLHS.clear();
    EXPECT_TRUE((this->aDequeLHS).size() == 0);
}

TYPED_TEST(DequeTest, end) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->it = this->aDequeLHS.end();
    --this->it;
    EXPECT_TRUE(*(this->it) == 3);
}

TYPED_TEST(DequeTest, erase) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->it = this->aDequeLHS.begin();
    this->it = (this->aDequeLHS).erase(this->it);
    EXPECT_TRUE(*(this->it) == 2);
}

TYPED_TEST(DequeTest, front) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    EXPECT_TRUE((this->aDequeLHS).front()==1);
}

TYPED_TEST(DequeTest, insert) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->it = this->aDequeLHS.begin();
    this->it = this->aDequeLHS.insert(this->it,5);
    EXPECT_TRUE(*(this->it)==5);
    EXPECT_TRUE((this->aDequeLHS)[0]==5);
}

TYPED_TEST(DequeTest, pop_back) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->aDequeLHS.pop_back();
    EXPECT_TRUE((this->aDequeLHS).size()==2);
    EXPECT_TRUE((this->aDequeLHS)[1]==2);
}

TYPED_TEST(DequeTest, pop_front) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->aDequeLHS.pop_front();
    EXPECT_TRUE((this->aDequeLHS).size()==2);
    EXPECT_TRUE((this->aDequeLHS)[0]==2);
}

TYPED_TEST(DequeTest, resize) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->st = 2;
    (this->aDequeLHS).resize(this->st);
    EXPECT_TRUE((this->aDequeLHS).size()==2);
    EXPECT_TRUE((this->aDequeLHS)[1]==2);
}

TYPED_TEST(DequeTest, resize_2) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    ASSERT_EQ((this->aDequeLHS).size(), 3);
    this->aDequeLHS.resize(1);
    ASSERT_EQ((this->aDequeLHS).size(), 1);
    this->aDequeLHS.push_back(2);
    ASSERT_EQ((this->aDequeLHS).size(), 2);    
}

TYPED_TEST(DequeTest, resize_no_data) {
    ASSERT_EQ((this->aDequeLHS).size(), 0);
    this->aDequeLHS.resize(1);
    ASSERT_EQ((this->aDequeLHS).size(), 1);
    this->aDequeLHS.resize(50);
    ASSERT_EQ((this->aDequeLHS).size(), 50);    
}

TYPED_TEST(DequeTest, swap) {
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    
    this->aDequeRHS.push_back(3);
    this->aDequeRHS.push_back(2);
    this->aDequeRHS.push_back(1);

    (this->aDequeLHS).swap(this->aDequeRHS);

    std::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    EXPECT_TRUE(std::equal((this->aDequeRHS).begin(),(this->aDequeRHS).end(),vec.begin()));
    std::reverse(vec.begin(),vec.end());
    EXPECT_TRUE(std::equal((this->aDequeLHS).begin(),(this->aDequeLHS).end(),vec.begin()));
}

TYPED_TEST(DequeTest, copy_constructor){
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);
    this->aDequeLHS.push_back(1);
    this->aDequeLHS.push_back(2);
    this->aDequeLHS.push_back(3);

    ASSERT_EQ(this->aDequeLHS.size(), this->blah().size());
    ASSERT_EQ(this->aDequeLHS.front(), this->blah().front());
    ASSERT_EQ(this->aDequeLHS.back(), this->blah().back());
    // EXPECT_TRUE(this->aDequeLHS[1] == copy[1]);
}