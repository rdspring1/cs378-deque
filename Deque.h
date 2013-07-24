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
            return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
        }

        // ----------
        // operator <
        // ----------

        /**
         * <your documentation>
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
			// Empty
            if (cb == 0 && ce == 0 && pb == 0 && pe == 0 && b == 0 && e == 0 && count == 0)
				return true;
			// Non-Empty Non Circular
			if(cb <= pb && pb <= pe && pe <= ce && b != 0 && e != 0 && count >= 0)
				return true;
			return false;
        }

		void ia_fill (int add, const_reference v)
		{
			for(; add > 0; add -= SIZE)
			{
				size_type inner_array = (add >= SIZE) ? SIZE : add;
				*pe = _a.allocate(SIZE);
				e = uninitialized_fill(_a, *pe, *pe + inner_array, v);
				++pe;
			}
		}

		template<typename I>
		void ia_copy (int add, I bIter)
		{
			I eIter = b;
			for(; add > 0; add -= SIZE)
			{
				size_type inner_array = (add >= SIZE) ? SIZE : add;
				eIter += inner_array;
				*pe = _a.allocate(SIZE);
				e = uninitialized_copy(_a, bIter, eIter, *pe);
				bIter += inner_array;
				++pe;
			}
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
                    return (lhs._p == rhs._p) && (lhs._index == rhs._index);
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
                MyDeque* _p;
                std::size_t    _index;

            private:
                // -----
                // valid
                // -----

                bool valid () const 
                {
					return _p != nullptr;
                }

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * <your documentation>
                 */
                iterator (MyDeque* p, std::size_t i) : _p(p), _index(i)
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
                 * <your documentation>
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
                    ++_index;
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
                    --_index;
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
                    _index += d;
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
                    _index -= d;
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
                    return (lhs._p == rhs._p) && (lhs._index == rhs._index);
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
				const MyDeque* _p;
                std::size_t    _index;

            private:
                // -----
                // valid
                // -----

                bool valid () const 
				{
                    return _p != nullptr;
				}

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator (const MyDeque* p, std::size_t i) : _p(p), _index(i)
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
                 * <your documentation>
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
                    ++_index;
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
                    --_index;
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
                 * <your documentation>
                 */
                const_iterator& operator -= (difference_type d) 
                {
                    _index -= d;
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
         * <your documentation>
         */
        explicit MyDeque (size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type()) : _a (a)
        {
            size_type outer_array = (s % SIZE) ? s / SIZE + 1 : s / SIZE;
			allocator_type::rebind<pointer>::other _astar;
			pe = pb = cb = _astar.allocate(outer_array);
			ce = cb + outer_array;
			ia_fill(s, v);
			b = pb[0];
			count = s;
            assert(valid());
        }

        /**
         * <your documentation>
         */
        MyDeque (const MyDeque& that) : _a (that._a)
        {
			count = that.size();
            size_type outer_array = (that.size() % SIZE) ? that.size() / SIZE + 1 : that.size() / SIZE;
			allocator_type::rebind<pointer>::other _astar;
			pe = pb = cb = _astar.allocate(outer_array);
			ce = cb + outer_array;
			ia_copy(that.size(), that.begin());
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
            if(cb)
			{
				clear();
				while(b != e)
				{
					_a.deallocate(b, SIZE);
					++b;
				}
				_a.deallocate(b, SIZE);
				allocator_type::rebind<pointer>::other _astar;
				_astar.deallocate(cb, ce - cb);
			}
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
            if (this == &that)
                return *this;

            if (that.size() == size())
			{
                // TODO: Equal Size
			}
            else if (that.size() < size()) 
			{
                // TODO: Less than Size
			}
            else if (that.size() <= capacity()) 
			{
                // TODO: Less than or equal to capacity
			}
            else 
			{
                // TODO: Greater than capacity
			}
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
            if (index < 0 || index >= size())
                throw std::out_of_range("deque::_M_range_check");
            return (*this)[index];
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
            assert(!empty());
            return *(end() - 1);
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
            return iterator(this, 0);
        }

        /**
         * <your documentation>
         */
        const_iterator begin () const 
        {
            return const_iterator(this, 0);
        }

        // -----
        // clear
        // -----

        /**
         * <your documentation>
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
            return iterator(this, count);
        }

        /**
         * <your documentation>
         */
        const_iterator end () const 
        {
            return const_iterator(this, count);
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
            assert(!empty());
            return *begin();
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
			int capacity = (ce - cb) * SIZE;
            if (s == this->size())
			{
                return;
			}
            if (s < this->size())
			{
				size_type offsetBOA = cb - pb;
				size_type offsetBIA = b - *pb;
				size_type outer_array_index = (s + offsetBIA) / SIZE + offsetBOA;
				size_type inner_array_index = (s + offsetBIA) % SIZE;
				pe = cb + outer_array_index;
				e = *pe + inner_array_index;
                destroy(_a, begin() + s, end());
			}
			else if (s <= capacity)
			{
				int add = s - this->size();
				int ia_remain = SIZE - (e - *pe);
				if(ia_remain != 0)
				{
					uninitialized_fill(_a, this->end(), this->end() + ia_remain, v);
					e += ia_remain;
					add -= ia_remain;
				}

				if(add != 0)
				{
					++pe;
					ia_fill(add, v);
				}
			}
            else 
			{
                // TODO: Rebuild Function
                resize(s, v);
			}
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
