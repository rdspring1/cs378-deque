// ----------------------
// projects/deque/Deque.h
// Copyright (C) 2013
// Glenn P. Downing
// ----------------------

#ifndef Deque_h
#define Deque_h

// Global Constant
const ptrdiff_t SIZET = 10;
const size_t USIZET = 10;

// --------
// includes
// --------

#include <algorithm> // copy, equal, lexicographical_compare, max, swap
#include <cassert>   // assert
#include <iterator>  // iterator, bidirectional_iterator_tag
#include <memory>    // allocator
#include <stdexcept> // out_of_range
#include <utility>   // !=, <=, >, >=

// -----
// using
// -----

using std::rel_ops::operator!=;
using std::rel_ops::operator<=;
using std::rel_ops::operator>;
using std::rel_ops::operator>=;

// -------
// destroy
// -------

///
/// To call the destructor of a range of elements using an allocator
/// @tparam A - Allocator Type
/// @tparam BI - Bidirectional Iterator
/// @param a - a reference to an allocator
/// @param b - a bidirectional iterator at the beginning of the range
/// @param e - a bidirectional iterator at the end of the range
/// @return a bidirection iterator at the beginning of the range
///
template <typename A, typename BI>
BI destroy (A& a, BI b, BI e) 
{
	while (b != e) 
	{
		--e;
		a.destroy(&*e);
	}
	return b;
}

// ------------------
// uninitialized_copy
// ------------------

///
/// To initialize a range of values by copying another range using the copy constructor
/// @tparam A - Allocator Type
/// @tparam II - Input Iterator
/// @tparam BI - Bidirectional Iterator
/// @param a - a reference to an allocator
/// @param b - an input iterator at the beginning of the range to be copied
/// @param e - an input iterator at the end of the range to be copied
/// @param x - a bidirectional iterator at the beginning of the destination range
/// @return a bidirection iterator at the end of the destination range
///
template <typename A, typename II, typename BI>
BI uninitialized_copy (A& a, II b, II e, BI x) 
{
	BI p = x;
	try 
	{
		while (b != e) 
		{
			a.construct(&*x, *b);
			++b;
			++x;
		}
	}
	catch (...) 
	{
		destroy(a, p, x);
		throw;
	}
	return x;
}

// ------------------
// uninitialized_fill
// ------------------

///
/// To initialize a range of values by using a specified value
/// @tparam A - Allocator Type
/// @tparam BI - Bidirectional Iterator
/// @tparam U - Value Type
/// @param a - a reference to an allocator
/// @param b - an bidirectional iterator at the beginning of the destination range
/// @param e - an bidirectional iterator at the end of the destination range
/// @param v - the value used to construct the element of the range
/// @return a bidirection iterator at the end of the destination range
///
template <typename A, typename BI, typename U>
BI uninitialized_fill (A& a, BI b, BI e, const U& v) 
{
	BI p = b;
	try 
	{
		while (b != e) 
		{
			a.construct(&*b, v);
			++b;
		}
	}
	catch (...) 
	{
		destroy(a, p, b);
		throw;
	}
	return e;
}

// -------
// MyDeque
// -------

///
/// An alternate implementation of the deque container
/// @tparam T - Type of the elements
/// @tparam A - Type of Allocator object used to define the storage allocation model; The default value - std::allocator
///
template < typename T, typename A = std::allocator<T> >
class MyDeque 
{
public:
	// --------
	// typedefs
	// --------

	typedef A                                        allocator_type;
	typedef typename allocator_type::value_type      value_type;

	typedef typename allocator_type::size_type       size_type;
	typedef typename allocator_type::difference_type difference_type;

	typedef typename allocator_type::pointer         pointer;
	typedef typename allocator_type::const_pointer   const_pointer;

	typedef typename allocator_type::reference       reference;
	typedef typename allocator_type::const_reference const_reference;

	typedef typename allocator_type::template rebind<pointer>::other astar_type; 

public:
	// -----------
	// operator ==
	// -----------

	/**
	* equal operator
	* @param lhs - the left hand side MyDeque
	* @param rhs - the right hand side MyDeque
	* @return true if the lhs MyDeque is equal to the rhs MyDeque
	*/
	friend bool operator == (const MyDeque& lhs, const MyDeque& rhs) 
	{
		return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	// ----------
	// operator <
	// ----------

	/**
	* less than operator
	* @param lhs - the left hand side MyDeque
	* @param rhs - the right hand side MyDeque
	* @return true if the lhs MyDeque is lexicographically less than the rhs MyDeque
	*/
	friend bool operator < (const MyDeque& lhs, const MyDeque& rhs) 
	{
		return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

private:
	// ----
	// data
	// ----

	allocator_type _a;
	astar_type _astar;
	pointer* cb;
	pointer* ce;
	pointer* pb;
	pointer* pe;
	pointer b;
	pointer e;
	size_type count;

private:
	// -----
	// valid
	// -----

	///
	/// @return true if the MyDeque object is in a valid state
	///
	bool valid () const 
	{
		// Empty
		if (cb == 0 && ce == 0 && pb == 0 && pe == 0 && b == 0 && e == 0 && count == 0)
			return true;
		// Non-Empty Non Circular
		if(cb <= pb && pb <= pe && pe <= ce && count >= 0)
			return true;
		return false;
	}

	///
	/// Fill the MyDeque container's inner arrays with specified value
	/// @param add - the number of elements to be added to the MyDeque container
	/// @param v - the value used to construct the elements of the MyDeque container
	/// @param x - a pointer traversing the outer array of the deque
	///
	void ia_fill (int add, const_reference v, pointer* x)
	{
		for(; add > 0; add -= SIZET)
		{
			size_type inner_array = (add >= SIZET) ? SIZET : add;
			e = uninitialized_fill(_a, *x, *x + inner_array, v);
			++x;
		}
	}

	///
	/// Copy elements from another container to the MyDeque container's inner arrays
	/// @tparam RI - a random access iterator
	/// @param add - the number of elements to be added to the MyDeque container 
	/// @param bIter - an random access iterator at the beginning of the range to be copied
	/// @param x - a pointer traversing the outer array of the deque
	///
	template<typename RI>
	void ia_copy (int add, RI bIter, pointer* x)
	{
		RI eIter = bIter;
		for(; add > 0; add -= SIZET)
		{
			size_type inner_array = (add >= SIZET) ? SIZET : add;
			eIter += inner_array;
			uninitialized_copy(_a, bIter, eIter, *x);
			bIter += inner_array;
			++x;
		}
	}

	/// Copy elements from another container to the MyDeque container by swapping inner arrays
	/// @param begin - a pointer to the beginning of the source range
	/// @param end - a pointer to the end of the source range
	/// @param dest - a pointer traversing the outer array of the MyDeque at the beginning of the destination range
	///
	void ia_copy (pointer* begin, pointer* end, pointer* dest)
	{
		while(begin != end)
		{
			std::swap(*begin, *dest);
			++begin;
			++dest;
		}
	}

	///
	/// Allocate memory for the inner arrays of the deque container
	/// @param _b - a pointer to the beginning of the deque container's outer array
	/// @param _e - a pointer to the end of the deque container's outer array
	///
	void allocate(pointer* _b, pointer* _e)
	{
		while(_b != _e)
		{
			*_b = _a.allocate(SIZET);
			++_b;
		}
		b = pb[0];
		e = pe[0];
	}

	///
	/// Replicate the MyDeque container and expand the capacity of the container 
	/// @param s - the minimum capacity of the new MyDeque container
	///
	void rebuild(size_type s)
	{
		MyDeque x(*this, s);
		this->swap(x);
	}

	///
	/// This private constructor creates a copy of a MyDeque container and expand the capacity of the container
	/// @param that - an other MyDeque container
	/// @param s - the minimum capacity of the new MyDeque container
	///
	MyDeque (MyDeque& that, size_type s) : _a (that._a) 
	{
		assert(s >= that.size());
		// # of outer arrays used to store old data
		size_type copy_array = (that.size() % SIZET) ? that.size() / SIZET + 1 : that.size() / SIZET;
		// # of outer arrays for the rebuilt MyDeque
		size_type outer_array = (s % SIZET) ? s / SIZET + 1 : s / SIZET;
		((outer_array *= 2) % 2) ? outer_array : outer_array += 1;
		// Allocate Memory
		cb = _astar.allocate(outer_array);
		ce = cb + outer_array;
		pb = cb + outer_array / 2;
		pe = pb + copy_array;
		allocate(cb, ce);

		// Copy old data
		if(copy_array > 0)
		{
			ptrdiff_t bdiff = that.b - *that.pb;
			assert(bdiff <= SIZET);
			if(that.pb == that.pe)
				ia_copy(that.pb, that.pe + 1, this->pb);
			else
				ia_copy(that.pb, that.pe, this->pb);
			b = *pb + bdiff;
			e = *pe;
		}
		this->count = that.count;
		that.count = 0;
		assert(valid());
	}

public:
	// --------------
	// const_iterator
	// --------------

	///
	/// A const bidirectional iterator for the MyDeque class
	///
	class const_iterator 
	{
	public:
		// --------
		// typedefs
		// --------

		typedef std::bidirectional_iterator_tag   iterator_category;
		typedef typename MyDeque::value_type      value_type;
		typedef typename MyDeque::difference_type difference_type;
		typedef typename MyDeque::const_pointer   pointer;
		typedef typename MyDeque::const_reference reference;

	public:
		// -----------
		// operator ==
		// -----------

		/**
		* equal operator
		* @param lhs - the left hand side Const_Iterator
		* @param rhs - the right hand side Const_Iterator
		* @return true if the lhs Const_Iterator is equal to the rhs Const_Iterator
		*/
		friend bool operator == (const const_iterator& lhs, const const_iterator& rhs) 
		{
			return (lhs._p == rhs._p) && (lhs._index == rhs._index);
		}

		/**
		* not equal operator
		* @param lhs - the left hand side Const_Iterator
		* @param rhs - the right hand side Const_Iterator
		* @return true if the lhs Const_Iterator is not equal to the rhs Const_Iterator
		*/
		friend bool operator != (const const_iterator& lhs, const const_iterator& rhs) 
		{
			return !(lhs == rhs);
		}

		// ----------
		// operator +
		// ----------

		/**
		* addition operator
		* @param lhs - the left hand side Const_Iterator
		* @param rhs - the right hand side difference_type
		* @return an Const_Iterator shifted forward by the difference_type value
		*/
		friend const_iterator operator + (const_iterator lhs, difference_type rhs) 
		{
			return lhs += rhs;
		}

		// ----------
		// operator -
		// ----------

		/**
		* subtraction operator
		* @param lhs - the left hand side Const_Iterator
		* @param rhs - the right hand side difference_type
		* @return an Const_Iterator shifted backward by the difference_type value
		*/
		friend const_iterator operator - (const_iterator lhs, difference_type rhs) 
		{
			return lhs -= rhs;
		}

	private:
		// ----
		// data
		// ----
		const MyDeque* _p;
		size_type    _index;

	private:
		// -----
		// valid
		// -----

		/**
		* @return true if the Const_Iterator object is in a valid state
		*/
		bool valid () const 
		{
			return (_p == nullptr && _index == 0) || _index >= 0;
		}

	public:
		// -----------
		// constructor
		// -----------

		/**
		* Create an Const_Iterator object using the MyDeque container
		* @param p - a const pointer to the MyDeque container
		* @param i - index state for the Const_Iterator
		*/
		const_iterator (const MyDeque* p = nullptr, size_type i = 0) : _p(p), _index(i)
		{
			assert(valid());
		}

		// Default copy, destructor, and copy assignment.
		// const_iterator (const const_iterator&);
		// ~const_iterator ();
		// const_iterator& operator = (const const_iterator&);

		// ----------
		// operator *
		// ----------

		/**
		* dereference operator
		* @return a reference to the value in the Const_Iterator's current state
		*/
		reference operator * () const 
		{
			assert(_index <= _p->size());
			return _p->operator[](_index);
		}

		// -----------
		// operator ->
		// -----------

		/**
		* pointer member access operator
		* @return a pointer to the value in the Const_Iterator's current state
		*/
		pointer operator -> () const 
		{
			return &**this;
		}

		// -----------
		// operator ++
		// -----------

		/**
		* Pre-increment Operator
		* @return a Const_Iterator reference incremented by 1
		*/
		const_iterator& operator ++ () 
		{
			++_index;
			assert(valid());
			return *this;
		}

		/**
		* Post-Increment Operator
		* Does not effect the Const_Iterator argument
		* @return an Const_Iterator incremented by 1
		*/
		const_iterator operator ++ (int) 
		{
			const_iterator x = *this;
			++(*this);
			assert(valid());
			return x;
		}

		// -----------
		// operator --
		// -----------

		/**
		* Pre-decrement Operator
		* @return a Const_Iterator reference decremented by 1
		*/
		const_iterator& operator -- () 
		{
			--_index;
			assert(valid());
			return *this;
		}

		/**
		* Post-Decrement Operator
		* Does not effect the Const_Iterator argument
		* @return an Const_Iterator decremented by 1
		*/
		const_iterator operator -- (int) 
		{
			const_iterator x = *this;
			--(*this);
			assert(valid());
			return x;
		}

		// -----------
		// operator +=
		// -----------

		/**
		* Addition Assignent Operator
		* @param d - the right hand side difference_type
		* @return a Const_Iterator reference shifted forward by the difference_type value
		*/
		const_iterator& operator += (difference_type d) 
		{
			_index += d;
			assert(valid());
			return *this;
		}

		// -----------
		// operator -=
		// -----------

		/**
		* Subtraction Assignent Operator
		* @param d - the right hand side difference_type
		* @return a Const_Iterator reference shifted backward by the difference_type value
		*/
		const_iterator& operator -= (difference_type d) 
		{
			_index -= d;
			assert(valid());
			return *this;
		}
	};

public:
	// --------
	// iterator
	// --------

	///
	/// A modifiable bidirectional iterator for the MyDeque class
	///
	class iterator 
	{
	public:
		// --------
		// typedefs
		// --------

		typedef std::bidirectional_iterator_tag   iterator_category;
		typedef typename MyDeque::value_type      value_type;
		typedef typename MyDeque::difference_type difference_type;
		typedef typename MyDeque::pointer         pointer;
		typedef typename MyDeque::reference       reference;

	public:
		// -----------
		// operator ==
		// -----------

		/**
		* equal operator
		* @param lhs - the left hand side Iterator
		* @param rhs - the right hand side Iterator
		* @return true if the lhs Iterator is equal to the rhs Iterator
		*/
		friend bool operator == (const iterator& lhs, const iterator& rhs) 
		{
			return (lhs._p == rhs._p) && (lhs._index == rhs._index);
		}

		/**
		* not equal operator
		* @param lhs - the left hand side Iterator
		* @param rhs - the right hand side Iterator
		* @return true if the lhs Iterator is not equal to the rhs Iterator
		*/
		friend bool operator != (const iterator& lhs, const iterator& rhs) 
		{
			return !(lhs == rhs);
		}

		// ----------
		// operator +
		// ----------

		/**
		* addition operator
		* @param lhs - the left hand side Iterator
		* @param rhs - the right hand side difference_type
		* @return an Iterator shifted forward by the difference_type value
		*/
		friend iterator operator + (iterator lhs, difference_type rhs) 
		{
			return lhs += rhs;
		}

		// ----------
		// operator -
		// ----------

		/**
		* subtraction operator
		* @param lhs - the left hand side Iterator
		* @param rhs - the right hand side difference_type
		* @return an Iterator shifted backward by the difference_type value
		*/
		friend iterator operator - (iterator lhs, difference_type rhs) 
		{
			return lhs -= rhs;
		}

	private:
		// ----
		// data
		// ----
		MyDeque* _p;
		size_type    _index;

	private:
		// -----
		// valid
		// -----

		/**
		* @return true if the Iterator object is in a valid state
		*/
		bool valid () const 
		{
			return (_p == nullptr && _index == 0) || _index >= 0;
		}

	public:
		// -----------
		// constructor
		// -----------

		/**
		* Create an Iterator object using the MyDeque container
		* @param p - a pointer to the MyDeque container
		* @param i - index state for the Iterator
		*/
		iterator (MyDeque* p = nullptr, size_type i = 0) : _p(p), _index(i)
		{
			assert(valid());
		}

		// Default copy, destructor, and copy assignment.
		// iterator (const iterator&);
		// ~iterator ();
		// iterator& operator = (const iterator&);

		// ----------
		// operator *
		// ----------

		/**
		* dereference operator
		* @return a reference to the value in the Iterator's current state
		*/
		reference operator * () const 
		{
			assert(_index <= _p->size()); 
			return _p->operator[](_index);
		}

		// -----------
		// operator ->
		// -----------

		/**
		* pointer member access operator
		* @return a pointer to the value in the Iterator's current state
		*/
		pointer operator -> () const 
		{
			return &**this;
		}

		// -----------
		// operator ++
		// -----------

		/**
		* Pre-increment Operator
		* @return a Iterator reference incremented by 1
		*/
		iterator& operator ++ () 
		{
			++_index;
			assert(valid());
			return *this;
		}

		/**
		* Post-Increment Operator
		* Does not effect the Iterator argument
		* @return an Iterator incremented by 1
		*/
		iterator operator ++ (int) 
		{
			iterator x = *this;
			++(*this);
			assert(valid());
			return x;
		}

		// -----------
		// operator --
		// -----------

		/**
		* Pre-decrement Operator
		* @return a Iterator reference decremented by 1
		*/
		iterator& operator -- () 
		{
			--_index;
			assert(valid());
			return *this;
		}

		/**
		* Post-Decrement Operator
		* Does not effect the Iterator argument
		* @return an Iterator decremented by 1
		*/
		iterator operator -- (int) 
		{
			iterator x = *this;
			--(*this);
			assert(valid());
			return x;
		}

		// -----------
		// operator +=
		// -----------

		/**
		* Addition Assignent Operator
		* @param d - the right hand side difference type
		* @return a Iterator reference shifted forward by the difference_type value
		*/
		iterator& operator += (difference_type d) 
		{
			_index += d;
			assert(valid());
			return *this;
		}

		// -----------
		// operator -=
		// -----------

		/**
		* Subtraction Assignent Operator
		* @param d - the right hand side difference type
		* @return a Iterator reference shifted backward by the difference_type value
		*/
		iterator& operator -= (difference_type d) 
		{
			_index -= d;
			assert(valid());
			return *this;
		}

		operator const_iterator ()
		{
			return const_iterator(_p, _index);
		}

	};

public:
	// ------------
	// constructors
	// ------------

	/**
	* Default Constructor - Empty MyDeque
	* @param a - an optional argument for an allocator object
	*/
	explicit MyDeque (const allocator_type& a = allocator_type()) : _a (a)
	{
		cb = 0;
		ce = 0;
		pb = 0;
		pe = 0;
		b = 0;
		e = 0;
		count = 0;
		assert(valid());
	}

	/**
	* Fill Constructor - Create MyDeque with specified size and type
	* @param s - Initial Container Size
	* @param v - an optional argument for a value used to initialize the container
	* @param a - an optional argument for an allocator object
	*/
	explicit MyDeque (size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type()) : _a (a), count(s)
	{
		if(s != 0)
		{
			size_type outer_array = (s % SIZET) ? s / SIZET + 1 : s / SIZET;
			pb = cb = _astar.allocate(outer_array);
			pe = (s < USIZET) ? pb + outer_array - 1 : pb + outer_array;
			ce = cb + outer_array;
			allocate(cb, ce);
			ia_fill(s, v, pb);
		}
		else
		{
			cb = 0;
			ce = 0;
			pb = 0;
			pe = 0;
			b = 0;
			e = 0;
		}
		assert(valid());
	}

	/**
	* Copy Constructor - Create a copy of a MyDeque container 
	* @param that - another MyDeque object
	*/
	MyDeque (const MyDeque& that) : _a (that._a)
	{
		count = that.size();
		if(count != 0)
		{
			size_type outer_array = (that.size() % SIZET) ? that.size() / SIZET + 1 : that.size() / SIZET;
			pb = cb = _astar.allocate(outer_array);
			pe = pb + outer_array;
			ce = cb + outer_array;
			allocate(cb, ce);
			ia_copy(that.size(), that.begin(), pb);
		}
		else
		{
			cb = 0;
			ce = 0;
			pb = 0;
			pe = 0;
			b = 0;
			e = 0;
		}
		assert(valid());
	}

	// ----------
	// destructor
	// ----------

	/**
	* Destructor - Destroys all objects and Deallocates all memory for MyDeque container
	*/
	~MyDeque () 
	{
		if(cb)
		{
			clear();
			pointer* x = cb;
			while(x != ce)
			{
				_a.deallocate(*x, SIZET);
				++x;
			}
			_astar.deallocate(cb, ce - cb);
		}
		assert(valid());
	}

	// ----------
	// operator =
	// ----------

	/**
	* Copy Assignment Operator
	* @param that - another MyDeque container
	* @return - a reference to this MyDeque container initialized to copied MyDeque
	*/
	MyDeque& operator = (const MyDeque& that) 
	{
		if (this == &that)
			return *this;

		// capacity = the number of elements from the allocated end to max end
		size_type capacity = (cb == nullptr) ? 0 : (ce - pb) * SIZET;
		if (that.size() == this->size()) // Equal Size
		{	
			std::copy(that.begin(), that.end(), this->begin());
		}
		else if (that.size() < this->size()) // Less than Size
		{		
			count = that.size();
			std::copy(that.begin(), that.end(), this->begin());
		}
		else if (that.size() <= capacity) // Less than or equal to capacity
		{
			count = that.size();
			std::copy(that.begin(), that.begin() + this->size(), this->begin());
			uninitialized_copy(_a, that.begin() + this->size(), that.end(), this->end());
		}
		else // Greater than capacity
		{
			clear();
			rebuild(that.size());
			count = that.size();
			uninitialized_copy(_a, that.begin(), that.end(), this->begin());
		}

		// Regenerate pe and e pointers
		if(cb != nullptr)
		{
			size_type offsetBOA = pb - cb;
			size_type offsetBIA = b - *pb;
			size_type outer_array_index = (this->size() + offsetBIA) / SIZET + offsetBOA;
			size_type inner_array_index = (this->size() + offsetBIA) % SIZET;
			pe = cb + outer_array_index;
			e = *pe + inner_array_index;
		}

		assert(valid());
		return *this;
	}

	// -----------
	// operator []
	// -----------

	/**
	* subscript operator
	* @param index - element position in the container
	* @return a reference to the element at the position in the container
	*/
	reference operator [] (size_type index) 
	{
		static value_type dummy;
		if(cb == nullptr)
			return dummy;
		size_type offsetBOA = pb - cb;
		size_type offsetBIA = b - *pb;
		size_type outer_array_index = (index + offsetBIA) / SIZET + offsetBOA;
		size_type inner_array_index = (index + offsetBIA) % SIZET;
		return cb[outer_array_index][inner_array_index];
	}

	/**
	* const subscript operator
	* @param index - element position in the container
	* @return a const reference to the element at the position in the container
	*/
	const_reference operator [] (size_type index) const 
	{
		return const_cast<MyDeque*>(this)->operator[](index);
	}

	// --
	// at
	// --

	/**
	* Returns a reference to the element at position index in the MyDeque container object
	* Checks whether position index is within the bounds of the MyDeque container
	* @param index - element position in the container
	* @return a reference to the element at the position in the container
	* @throws out_of_range exception if position index is not within the bounds of the MyDeque container
	*/
	reference at (size_type index) 
	{
		if (index < 0 || index >= size())
			throw std::out_of_range("deque::_M_range_check");
		return (*this)[index];
	}

	/**
	* Returns a const reference to the element at position index in the MyDeque container object
	* Checks whether position index is within the bounds of the MyDeque container
	* @param index - element position in the container
	* @return a const reference to the element at the position in the container
	* @throws out_of_range exception if position index is not within the bounds of the MyDeque container
	*/
	const_reference at (size_type index) const 
	{
		return const_cast<MyDeque*>(this)->at(index);
	}

	// ----
	// back
	// ----

	/**
	* Access last element
	* @return a reference to the last element of the MyDeque container
	*/
	reference back () 
	{
		assert(!empty());
		return *(end() - 1);
	}

	/**
	* Access last element
	* @return a const reference to the last element of the MyDeque container
	*/
	const_reference back () const 
	{
		return const_cast<MyDeque*>(this)->back();
	}

	// -----
	// begin
	// -----

	/**
	* @return an Iterator to the beginning of the MyDeque container
	*/
	iterator begin () 
	{
		return iterator(this, 0);
	}

	/**
	* @return a Const Iterator to the beginning of the MyDeque container
	*/
	const_iterator begin () const 
	{
		return const_iterator(this, 0);
	}

	// -----
	// clear
	// -----

	/**
	* Clear the content of the MyDeque Container
	* Removes and Destroys all elements of the MyDeque Container
	*/
	void clear () 
	{
		resize(0);
		assert(valid());
	}

	// -----
	// empty
	// -----

	/**
	* Test whether the MyDeque container is empty
	* @return true if the MyDeque container contains zero elements
	*/
	bool empty () const 
	{
		return !size();
	}

	// ---
	// end
	// ---

	/**
	* @return an Iterator to the end of the MyDeque container
	*/
	iterator end () 
	{
		return iterator(this, count);
	}

	/**
	* @return a Const Iterator to the end of the MyDeque container
	*/
	const_iterator end () const 
	{
		return const_iterator(this, count);
	}

	// -----
	// erase
	// -----

	/**
	* Erase element
	* @param iter - an iterator that points to the element to be removed
	* @return an iterator pointing to the new location of the element that followed the element erased by the function
	*/
	iterator erase (iterator iter) 
	{
		if(count < 1)
			return this->begin();
		--count;
		iterator lhs = iter;
		iterator rhs = iter + 1;
		destroy(_a, lhs, rhs);
		while(lhs != this->end())
		{
			std::swap(*rhs, *lhs);
			++lhs;
			++rhs;
		}
		assert(valid());
		return iter;
	}

	// -----
	// front
	// -----

	/**
	* Access first element
	* @return a reference to the first element of the MyDeque container
	*/
	reference front () 
	{
		assert(!empty());
		return *begin();
	}

	/**
	* Access first element
	* @return a const reference to the first element of the MyDeque container
	*/
	const_reference front () const 
	{
		return const_cast<MyDeque*>(this)->front();
	}

	// ------
	// insert
	// ------

	/**
	* Insert element
	* @param iter - an iterator that points where the new element will be inserted
	* @param v - a const reference to the value of the new element
	* @return an iterator that points to the new element in the MyDeque container
	*/
	iterator insert (iterator iter, const_reference v) 
	{
		if(cb == nullptr || e == *ce)
			rebuild(this->size() + 1);

		size_type ia_remain = SIZET - (e - *pe);
		if(ia_remain == 1)
		{
			++pe;
			e = *pe;
		}
		else
		{
			++e;
		}
		++count;
		iterator lhs = this->end() - 1;
		iterator rhs = this->end() - 2;
		while(lhs != iter)
		{
			std::swap(*lhs, *rhs);
			--lhs;
			--rhs;
		}
		uninitialized_fill(_a, iter, iter + 1, v);
		assert(valid());
		return iter;
	}

	// ---
	// pop
	// ---

	/**
	* Delete the last element of the MyDeque container
	*/
	void pop_back () 
	{
		assert(!empty());
		resize(size() - 1);
		assert(valid());
	}

	/**
	* Delete the first element of the MyDeque container
	*/
	void pop_front () 
	{
		destroy(_a, this->begin(), this->begin() + 1);
		++b;
		if(b - *pb == SIZET && count != 1)
		{
			++pb;
			b = *pb;
		}
		--count;
		assert(valid());
	}

	// ----
	// push
	// ----

	/**
	* Add element to the end of the MyDeque container
	* @param v - a const reference to the value of the new element
	*/
	void push_back (const_reference v) 
	{
		resize(this->size() + 1, v);
		assert(valid());
	}

	/**
	* Add element to the front of the MyDeque container
	* @param v - a const reference to the value of the new element
	*/
	void push_front (const_reference v) 
	{
		// Check capacity
		if(cb == nullptr || *cb == b)
			rebuild(this->size() + 1);

		int ia_remain = b - *pb;
		if(ia_remain == 0)
		{
			--pb;
			b = *pb + SIZET - 1;
		}
		else
		{
			--b;
		}
		uninitialized_fill(_a, this->begin(), this->begin() + 1, v);
		++count;            
		assert(valid());
	}

	// ------
	// resize
	// ------

	/**
	* Change size of the MyDeque container
	* New Size < Current Size - Remove and Destroy extra elements
	* New Size == Current Size - Nothing
	* New Size > Current Size - Add new elements initialized using the v argument
	* @param s - new size for MyDeque container
	* @param v - an optional argument whose data is copied to the new elements when the new size is greater than the current size
	*/
	void resize (size_type s, const_reference v = value_type()) 
	{
		size_type add = s - this->size();
		size_type capacity = (cb == nullptr) ? 0 : (ce - pe) * SIZET;
		if (s == this->size())
		{
			return;
		}
		if (s < this->size())
		{
			size_type offsetBOA = pb - cb;
			size_type offsetBIA = b - *pb;
			size_type outer_array_index = (s + offsetBIA) / SIZET + offsetBOA;
			size_type inner_array_index = (s + offsetBIA) % SIZET;
			pe = cb + outer_array_index;
			e = *pe + inner_array_index;
			size_type ia_remain = SIZET - (e - *pe);
			if(ia_remain == 0)
			{
				++pe;
				e = *pe;
			}
			destroy(_a, this->begin() + s, this->end());
			count = s;
		}
		else if (add <= capacity)
		{
			size_type ia_remain = SIZET - (e - *pe);
			if(ia_remain != 0)
			{
				int fillsize = std::min(add, ia_remain);
				uninitialized_fill(_a, e, e + fillsize, v);
				e += fillsize;
				add -= fillsize;
				ia_remain -= fillsize;
			}

			if(ia_remain == 0)
			{
				++pe;
				e = *pe;
			}

			if(add != 0)
			{
				ia_fill(add, v, pe);
				++pe;
			}

			count = s;
		}
		else 
		{
			rebuild(s);
			resize(s, v);
		}
		assert(valid());
	}

	// ----
	// size
	// ----

	/**
	* @return the number of elements in the MyDeque container
	*/
	size_type size () const 
	{
		return count;
	}

	// ----
	// swap
	// ----

	/**
	* Exchange the content of two MyDeque containers
	* @param that - another MyDeque container
	*/
	void swap (MyDeque& that) 
	{
		if (_a == that._a) 
		{
			std::swap(cb, that.cb);
			std::swap(ce, that.ce);
			std::swap(pb, that.pb);
			std::swap(pe, that.pe);
			std::swap(b, that.b);
			std::swap(e, that.e);
			std::swap(count, that.count);
		}
		else 
		{
			MyDeque x(*this);
			*this = that;
			that  = x;
		}
		assert(valid());
	}
};

#endif // Deque_h
