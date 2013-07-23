// ----------------------
// projects/deque/Deque.h
// Copyright (C) 2013
// Glenn P. Downing
// ----------------------

#ifndef Deque_h
#define Deque_h

// Global Constant
const int SIZE = 10;

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

    public:
        // -----------
        // operator ==
        // -----------

        /**
         * <your documentation>
         */
        friend bool operator == (const MyDeque& lhs, const MyDeque& rhs) 
        {
            // <your code>
            // you must use std::equal()
            return true;
        }

        // ----------
        // operator <
        // ----------

        /**
         * <your documentation>
         */
        friend bool operator < (const MyDeque& lhs, const MyDeque& rhs) 
        {
            // <your code>
            // you must use std::lexicographical_compare()
            return true;
        }

    private:
        // ----
        // data
        // ----

        allocator_type _a;
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

        bool valid () const 
        {
            // <your code>
            return true;
        }

    public:
        // --------
        // iterator
        // --------

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
                 * <your documentation>
                 */
                friend bool operator == (const iterator& lhs, const iterator& rhs) 
                {
                    // <your code>
                    return true;
                }

                /**
                 * <your documentation>
                 */
                friend bool operator != (const iterator& lhs, const iterator& rhs) 
                {
                    return !(lhs == rhs);
                }

                // ----------
                // operator +
                // ----------

                /**
                 * <your documentation>
                 */
                friend iterator operator + (iterator lhs, difference_type rhs) 
                {
                    return lhs += rhs;
                }

                // ----------
                // operator -
                // ----------

                /**
                 * <your documentation>
                 */
                friend iterator operator - (iterator lhs, difference_type rhs) 
                {
                    return lhs -= rhs;
                }

            private:
                // ----
                // data
                // ----

                // <your data>

            private:
                // -----
                // valid
                // -----

                bool valid () const 
                {
                    // <your code>
                    return true;
                }

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * <your documentation>
                 */
                iterator (/* <your arguments> */) 
                {
                    // <your code>
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
                 * <your documentation>
                 */
                reference operator * () const 
                {
                    // <your code>
                    // dummy is just to be able to compile the skeleton, remove it
                    static value_type dummy;
                    return dummy;
                }

                // -----------
                // operator ->
                // -----------

                /**
                 * <your documentation>
                 */
                pointer operator -> () const 
                {
                    return &**this;
                }

                // -----------
                // operator ++
                // -----------

                /**
                 * <your documentation>
                 */
                iterator& operator ++ () 
                {
                    // <your code>
                    assert(valid());
                    return *this;
                }

                /**
                 * <your documentation>
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
                 * <your documentation>
                 */
                iterator& operator -- () 
                {
                    // <your code>
                    assert(valid());
                    return *this;
                }

                /**
                 * <your documentation>
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
                 * <your documentation>
                 */
                iterator& operator += (difference_type d) 
                {
                    // <your code>
                    assert(valid());
                    return *this;
                }

                // -----------
                // operator -=
                // -----------

                /**
                 * <your documentation>
                 */
                iterator& operator -= (difference_type d) 
                {
                    // <your code>
                    assert(valid());
                    return *this;
                }
        };

    public:
        // --------------
        // const_iterator
        // --------------

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
                 * <your documentation>
                 */
                friend bool operator == (const const_iterator& lhs, const const_iterator& rhs) 
                {
                    // <your code>
                    return true;
                }

                /**
                 * <your documentation>
                 */
                friend bool operator != (const const_iterator& lhs, const const_iterator& rhs) 
                {
                    return !(lhs == rhs);
                }

                // ----------
                // operator +
                // ----------

                /**
                 * <your documentation>
                 */
                friend const_iterator operator + (const_iterator lhs, difference_type rhs) 
                {
                    return lhs += rhs;
                }

                // ----------
                // operator -
                // ----------

                /**
                 * <your documentation>
                 */
                friend const_iterator operator - (const_iterator lhs, difference_type rhs) 
                {
                    return lhs -= rhs;
                }

            private:
                // ----
                // data
                // ----

                // <your data>

            private:
                // -----
                // valid
                // -----

                bool valid () const {
                    // <your code>
                    return true;}

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator (/* <your arguments> */) 
                {
                    // <your code>
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
                 * <your documentation>
                 */
                reference operator * () const 
                {
                    // <your code>
                    // dummy is just to be able to compile the skeleton, remove it
                    static value_type dummy;
                    return dummy;
                }

                // -----------
                // operator ->
                // -----------

                /**
                 * <your documentation>
                 */
                pointer operator -> () const 
                {
                    return &**this;
                }

                // -----------
                // operator ++
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator& operator ++ () 
                {
                    // <your code>
                    assert(valid());
                    return *this;
                }

                /**
                 * <your documentation>
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
                 * <your documentation>
                 */
                const_iterator& operator -- () 
                {
                    // <your code>
                    assert(valid());
                    return *this;
                }

                /**
                 * <your documentation>
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
                 * <your documentation>
                 */
                const_iterator& operator += (difference_type) 
                {
                    // <your code>
                    assert(valid());
                    return *this;
                }

                // -----------
                // operator -=
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator& operator -= (difference_type) 
                {
                    // <your code>
                    assert(valid());
                    return *this;
                }
        };

    public:
        // ------------
        // constructors
        // ------------

        /**
         * <your documentation>
         */
        explicit MyDeque (const allocator_type& a = allocator_type()) : _a (a)
        {
            cb = pb = b = pe = e = ce = 0;
			count = 0;
            assert(valid());
        }

        /**
         * <your documentation>
         */
        explicit MyDeque (size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type()) : _a (a)
        {
            size_type outer_array = (s % SIZE) ? s / SIZE + 1 : s / SIZE;
			allocator_type::rebind<pointer>::other _astar;
			pe = pb = cb = _astar.allocate(outer_array);
			ce = cb + outer_array;

			for(int i = s; i > 0; i -= SIZE)
			{
				size_type inner_array = (i >= SIZE) ? SIZE : i;
				*pe = _a.allocate(SIZE);
				e = uninitialized_fill(_a, *pe, *pe + inner_array, v);
				++pe;
			}
			b = pb[0];
			count = s;
            assert(valid());
        }

        /**
         * <your documentation>
         */
        MyDeque (const MyDeque& that) : _a (that.a)
        {
			count = that.size();
            size_type outer_array = (that.size() % SIZE) ? that.size() / SIZE + 1 : that.size() / SIZE;
			allocator_type::rebind<pointer>::other _astar;
			pe = pb = cb = _astar.allocate(outer_array);
			ce = cb + outer_array;

			auto bIter = that.begin();
			auto eIter = that.begin();
			for(int i = that.size(); i > 0; i -= SIZE)
			{
				size_type inner_array = (i >= SIZE) ? SIZE : i;
				eIter += inner_array;
				*pe = _a.allocate(SIZE);
				e = uninitialized_copy(_a, bIter, eIter, *pe);
				bIter += inner_array;
				++pe;
			}
			b = pb[0];
            assert(valid());
        }

        // ----------
        // destructor
        // ----------

        /**
         * <your documentation>
         */
        ~MyDeque () 
        {
            // <your code>
            assert(valid());
        }

        // ----------
        // operator =
        // ----------

        /**
         * <your documentation>
         */
        MyDeque& operator = (const MyDeque& rhs) 
        {
            // <your code>
            assert(valid());
            return *this;
        }

        // -----------
        // operator []
        // -----------

        /**
         * <your documentation>
         */
        reference operator [] (size_type index) 
        {
			size_type offsetBOA = cb - pb;
            size_type offsetBIA = b - *pb;
			size_type outer_array_index = (index + offsetBIA) / SIZE + offsetBOA;
			size_type inner_array_index = (index + offsetBIA) % SIZE;
			return cb[outer_array_index][inner_array_index];
        }

        /**
         * <your documentation>
         */
        const_reference operator [] (size_type index) const 
        {
            return const_cast<MyDeque*>(this)->operator[](index);
        }

        // --
        // at
        // --

        /**
         * <your documentation>
         */
        reference at (size_type index) 
        {
            // <your code>
            // dummy is just to be able to compile the skeleton, remove it
            static value_type dummy;
            return dummy;
        }

        /**
         * <your documentation>
         */
        const_reference at (size_type index) const 
        {
            return const_cast<MyDeque*>(this)->at(index);
        }

        // ----
        // back
        // ----

        /**
         * <your documentation>
         */
        reference back () 
        {
            // <your code>
            // dummy is just to be able to compile the skeleton, remove it
            static value_type dummy;
            return dummy;
        }

        /**
         * <your documentation>
         */
        const_reference back () const 
        {
            return const_cast<MyDeque*>(this)->back();
        }

        // -----
        // begin
        // -----

        /**
         * <your documentation>
         */
        iterator begin () 
        {
            // <your code>
            return iterator(/* <your arguments> */);
        }

        /**
         * <your documentation>
         */
        const_iterator begin () const 
        {
            // <your code>
            return const_iterator(/* <your arguments> */);
        }

        // -----
        // clear
        // -----

        /**
         * <your documentation>
         */
        void clear () 
        {
            // <your code>
            assert(valid());
        }

        // -----
        // empty
        // -----

        /**
         * <your documentation>
         */
        bool empty () const 
        {
            return !size();
        }

        // ---
        // end
        // ---

        /**
         * <your documentation>
         */
        iterator end () 
        {
            // <your code>
            return iterator(/* <your arguments> */);
        }

        /**
         * <your documentation>
         */
        const_iterator end () const 
        {
            // <your code>
            return const_iterator(/* <your arguments> */);
        }

        // -----
        // erase
        // -----

        /**
         * <your documentation>
         */
        iterator erase (iterator) 
        {
            // <your code>
            assert(valid());
            return iterator();
        }

        // -----
        // front
        // -----

        /**
         * <your documentation>
         */
        reference front () 
        {
            // <your code>
            // dummy is just to be able to compile the skeleton, remove it
            static value_type dummy;
            return dummy;
        }

        /**
         * <your documentation>
         */
        const_reference front () const 
        {
            return const_cast<MyDeque*>(this)->front();
        }

        // ------
        // insert
        // ------

        /**
         * <your documentation>
         */
        iterator insert (iterator, const_reference) 
        {
            // <your code>
            assert(valid());
            return iterator();
        }

        // ---
        // pop
        // ---

        /**
         * <your documentation>
         */
        void pop_back () 
        {
            // <your code>
            assert(valid());
        }

        /**
         * <your documentation>
         */
        void pop_front () 
        {
            // <your code>
            assert(valid());
        }

        // ----
        // push
        // ----

        /**
         * <your documentation>
         */
        void push_back (const_reference) 
        {
            // <your code>
            assert(valid());
        }

        /**
         * <your documentation>
         */
        void push_front (const_reference) 
        {
            // <your code>
            assert(valid());
        }

        // ------
        // resize
        // ------

        /**
         * <your documentation>
         */
        void resize (size_type s, const_reference v = value_type()) 
        {
            // <your code>
            assert(valid());
        }

        // ----
        // size
        // ----

        /**
         * <your documentation>
         */
        size_type size () const 
        {
            return count;
        }

        // ----
        // swap
        // ----

        /**
         * <your documentation>
         */
        void swap (MyDeque&) 
        {
            // <your code>
            assert(valid());
        }
    };

#endif // Deque_h
