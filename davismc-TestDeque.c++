// ----------------------------
// projects/deque/TestDeque.c++
// Copyright (C) 2013
// Glenn P. Downing
// ----------------------------


// --------
// includes
// --------

#include <algorithm> // equal
#include <cstring>   // strcmp
#include <deque>     // deque
#include <sstream>   // ostringstream
#include <stdexcept> // invalid_argument
#include <string>    // ==
#include <memory>	 // allocator
#include <cstdlib>	 // rand

#include "gtest/gtest.h"

#include "Deque.h"

/*
 * Implementation Notes
 *
 * Some tests use a type fixture to test both std::deque and MyDeque without
 * doubling code.  On other tests, this was problematic because I needed more
 * control over the constructor or needed to differentiate between interator and
 * const_iterator, so I simply doubled the code and used standard tests.
 *
 * Tests were ran with both <int> and <string>.  This gave coverage for both
 * base types as well as class objects.
*/

// Type fixture to support testing both deque and MyDeque without doubling code
// Type <int>
template <typename C>
class IntTest : public testing::Test {
	protected:
		C x;
		C y;

		IntTest(): x(0), y(0){}
};
typedef testing::Types<std::deque<int>, MyDeque<int> > IntTypes;
TYPED_TEST_CASE(IntTest, IntTypes);


// Type fixture to support testing both deque and MyDeque without doubling code
// Type <string>
template <typename C>
class StringTest : public testing::Test {
	protected:
		C x;
		C y;

		StringTest(): x(0), y(0){}
};
typedef testing::Types<std::deque<std::string>, MyDeque<std::string> > StringTypes;
TYPED_TEST_CASE(StringTest, StringTypes);

// ---------
// TestDeque
// ---------

// ----
// size
// ----

TEST(SizeTests, size_1) {
	const MyDeque<int> x;
	ASSERT_TRUE(x.size() == 0);}

TYPED_TEST(IntTest, size_2) {
	ASSERT_TRUE(this->x.size() == 0);
}

TYPED_TEST(IntTest, size_3) {
	this->x.push_back(5);
	ASSERT_TRUE(this->x.size() == 1);
}

TEST(SizeTests, size_5) {
	MyDeque<std::string> x;
	ASSERT_TRUE(x.size() == 0);
	x.push_back("abc");
	ASSERT_TRUE(x.size() == 1);}

TYPED_TEST(StringTest, size_4) {
	this->x.push_back("abc");
	this->x.push_front("tuv");
	ASSERT_TRUE(this->x.size() == 2);
}


// ---------
// operator ==
// ---------
TYPED_TEST(IntTest, operator_equals_1) {
	this->x.push_back(15);
	this->x.push_back(25);

	this->y.push_back(15);
	this->y.push_back(25);
	ASSERT_TRUE(this->x == this->y);

	this->y.push_back(30);
	ASSERT_FALSE(this->x == this->y);
}

// Two empty containers should be equal
TYPED_TEST(IntTest, operator_equals_2) {
	ASSERT_TRUE(this->x == this->y);

	this->y.push_back(30);
	ASSERT_FALSE(this->x == this->y);
}

// Same values, different order
TYPED_TEST(IntTest, operator_equals_3) {
	this->x.push_back(12);
	this->x.push_back(24);

	this->y.push_back(24);
	this->y.push_back(12);
	ASSERT_FALSE(this->x == this->y);
}

// Strings
TYPED_TEST(StringTest, operator_equals_4) {
	ASSERT_TRUE(this->x == this->y);
	this->x.push_back("abc");
	this->x.push_back("def");

	this->y.push_back("abc");
	this->y.push_back("def");
	ASSERT_TRUE(this->x == this->y);
} 

// ---------
// operator <
// ---------
TYPED_TEST(IntTest, operator_lessthan_1) {
	this->x.push_back(15);
	this->x.push_back(23);

	this->y.push_back(15);
	this->y.push_back(25);
	ASSERT_TRUE(this->x < this->y);
	ASSERT_TRUE(this->y > this->x);

	this->x.push_front(30);
	ASSERT_TRUE(this->y < this->x);
}

// Two empty containers should not be less than
TYPED_TEST(IntTest, operator_lessthan_2) {
	ASSERT_TRUE(this->x == this->y);

	ASSERT_FALSE(this->x < this->y);
	ASSERT_FALSE(this->y < this->x);
}

// One empty, other has a value
TYPED_TEST(IntTest, operator_lessthan_3) {
	this->y.push_back(30);
	ASSERT_TRUE(this->x < this->y);
}

// Compare strings
TYPED_TEST(StringTest, operator_lessthan_4) {
	this->x.push_back("CCC");
	this->y.push_back("AAA");
	ASSERT_TRUE(this->y < this->x);
}

// ---------
// iterator ==
// ---------
TYPED_TEST(IntTest, iterator_operator_equals_1) {
	auto first = this->x.begin();
	auto second = this->x.begin();
	ASSERT_TRUE(first == second);
}

TYPED_TEST(IntTest, iterator_operator_equals_2) {
	this->x.push_back(50);
	auto first = this->x.begin();
	auto second = this->x.end();
	ASSERT_FALSE(first == second);
}

// const_iterator
TEST(ConstIterator, iterator_operator_equals3) {
	{
	std::deque<int> x(2);
	std::deque<int>::const_iterator first = x.begin();
	std::deque<int>::const_iterator second = x.begin();
	ASSERT_TRUE(first == second);
	second = x.end();
	ASSERT_FALSE(first == second);
	}

	{
	MyDeque<int> x(2);
	MyDeque<int>::const_iterator first = x.begin();
	MyDeque<int>::const_iterator second = x.begin();
	ASSERT_TRUE(first == second);
	second = x.end();
	ASSERT_FALSE(first == second);
	}
}

// ---------
// iterator !=
// ---------
// Tested by == tests, trivial implementation

// ---------
// iterator +, -
// ---------
// Tested by +=, -= tests, trivial implementation

// ---------
// iterator operator *
// ---------
TYPED_TEST(IntTest, iterator_operator_star_1) {
	this->x.push_back(30);
	auto b = this->x.begin();
	ASSERT_TRUE(*b == 30);
}

TYPED_TEST(IntTest, iterator_operator_star_2) {
	this->x.push_back(9);
	this->x.push_back(5);
	this->x.push_back(12);
	auto b = this->x.begin();
	ASSERT_TRUE(*b == 9);
	++b;
	++*b;
	ASSERT_TRUE(*b == 6);
	++b;
	ASSERT_TRUE(*b == 12);
}

TYPED_TEST(StringTest, iterator_operator_star_3) {
	this->x.push_back("AAA");
	this->x.push_back("BBB");
	this->x.push_back("CCC");
	auto b = this->x.begin();
	ASSERT_TRUE(*b == "AAA");
	++b;
	ASSERT_TRUE(*b == "BBB");
	++b;
	ASSERT_TRUE(*b == "CCC");
}

TEST(ConstIterator, const_iterator_operator_star_1) {
	MyDeque<int> x(0);
	x.push_back(30);
	MyDeque<int>::const_iterator b = x.begin();
	ASSERT_TRUE(*b == 30);
}

TEST(ConstIterator, const_iterator_operator_star_2) {
	MyDeque<int> x(0);
	x.push_back(9);
	x.push_back(5);
	x.push_back(12);
	MyDeque<int>::const_iterator b = x.begin();
	ASSERT_TRUE(*b == 9);
	++b;
	ASSERT_TRUE(*b == 5);
	++b;
	ASSERT_TRUE(*b == 12);
}
TEST(ConstIterator, const_iterator_operator_star_3) {
	MyDeque<std::string> x(0);
	x.push_back("AAA");
	x.push_back("BBB");
	x.push_back("CCC");
	MyDeque<std::string>::const_iterator b = x.begin();
	ASSERT_TRUE(*b == "AAA");
	++b;
	ASSERT_TRUE(*b == "BBB");
	++b;
	ASSERT_TRUE(*b == "CCC");
}

// -----------
// operator ->
// -----------
// Implemented by Professor
TEST(Iterator, iterator_operator_arrow_1) {
	std::deque<std::string> x(2);
	std::string s("AAA");
	x.push_back(s);
	std::deque<std::string>::iterator b = x.begin();
	ASSERT_TRUE(strcmp(b->c_str(), s.c_str()));
}

TEST(Iterator, iterator_operator_arrow_2) {
	MyDeque<std::string> x(2);
	std::string s("AAA");
	x.push_back(s);
	MyDeque<std::string>::iterator b = x.begin();
	ASSERT_TRUE(strcmp(b->c_str(), s.c_str()));
}


TEST(ConstIterator, const_iterator_operator_arrow_1) {
	std::deque<std::string> x(2);
	std::string s("AAA");
	x.push_back(s);
	std::deque<std::string>::const_iterator b = x.begin();
	ASSERT_TRUE(strcmp(b->c_str(), s.c_str()));
}

TEST(ConstIterator, const_iterator_operator_arrow_2) {
	MyDeque<std::string> x(2);
	std::string s("AAA");
	x.push_back(s);
	MyDeque<std::string>::const_iterator b = x.begin();
	ASSERT_TRUE(strcmp(b->c_str(), s.c_str()));
}

// ---------
// iterator operator ++
// ---------
TEST(Iterator, iterator_operator_plus_plus_1) {
	std::deque<int> x;
	x.push_back(1);
	x.push_back(2);
	x.push_back(3);
	auto b = x.begin();
	ASSERT_TRUE(*b == 1);
	++b;
	ASSERT_TRUE(*b == 2);
	++b;
	ASSERT_TRUE(*b == 3);
}

TEST(Iterator, iterator_operator_plus_plus_2) {
	MyDeque<int> x;
	x.push_back(1);
	x.push_back(2);
	x.push_back(3);
	auto b = x.begin();
	ASSERT_TRUE(*b == 1);
	++b;
	ASSERT_TRUE(*b == 2);
	++b;
	ASSERT_TRUE(*b == 3);
}

TEST(ConstIterator, iterator_operator_plus_plus_3) {
	std::deque<std::string> x;
	x.push_back("first");
	x.push_back("second");
	x.push_back("third");
	std::deque<std::string>::const_iterator b = x.begin();
	ASSERT_TRUE(*b == "first");
	++b;
	ASSERT_TRUE(*b == "second");
	++b;
	ASSERT_TRUE(*b == "third");
}

TEST(ConstIterator, iterator_operator_plus_plus_4) {
	MyDeque<std::string> x;
	x.push_back("first");
	x.push_back("second");
	x.push_back("third");
	MyDeque<std::string>::const_iterator b = x.begin();
	ASSERT_TRUE(*b == "first");
	++b;
	ASSERT_TRUE(*b == "second");
	++b;
	ASSERT_TRUE(*b == "third");
}


// ---------
// iterator operator -- 
// ---------
TEST(Iterator, iterator_operator_minus_minus_1) {
	std::deque<int> x;
	x.push_back(1);
	x.push_back(2);
	x.push_back(3);
	auto e = x.end();
	--e;
	ASSERT_TRUE(*e == 3);
	--e;
	ASSERT_TRUE(*e == 2);
	--e;
	ASSERT_TRUE(*e == 1);
}

TEST(Iterator, iterator_operator_minus_minus_2) {
	MyDeque<int> x;
	x.push_back(1);
	x.push_back(2);
	x.push_back(3);
	auto e = x.end();
	--e;
	ASSERT_TRUE(*e == 3);
	--e;
	ASSERT_TRUE(*e == 2);
	--e;
	ASSERT_TRUE(*e == 1);
}

TEST(ConstIterator, iterator_operator_minus_minus_3) {
	std::deque<std::string> x;
	x.push_back("first");
	x.push_back("second");
	x.push_back("third");
	std::deque<std::string>::const_iterator e = x.end();
	--e;
	ASSERT_TRUE(*e == "third");
	--e;
	ASSERT_TRUE(*e == "second");
	--e;
	ASSERT_TRUE(*e == "first");
}

TEST(ConstIterator, iterator_operator_minus_minus_4) {
	MyDeque<std::string> x;
	x.push_back("first");
	x.push_back("second");
	x.push_back("third");
	MyDeque<std::string>::const_iterator e = x.end();
	--e;
	ASSERT_TRUE(*e == "third");
	--e;
	ASSERT_TRUE(*e == "second");
	--e;
	ASSERT_TRUE(*e == "first");
}

// ---------
// iterator operator -- (int)
// ---------
// postfix operator, implemented by professor, calls operator --

// ---------
// iterator operator +=
// ---------
TEST(Iterator, iterator_operator_plus_equal_1) {
	std::deque<int> x;
	x.push_back(1);
	x.push_back(2);
	x.push_back(3);
	auto b = x.begin();
	ASSERT_TRUE(*b == 1);
	b += 2;
	ASSERT_TRUE(*b == 3);
}

TEST(Iterator, iterator_operator_plus_equal_2) {
	MyDeque<int> x;
	x.push_back(1);
	x.push_back(2);
	x.push_back(3);
	auto b = x.begin();
	ASSERT_TRUE(*b == 1);
	b += 2;
	ASSERT_TRUE(*b == 3);
}

TEST(ConstIterator, iterator_operator_plus_equal_3) {
	std::deque<std::string> x;
	x.push_back("first");
	x.push_back("second");
	x.push_back("third");
	std::deque<std::string>::const_iterator b = x.begin();
	ASSERT_TRUE(*b == "first");
	b += 2;
	ASSERT_TRUE(*b == "third");
}

TEST(ConstIterator, iterator_operator_plus_equal_4) {
	MyDeque<std::string> x;
	x.push_back("first");
	x.push_back("second");
	x.push_back("third");
	MyDeque<std::string>::const_iterator b = x.begin();
	ASSERT_TRUE(*b == "first");
	b += 2;
	ASSERT_TRUE(*b == "third");
}

// -----------
// operator -=
// -----------
TEST(Iterator, iterator_operator_minus_equal_1) {
	std::deque<int> x;
	x.push_back(1);
	x.push_back(2);
	x.push_back(3);
	auto e = x.end();
	--e;
	ASSERT_TRUE(*e == 3);
	e -= 2;
	ASSERT_TRUE(*e == 1);
}

TEST(Iterator, iterator_operator_minus_equal_2) {
	MyDeque<int> x;
	x.push_back(1);
	x.push_back(2);
	x.push_back(3);
	auto e = x.end();
	--e;
	ASSERT_TRUE(*e == 3);
	e -= 2;
	ASSERT_TRUE(*e == 1);
}

TEST(ConstIterator, iterator_operator_minus_equal_3) {
	std::deque<std::string> x;
	x.push_back("first");
	x.push_back("second");
	x.push_back("third");
	std::deque<std::string>::const_iterator e = x.end();
	--e;
	ASSERT_TRUE(*e == "third");
	e -= 2;
	ASSERT_TRUE(*e == "first");
}

TEST(ConstIterator, iterator_operator_minus_equal_4) {
	MyDeque<std::string> x;
	x.push_back("first");
	x.push_back("second");
	x.push_back("third");
	MyDeque<std::string>::const_iterator e = x.end();
	--e;
	ASSERT_TRUE(*e == "third");
	e -= 2;
	ASSERT_TRUE(*e == "first");
}


// -----------
// MyDeque(const allocator_type& a = allocator_type()) 
// -----------
TEST(Constructors, def_constr_1) {
	MyDeque<int> x;
	ASSERT_TRUE(x.size() == 0);
}

TEST(Constructors, def_constr_2) {
	MyDeque<int> x;
	ASSERT_TRUE(x.size() == 0);
	x.push_back(2);
	ASSERT_TRUE(x.size() == 1);
}

TEST(Constructors, def_constr_3) {
	MyDeque<std::string> x;
	ASSERT_TRUE(x.size() == 0);
	x.push_back("BB");
	ASSERT_TRUE(x.size() == 1);
}

TEST(Constructors, def_constr_4) {
	MyDeque<int> x = MyDeque<int>(std::allocator<int>());
	ASSERT_TRUE(x.size() == 0);
}

// -----------
// MyDeque(size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type())
// -----------
TEST(Constructors, constr_size_val_1) {
	MyDeque<int> x(5);
	ASSERT_TRUE(x.size() == 5);}

TEST(Constructors, constr_size_val_2) {
	MyDeque<std::string> x(0);
	ASSERT_TRUE(x.size() == 0);}

TEST(Constructors, constr_size_val_3) {
	MyDeque<std::string> x(1000);
	ASSERT_TRUE(x.size() == 1000);}

TEST(Constructors, constr_size_val_4) {
	MyDeque<int> x = MyDeque<int>(6, 0, std::allocator<int>());
	ASSERT_TRUE(x.size() == 6);}

TEST(Constructors, constr_size_val_5) {
	MyDeque<int> x(3, 25);
	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(x[0] == 25);
	ASSERT_TRUE(x[2] == 25);}

TEST(Constructors, constr_size_val_6) {
	MyDeque<std::string> x(4, "abc");
	ASSERT_TRUE(x.size() == 4);
	ASSERT_TRUE(x[0] == "abc");
	ASSERT_TRUE(x[3] == "abc");}

TEST(Constructors, constr_size_val_7) {
	MyDeque<int> x = MyDeque<int>(6, 10, std::allocator<int>());
	ASSERT_TRUE(x.size() == 6);}

// -----------
// MyDeque(const MyDeque& that)
// -----------
TEST(Constructors, copy_constr_1) {
	MyDeque<int> x(0);
	ASSERT_TRUE(x.size() == 0);

	MyDeque<int> y(x);
	ASSERT_TRUE(y.size() == 0);}

TEST(Constructors, copy_constr_2) {
	MyDeque<int> x(3);
	ASSERT_TRUE(x.size() == 3);
	x.at(0) = 5;
	x.at(1) = 12;

	MyDeque<int> y(x);
	ASSERT_TRUE(y.size() == 3);
	ASSERT_TRUE(y[0] == 5);
	ASSERT_TRUE(y[1] == 12);
	ASSERT_TRUE(y[2] == 0);}

TEST(Constructors, copy_constr_3) {
	MyDeque<std::string> x(0);
	x.push_back("abc");
	x.push_back("xyz");
	ASSERT_TRUE(x.size() == 2);

	MyDeque<std::string> y(x);
	ASSERT_TRUE(y.size() == 2);
	ASSERT_TRUE(y[0] == "abc");
	ASSERT_TRUE(y[1] == "xyz");}

// Prove it's a deep copy - pointers should be different
TEST(Constructors, copy_constr_4) {
	MyDeque<int> x(3);
	ASSERT_TRUE(x.size() == 3);
	x.at(0) = 5;
	x.at(1) = 12;

	MyDeque<int> y(x);
	ASSERT_TRUE(y.size() == 3);
	ASSERT_TRUE(y[0] == 5);

	ASSERT_TRUE(&x[0] != &y[0]);	
	ASSERT_TRUE(&x[1] != &y[1]);
	ASSERT_TRUE(&x[2] != &y[2]);}

// ----------
// destructor
// ----------
// Unclear how to unit test this

// ----------
// operator =   copy assignment 
// ----------
TEST(Operators, copy_assgn_1) {
	MyDeque<int> x(0);
	ASSERT_TRUE(x.size() == 0);

	MyDeque<int> y;
	y = x;
	ASSERT_TRUE(y.size() == 0);}

// rhs is larger than lhs
TEST(Constructors, copy_assgn_2) {
	MyDeque<int> x(0);
	x.push_back(25);
	x.push_back(13);

	MyDeque<int> y;
	y = x;
	ASSERT_TRUE(y.size() == 2);
	ASSERT_TRUE(y[0] == 25);
	ASSERT_TRUE(y[1] == 13);}

// same size
TEST(Constructors, copy_assgn_3) {
	MyDeque<std::string> x(0);
	x.push_back("abc");
	x.push_back("xyz");

	MyDeque<std::string> y;
	y.push_back("uwl");
	y.push_back("trs");
	y = x;
	ASSERT_TRUE(y.size() == 2);
	ASSERT_TRUE(y[0] == "abc");
	ASSERT_TRUE(y[1] == "xyz");}

// Prove it's a deep copy - pointers should be different
TEST(Constructors, copy_assgn_4) {
	MyDeque<int> x(0);
	x.push_back(-5);
	x.push_back(-30);

	MyDeque<int> y;
	y = x;
	ASSERT_TRUE(y.size() == 2);
	ASSERT_TRUE(y[0] == -5);

	ASSERT_TRUE(&x[0] != &y[0]);	
	ASSERT_TRUE(&x[1] != &y[1]);
	ASSERT_TRUE(&x[2] != &y[2]);}

// rhs is smaller than lhs
TEST(Constructors, copy_assgn_5) {
	MyDeque<int> x(0);
	x.push_back(25);
	x.push_back(13);

	MyDeque<int> y;
	y.push_back(15);
	y.push_back(25);
	y.push_back(30);
	y = x;
	ASSERT_TRUE(y.size() == 2);
	ASSERT_TRUE(y[0] == 25);
	ASSERT_TRUE(y[1] == 13);}

// Pass it itself
TEST(Constructors, copy_assgn_6) {
	MyDeque<std::string> x(0);
	x.push_back("abc");
	x.push_back("xyz");

	x = x;
	ASSERT_TRUE(x.size() == 2);
	ASSERT_TRUE(x[0] == "abc");
	ASSERT_TRUE(x[1] == "xyz");}

// ----------
// operator []   index access 
// ----------
TEST(Operators, index_1) {
	MyDeque<int> x(1);
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(x[0] == 0);
}

TEST(Operators, index_2) {
	MyDeque<int> x(2);
	ASSERT_TRUE(x.size() == 2);
	ASSERT_TRUE(x[0] == 0);

	x[0] = 12;
	ASSERT_TRUE(x[0] == 12);
}

TEST(Operators, index_3) {
	MyDeque<int> x(2);
	ASSERT_TRUE(x.size() == 2);
	ASSERT_TRUE(x[0] == 0);

	x[0] = 20;
	ASSERT_TRUE(x[0] == 20);
	x.push_front(10);
	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(x[0] == 10);
	ASSERT_TRUE(x[1] == 20);
	ASSERT_TRUE(x[2] == 0);
}

// ----------
// operator [] const   index access 
// ----------
TEST(Operators, index_const_1) {
	const MyDeque<int> x(1);
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(x[0] == 0);
}

TEST(Operators, index_const_2) {
	MyDeque<int> x(2);
	ASSERT_TRUE(x.size() == 2);
	ASSERT_TRUE(x[0] == 0);
	x[0] = 12;

	const MyDeque<int> y = x;
	ASSERT_TRUE(y[0] == 12);
}

TEST(Operators, index_const_3) {
	MyDeque<int> x(2);
	ASSERT_TRUE(x.size() == 2);
	ASSERT_TRUE(x[0] == 0);
	x[0] = 20;
	ASSERT_TRUE(x[0] == 20);
	x.push_front(10);

	const MyDeque<int> y = x;
	ASSERT_TRUE(y.size() == 3);
	ASSERT_TRUE(y[0] == 10);
	ASSERT_TRUE(y[1] == 20);
	ASSERT_TRUE(y[2] == 0);
}

// ----------
// at()   index access 
// ----------
TEST(Operations, at_1) {
	try{
		MyDeque<int> x(1);
		ASSERT_TRUE(x.size() == 1);
		ASSERT_TRUE(x.at(0) == 0);	}
	catch (...) {
		ASSERT_TRUE(false);}}

TEST(Operations, at_2) {
	try{
		MyDeque<int> x(2);
		ASSERT_TRUE(x.size() == 2);
		ASSERT_TRUE(x.at(0) == 0);

		x[0] = 12;
		ASSERT_TRUE(x.at(0) == 12);	}
	catch (...) {
		ASSERT_TRUE(false);}}

TEST(Operations, at_3) {
	try{
		MyDeque<int> x(2);
		ASSERT_TRUE(x.size() == 2);
		ASSERT_TRUE(x.at(0) == 0);

		x[0] = 20;
		ASSERT_TRUE(x.at(0) == 20);
		x.push_front(10);
		ASSERT_TRUE(x.size() == 3);
		ASSERT_TRUE(x.at(0) == 10);
		ASSERT_TRUE(x.at(1) == 20);
		ASSERT_TRUE(x.at(2) == 0);	}
	catch (...){
		ASSERT_TRUE(false);}}

// Try an illegal access, catch the throw
TEST(Operations, at_4) {
	try{
		MyDeque<int> x(0);
		ASSERT_TRUE(x.size() == 0);
		std::cout << x.at(1);
	}
	catch (...){
		ASSERT_TRUE(true);}}
		//catch (std::invalid_argument& e) {
          //  CPPUNIT_ASSERT(strcmp(e.what(), "Integer()"));}}


TEST(Operations, at_5) {
	try{
		MyDeque<int> x(0);
		ASSERT_TRUE(x.size() == 0);
		x.at(1) = 25;}
	catch (...){
		ASSERT_TRUE(true);}}

// ----------
// at() const  index access 
// ----------
TEST(Operations, at_const_1) {
	try{
		const MyDeque<int> x(1);
		ASSERT_TRUE(x.size() == 1);
		ASSERT_TRUE(x.at(0) == 0);	}
	catch (...) {
		ASSERT_TRUE(false);}}

TEST(Operations, at_const_2) {
	try{
		MyDeque<int> x(2);
		ASSERT_TRUE(x.size() == 2);
		ASSERT_TRUE(x.at(0) == 0);

		x[0] = 12;
		const MyDeque<int> y = x;
		ASSERT_TRUE(y.at(0) == 12);	}
	catch (std::out_of_range& e) {
		ASSERT_TRUE(false);}}

TEST(Operations, at_const_3) {
	try{
		MyDeque<int> f(2);
		ASSERT_TRUE(f.size() == 2);
		ASSERT_TRUE(f.at(0) == 0);

		f.at(0) = 20;
		ASSERT_TRUE(f.at(0) == 20);
		f.push_front(10);
		const MyDeque<int> x = f;
		ASSERT_TRUE(x.size() == 3);
		ASSERT_TRUE(x.at(0) == 10);
		ASSERT_TRUE(x.at(1) == 20);
		ASSERT_TRUE(x.at(2) == 0);	}
	catch (std::out_of_range& e){
		ASSERT_TRUE(false);}}

// Try an illegal access, catch the throw
TEST(Operations, at_const_4) {
	try{
		const MyDeque<int> x(0);
		ASSERT_TRUE(x.size() == 0);
		std::cout << x.at(1);
	}
	catch (std::out_of_range& e){
		ASSERT_TRUE(true);
		ASSERT_TRUE(strcmp(e.what(), "_M_range_check"));}}
		//catch (std::invalid_argument& e) {
        //  CPPUNIT_ASSERT(strcmp(e.what(), "MyDeque::_M_range_check"));}}


// ----------
// back 
// ----------
TEST(Operations, back_1) {
	MyDeque<int> x(1);
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(x.back() == 0);
	ASSERT_TRUE(x.front() == 0);
}

TEST(Operations, back_2) {
	MyDeque<int> x;
	x.push_back(5);
	x.push_back(10);
	x.push_back(15);
	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(x.back() == 15);
}

TEST(Operations, back_3) {
	MyDeque<std::string> x;
	x.push_back("abc");
	x.push_back("jkl");
	x.push_back("xyz");
	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(x.back() == "xyz");
}

// ----------
// back const
// ----------
TEST(Operations, back_const_1) {
	MyDeque<int> f(1);
	const MyDeque<int> x(f);
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(x.back() == 0);
	ASSERT_TRUE(x.front() == 0);
}

TEST(Operations, back_const_2) {
	MyDeque<int> f;
	f.push_back(5);
	f.push_back(10);
	f.push_back(15);
	const MyDeque<int> x(f);
	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(x.back() == 15);
}

TEST(Operations, back_const_3) {
	MyDeque<std::string> f;
	f.push_back("abc");
	f.push_back("jkl");
	f.push_back("xyz");
	const MyDeque<std::string> x(f);
	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(x.back() == "xyz");
}

// ----------
// front 
// ----------
TEST(Operations, front_1) {
	MyDeque<int> x(1);
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(x.back() == 0);
	ASSERT_TRUE(x.front() == 0);
}

TEST(Operations, front_2) {
	MyDeque<int> x;
	x.push_back(5);
	x.push_back(10);
	x.push_back(15);
	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(x.front() == 5);
}

TEST(Operations, front_3) {
	MyDeque<std::string> x;
	x.push_back("abc");
	x.push_back("jkl");
	x.push_back("xyz");
	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(x.front() == "abc");
}

// ----------
// front const
// ----------
TEST(Operations, front_const_1) {
	MyDeque<int> f(1);
	const MyDeque<int> x(f);
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(x.back() == 0);
	ASSERT_TRUE(x.front() == 0);
}

TEST(Operations, front_const_2) {
	MyDeque<int> f;
	f.push_back(5);
	f.push_back(10);
	f.push_back(15);
	const MyDeque<int> x(f);
	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(x.front() == 5);
}

TEST(Operations, front_const_3) {
	MyDeque<std::string> f;
	f.push_back("abc");
	f.push_back("jkl");
	f.push_back("xyz");
	const MyDeque<std::string> x(f);
	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(x.front() == "abc");
}

// ----------
// begin
// ----------
TEST(Operations, begin_1) {
	MyDeque<int> x(1);
	MyDeque<int>::iterator b = x.begin();
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(*b == 0);
}

TEST(Operations, begin_2) {
	MyDeque<int> x(0);
	x.push_back(25);
	MyDeque<int>::iterator b = x.begin();
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(*b == 25);
	ASSERT_TRUE(*&*b == 25);
}

TEST(Operations, begin_3) {
	MyDeque<std::string> x(0);
	x.push_back("abc");
	x.push_back("efg");
	MyDeque<std::string>::iterator b = x.begin();
	ASSERT_TRUE(x.size() == 2);
	ASSERT_TRUE(*b == "abc");
	++b;
	ASSERT_TRUE(*b == "efg");
}

// ----------
// begin const
// ----------
TEST(Operations, begin_const_1) {
	MyDeque<int> x(1);
	MyDeque<int>::const_iterator b = x.begin();
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(*b == 0);
}

TEST(Operations, begin_const_2) {
	MyDeque<int> x(0);
	x.push_back(25);
	MyDeque<int>::const_iterator b = x.begin();
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(*b == 25);
	ASSERT_TRUE(*&*b == 25);
}

TEST(Operations, begin_const_3) {
	MyDeque<std::string> x(0);
	x.push_back("abc");
	x.push_back("efg");
	MyDeque<std::string>::const_iterator b = x.begin();
	ASSERT_TRUE(x.size() == 2);
	ASSERT_TRUE(*b == "abc");
	++b;
	ASSERT_TRUE(*b == "efg");
}

// ----------
// clear
// ----------
TEST(Operations, clear_1) {
	MyDeque<int> x(1);
	ASSERT_TRUE(x.size() == 1);
	x.clear();
	ASSERT_TRUE(x.size() == 0);}

TEST(Operations, clear_2) {
	MyDeque<int> x(0);
	x.push_back(5);
	x.push_back(10);
	ASSERT_TRUE(x.size() == 2);
	x.clear();
	ASSERT_TRUE(x.size() == 0);}

TEST(Operations, clear_3) {
	MyDeque<std::string> x(0);
	x.push_back("abc");
	x.push_back("efg");
	x.push_back("xyz");
	ASSERT_TRUE(x.size() == 3);
	x.clear();
	ASSERT_TRUE(x.size() == 0);}

// ----------
// empty
// ----------
TEST(Operations, empty_1) {
	MyDeque<int> x(0);
	ASSERT_TRUE(x.size() == 0);
	ASSERT_TRUE(x.empty());}

TEST(Operations, empty_2) {
	MyDeque<int> x(1);
	ASSERT_TRUE(x.size() == 1);
	ASSERT_FALSE(x.empty());
	x.clear();
	ASSERT_TRUE(x.size() == 0);
	ASSERT_TRUE(x.empty());}


TEST(Operations, empty_3) {
	MyDeque<std::string> x(0);
	x.push_back("abc");
	x.push_back("efg");
	x.push_back("xyz");
	ASSERT_TRUE(x.size() == 3);
	x.clear();
	ASSERT_TRUE(x.size() == 0);}

// ----------
// end
// ----------
TEST(Operations, end_1) {
	MyDeque<int> x(1);
	MyDeque<int>::iterator e = x.end();
	--e;
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(*e == 0);}

TEST(Operations, end_2) {
	MyDeque<int> x(0);
	x.push_back(25);
	MyDeque<int>::iterator e = x.end();
	--e;
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(*e == 25);
	ASSERT_TRUE(*&*e == 25);}

TEST(Operations, end_3) {
	MyDeque<std::string> x(0);
	x.push_back("abc");
	x.push_back("efg");
	MyDeque<std::string>::iterator e = x.end();
	ASSERT_TRUE(x.size() == 2);
	--e;
	ASSERT_TRUE(*e == "efg");
	--e;
	ASSERT_TRUE(*e == "abc");}

// ----------
// end const
// ----------
TEST(Operations, end_const_1) {
	MyDeque<int> x(1);
	MyDeque<int>::const_iterator e = x.end();
	--e;
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(*e == 0);}

TEST(Operations, end_const_2) {
	MyDeque<int> x(0);
	x.push_back(25);
	MyDeque<int>::const_iterator e = x.end();
	--e;
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(*e == 25);
	ASSERT_TRUE(*&*e == 25);}

TEST(Operations, end_const_3) {
	MyDeque<std::string> x(0);
	x.push_back("abc");
	x.push_back("efg");
	MyDeque<std::string>::const_iterator e = x.end();
	ASSERT_TRUE(x.size() == 2);
	--e;
	ASSERT_TRUE(*e == "efg");
	--e;
	ASSERT_TRUE(*e == "abc");}

// ----------
// erase
// ----------
TEST(Operations, erase_1) {
	MyDeque<int> x;
	for (int i = 1; i < 8; ++i)
		x.push_back(i * 10);
	auto b = x.begin();
	b = x.erase(b); // returns an iterator to the next position
	ASSERT_TRUE(x.size() == 6);
	ASSERT_TRUE(x[0] == 20);
	ASSERT_TRUE(*b == 20);}

TEST(Operations, erase_2) {
	MyDeque<int> x;
	for (int i = 1; i < 8; ++i)
		x.push_back(i * 10);
	auto b = x.begin();
	b += 3;
	b = x.erase(b); // returns an iterator to the next position
	ASSERT_TRUE(x.size() == 6);
	ASSERT_TRUE(x[3] == 50);
	ASSERT_TRUE(*b == 50);
	b = x.erase(b);
	ASSERT_TRUE(x.size() == 5);
	ASSERT_TRUE(x[3] == 60);
	ASSERT_TRUE(*b == 60);}

TEST(Operations, erase_3) {
	MyDeque<int> x;
	for (int i = 1; i < 8; ++i)
		x.push_back(i * 10);
	auto e = x.end();
	--e;
	e = x.erase(e); // returns an iterator to the next position
	ASSERT_TRUE(x.size() == 6);
	ASSERT_TRUE(x[5] == 60);
	ASSERT_TRUE(e == x.end());}

TEST(Operations, erase_4) {
	MyDeque<std::string> x;
	x.push_back("abc");
	x.push_back("def");
	x.push_back("hij");
	auto b = x.begin() + 1;
	b = x.erase(b); // returns an iterator to the next position
	ASSERT_TRUE(x.size() == 2);
	ASSERT_TRUE(x[1] == "hij");
	ASSERT_TRUE(*b == "hij");}

// ----------
// insert
// ----------
// at front
TEST(Operations, insert_1) {
	MyDeque<int> x(0);
	auto b = x.begin();
	b = x.insert(b, 35);
	ASSERT_TRUE(x[0] == 35);
	ASSERT_TRUE(*b == 35);}

// at middle
TEST(Operations, insert_2) {
	MyDeque<int> x(1);
	auto b = x.begin();
	for (int i = 1; i < 8; ++i)
		b = x.insert(b, i * 10);
	ASSERT_TRUE(x.size() == 8);
	ASSERT_TRUE(x[6] == 10);
	ASSERT_TRUE(x[5] == 20);
	ASSERT_TRUE(x.at(0) == 70);
	ASSERT_TRUE(*b == 70);}

// at end
TEST(Operations, insert_3) {
	MyDeque<int> x(2);
	auto e = x.end();
	e = x.insert(e, 12);
	ASSERT_TRUE(x[2] == 12);
	ASSERT_TRUE(*e == 12);
	ASSERT_TRUE(x.size() == 3);
	e = x.insert(e, 24);
	ASSERT_TRUE(x[2] == 24);
	ASSERT_TRUE(x[3] == 12);
	ASSERT_TRUE(*e == 24);}

TEST(Operations, insert_8) {
	MyDeque<int> x(5, 15);
	auto b = x.begin() + 1;
	//x.print_deque();
	b = x.insert(b, 7);
	//x.print_deque();
	ASSERT_TRUE(x[1] == 7);
	ASSERT_TRUE(*b == 7);
	ASSERT_TRUE(x.size() == 6);}

TEST(Operations, insert_7) {
	MyDeque<std::string> x(5, "aaa");
	auto b = x.begin() + 1;
	b = x.insert(b, "qed");
	ASSERT_TRUE(x[1] == "qed");
	ASSERT_TRUE(*b == "qed");
	ASSERT_TRUE(x.size() == 6);}

// at end
TEST(Operations, insert_4) {
	MyDeque<std::string> x(2);
	auto e = x.end();
	e = x.insert(e, "qrs");
	ASSERT_TRUE(x[2] == "qrs");
	ASSERT_TRUE(*e == "qrs");
	ASSERT_TRUE(x.size() == 3);
	//e = x.insert(e, "tuv");
	//ASSERT_TRUE(x[2] == "tuv");
	//ASSERT_TRUE(x[3] == "qrs");
	//ASSERT_TRUE(*e == "tuv");}
}

TEST(Operations, insert_5) {
	std::deque<std::string> x(2);
	auto e = x.end();
	e = x.insert(e, "qrs");
	ASSERT_TRUE(x[2] == "qrs");
	ASSERT_TRUE(*e == "qrs");
	ASSERT_TRUE(x.size() == 3);
	e = x.insert(e, "tuv");
	ASSERT_TRUE(x[2] == "tuv");
	ASSERT_TRUE(x[3] == "qrs");
	ASSERT_TRUE(*e == "tuv");}

// cause a resize
TEST(Operations, insert_6) {
	MyDeque<int> x(1);
	auto b = x.end();
	for (int i = 1; i < 200; ++i){
		int num = i * 10;
		b = x.insert(x.end(), num);
		ASSERT_TRUE(*(x.end() - 1) == num);
		ASSERT_TRUE(*b == num);}}


// ----------
// pop_back
// ----------
TEST(Operations, pop_back_1) {
	MyDeque<int> x(1);
	ASSERT_TRUE(x.size() == 1);
	x.pop_back();
	ASSERT_TRUE(x.size() == 0);}

TEST(Operations, pop_back_2) {
	MyDeque<int> x(0);
	x.push_back(25);
	x.push_back(35);
	ASSERT_TRUE(x.size() == 2);
	x.pop_back();
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(x.back() == 25);}

TEST(Operations, pop_back_3) {
	MyDeque<std::string> x(0);
	x.push_back("abc");
	x.push_back("def");
	x.push_back("xyz");
	ASSERT_TRUE(x.size() == 3);
	x.pop_back();
	ASSERT_TRUE(x.size() == 2);
	ASSERT_TRUE(x.back() == "def");}

// ----------
// pop_front
// ----------
TEST(Operations, pop_front_1) {
	MyDeque<int> x(1);
	ASSERT_TRUE(x.size() == 1);
	x.pop_front();
	ASSERT_TRUE(x.size() == 0);}

TEST(Operations, pop_front_2) {
	MyDeque<int> x(0);
	x.push_back(25);
	x.push_back(35);
	ASSERT_TRUE(x.size() == 2);
	x.pop_front();
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(x.front() == 35);
	ASSERT_TRUE(x.back() == 35);}

TEST(Operations, pop_front_3) {
	MyDeque<std::string> x(0);
	x.push_back("abc");
	x.push_back("def");
	x.push_back("xyz");
	ASSERT_TRUE(x.size() == 3);
	x.pop_front();
	ASSERT_TRUE(x.size() == 2);
	ASSERT_TRUE(x.front() == "def");}

// ----------
// push_back
// ----------
TEST(Operations, push_back_1) {
	MyDeque<int> x(0);
	x.push_back(35);
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(x.back() == 35);}

TEST(Operations, push_back_2) {
	MyDeque<int> x(0);
	x.push_back(35);
	ASSERT_TRUE(x.size() == 1);
	ASSERT_TRUE(x.back() == 35);
	x.push_back(45);
	ASSERT_TRUE(x.size() == 2);
	ASSERT_TRUE(x.back() == 45);
	x.push_back(55);
	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(x.back() == 55);}

TEST(Operations, push_back_3) {
	MyDeque<std::string> x(0);
	x.push_back("abc");
	ASSERT_TRUE(x.size() == 1);
	x.push_back("def");
	ASSERT_TRUE(x.size() == 2);
	x.push_back("xyz");
	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(x.front() == "abc");
	ASSERT_TRUE(x.back() == "xyz");}

// ----------
// resize
// ----------
TEST(Operations, resize_1) {
	MyDeque<int> x(0);
	x.push_back(35);
	x.push_back(45);
	x.push_back(55);
	ASSERT_TRUE(x.size() == 3);
	x.resize(2);
	ASSERT_TRUE(x.size() == 2);
	ASSERT_TRUE(x.back() == 45);}

TEST(Operations, resize_2) {
	MyDeque<int> x(0);
	x.push_back(35);
	x.push_back(45);
	x.push_back(55);
	ASSERT_TRUE(x.size() == 3);
	x.resize(0);
	ASSERT_TRUE(x.size() == 0);}

TEST(Operations, resize_3) {
	MyDeque<int> x(0);
	x.push_back(35);
	x.push_back(45);
	x.push_back(55);
	ASSERT_TRUE(x.size() == 3);
	x.resize(5);
	ASSERT_TRUE(x.size() == 5);
	ASSERT_TRUE(x.at(4) == 0);
	ASSERT_TRUE(x.back() == 0);}

TEST(Operations, resize_4) {
	MyDeque<int> x(0);
	x.push_back(35);
	x.push_back(45);
	x.push_back(55);
	ASSERT_TRUE(x.size() == 3);
	x.resize(5, 65);
	ASSERT_TRUE(x.size() == 5);
	ASSERT_TRUE(x.at(4) == 65);
	ASSERT_TRUE(x.back() == 65);}

TEST(Operations, resize_5) {
	MyDeque<std::string> x(0);
	x.push_back("abc");
	x.push_back("def");
	x.push_back("xyz");
	ASSERT_TRUE(x.size() == 3);
	x.resize(2);

	ASSERT_TRUE(x.size() == 2);
	x.push_back("rst");
	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(x.back() == "rst");

	x.resize(6, "nop");
	ASSERT_TRUE(x.at(4) == "nop");
	ASSERT_TRUE(x.back() == "nop");
	ASSERT_TRUE(x.size() == 6);}

TEST(Operations, resize_6) {
	MyDeque<int> x(0);
	for (int i = 1; i < 200; ++i){
		int num = i * 10;
		x.push_back(num);}
	x.resize(10);}


// ----------
// swap
// ----------
TEST(Operations, swap_1) {
	MyDeque<int> x(1);
	MyDeque<int> y(1);
	int *x_ptr = &x[0];
	int *y_ptr = &y[0];
	x.swap(y);
	ASSERT_TRUE(x_ptr == &y[0]);
	ASSERT_TRUE(y_ptr == &x[0]);}

TEST(Operations, swap_2) {
	MyDeque<std::string> x(0);
	x.push_back("abc");
	x.push_back("def");
	x.push_back("ghi");

	MyDeque<std::string> y(0);
	y.push_back("lmn");
	y.push_back("opq");
	y.push_back("rst");

	ASSERT_TRUE(x.size() == 3);
	ASSERT_TRUE(y.size() == 3);

	x.swap(y);

	ASSERT_TRUE(x.front() == "lmn");
	ASSERT_TRUE(x.back() == "rst");
	ASSERT_TRUE(y.front() == "abc");
	ASSERT_TRUE(y.back() == "ghi");}

// Different lengths
TEST(Operations, swap_3) {
	MyDeque<int> x(0);
	x.push_back(5);
	x.push_back(10);

	MyDeque<int> y(0);
	y.push_back(8);

	int *x_ptr = &x[0];
	int *y_ptr = &y[0];

	x.swap(y);

	ASSERT_TRUE(x.front() == 8);
	ASSERT_TRUE(y.front() == 5);

	ASSERT_TRUE(x_ptr == &y[0]);
	ASSERT_TRUE(y_ptr == &x[0]);}

// ----------
// Acceptance Tests
// ----------
// Basic functionality - only uses constructors, push_back and at()

TEST(Acceptance, push_1){
	int const NUM_TESTS = 5000;

	std::deque<int> x;
	MyDeque<int> y;
	int rand_value;
	for (int i = 0; i < NUM_TESTS; ++i){
		rand_value = rand() % 99;
		x.push_back(rand_value);
		y.push_back(rand_value);}

	for (int i = 0; i < NUM_TESTS; ++i){
		ASSERT_TRUE(x.at(i) == y.at(i));}}

// push_front()
TEST(Acceptance, push_2){
	int const NUM_TESTS = 5000;

	std::deque<int> x;
	MyDeque<int> y;
	int rand_value;
	for (int i = 0; i < NUM_TESTS; ++i){
		rand_value = rand() % 99;
		x.push_front(rand_value);
		y.push_front(rand_value);}

	for (int i = 0; i < NUM_TESTS; ++i){
		ASSERT_TRUE(x.at(i) == y.at(i));}}

// mixed push_front() and push_back()
TEST(Acceptance, push_3){
	int const NUM_TESTS = 5000;

	std::deque<int> x;
	MyDeque<int> y;
	int rand_value;
	for (int i = 0; i < NUM_TESTS; ++i){
		rand_value = rand() % 99;
		x.push_front(rand_value);
		y.push_front(rand_value);
		rand_value = rand() % 99;
		x.push_back(rand_value);
		y.push_back(rand_value);}


	for (int i = 0; i < NUM_TESTS * 2; ++i){
		ASSERT_TRUE(x.at(i) == y.at(i));}}

// test pop_back
TEST(Acceptance, removals_1){
	int const NUM_TESTS = 2000;

	std::deque<int> x;
	MyDeque<int> y;
	int rand_value;
	for (int i = 0; i < NUM_TESTS; ++i){
		rand_value = rand() % 99;
		x.push_front(rand_value);
		y.push_front(rand_value);
		rand_value = rand() % 99;
		x.push_back(rand_value);
		y.push_back(rand_value);}

	while (x.empty() == y.empty() && !y.empty()){
		x.pop_back();
		y.pop_back();
		if (!y.empty()){
			ASSERT_TRUE(*x.begin() == *y.begin());
			ASSERT_TRUE(*(x.end() - 1)  == *(y.end() - 1));
		}
		ASSERT_TRUE(x.size() == y.size());}}

// test pop_front
TEST(Acceptance, removals_2){
	int const NUM_TESTS = 1000;

	std::deque<int> x;
	MyDeque<int> y;
	int rand_value;
	for (int i = 0; i < NUM_TESTS; ++i){
		rand_value = rand() % 99;
		x.push_front(rand_value);
		y.push_front(rand_value);
		rand_value = rand() % 99;
		x.push_back(rand_value);
		y.push_back(rand_value);}
	while (x.empty() == y.empty() && !y.empty()){
		x.pop_front();
		y.pop_front();
		if (!y.empty()){
			ASSERT_TRUE(*x.begin() == *y.begin());
			ASSERT_TRUE(*(x.end() - 1)  == *(y.end() - 1));
		}
		ASSERT_TRUE(x.size() == y.size());}}

// test erase(end())
TEST(Acceptance, removals_3){
	int const NUM_TESTS = 1000;

	std::deque<int> x;
	MyDeque<int> y;
	int rand_value;
	for (int i = 0; i < NUM_TESTS; ++i){
		rand_value = rand() % 99;
		x.push_front(rand_value);
		y.push_front(rand_value);
		rand_value = rand() % 99;
		x.push_back(rand_value);
		y.push_back(rand_value);}
	
	while (x.empty() == y.empty() && !y.empty()){
		x.erase(x.end());
		y.erase(y.end());
		if (!y.empty()){
			ASSERT_TRUE(*x.begin() == *y.begin());
			ASSERT_TRUE(*(x.end() - 1)  == *(y.end() - 1));
		}
		ASSERT_TRUE(x.size() == y.size());}}

// test erase(begin()) - special case should not be linear
TEST(Acceptance, removals_4){
	int const NUM_TESTS = 1000;

	std::deque<int> x;
	MyDeque<int> y;
	int rand_value;
	for (int i = 0; i < NUM_TESTS; ++i){
		rand_value = rand() % 99;
		x.push_front(rand_value);
		y.push_front(rand_value);
		rand_value = rand() % 99;
		x.push_back(rand_value);
		y.push_back(rand_value);}

	while (x.empty() == y.empty() && !y.empty()){
		x.erase(x.begin());
		y.erase(y.begin());
		if (!y.empty()){
			ASSERT_TRUE(*x.begin() == *y.begin());
			ASSERT_TRUE(*(x.end() - 1)  == *(y.end() - 1));
		}
		ASSERT_TRUE(x.size() == y.size());}}

// test erase(begin() + 1) - should be linear, can't optimize without random access iterator
TEST(Acceptance, removals_5){
	int const NUM_TESTS = 1000;

	std::deque<int> x;
	MyDeque<int> y;
	int rand_value;
	for (int i = 0; i < NUM_TESTS; ++i){
		rand_value = rand() % 99;
		x.push_front(rand_value);
		y.push_front(rand_value);
		rand_value = rand() % 99;
		x.push_back(rand_value);
		y.push_back(rand_value);}

	while (x.empty() == y.empty() && !y.empty()){
		x.erase(x.begin() + 1);
		y.erase(y.begin() + 1);
		if (!y.empty()){
			ASSERT_TRUE(*x.begin() == *y.begin());
			ASSERT_TRUE(*(x.end() - 1)  == *(y.end() - 1));
		}
		ASSERT_TRUE(x.size() == y.size());}}

// insert(begin()) - should be O(1), insert recognizes it's beginning and calls push_front
TEST(Acceptance, insert_1){
	int const NUM_TESTS = 1000;

	std::deque<int> x;
	MyDeque<int> y;
	int rand_value;
	for (int i = 0; i < NUM_TESTS; ++i){
		rand_value = rand() % 99;
		x.insert(x.begin(), rand_value);
		y.insert(y.begin(), rand_value);}

	for (int i = 0; i < NUM_TESTS; ++i){
		ASSERT_TRUE(x.at(i) == y.at(i));}}

// insert(begin() + 1) - O(n) hard to optimize further without random access iterator
// otherwise, we could copy at the front if the position is near the front
TEST(Acceptance, insert_2){
	int const NUM_TESTS = 1000;

	std::deque<int> x(1);
	MyDeque<int> y(1);
	int rand_value;
	for (int i = 0; i < NUM_TESTS; ++i){
		rand_value = rand() % 99;
		x.insert(x.begin() + 1, rand_value);
		y.insert(y.begin() + 1, rand_value);}

	for (int i = 0; i < NUM_TESTS; ++i){
		ASSERT_TRUE(x.at(i) == y.at(i));}}

// insert(end())
TEST(Acceptance, insert_3){
	int const NUM_TESTS = 1000;

	std::deque<int> x;
	MyDeque<int> y;
	int rand_value;
	for (int i = 0; i < NUM_TESTS; ++i){
		rand_value = rand() % 99;
		x.insert(x.end(), rand_value);
		y.insert(y.end(), rand_value);}

	for (int i = 0; i < NUM_TESTS; ++i){
		ASSERT_TRUE(x.at(i) == y.at(i));}}

// insert(end() - 1)
TEST(Acceptance, insert_4){
	int const NUM_TESTS = 1000;

	std::deque<int> x(1);
	MyDeque<int> y(1);
	int rand_value;
	for (int i = 0; i < NUM_TESTS; ++i){
		rand_value = rand() % 99;
		x.insert(x.end() - 1, rand_value);
		y.insert(y.end() - 1, rand_value);}

	for (int i = 0; i < NUM_TESTS; ++i){
		ASSERT_TRUE(x.at(i) == y.at(i));}}