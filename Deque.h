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

		///
		/// <your documentation>
		///
		void ia_fill (int add, const_reference v, pointer* x)
		{
			for(; add > 0; add -= SIZE)
			{
				size_type inner_array = (add >= SIZE) ? SIZE : add;
				e = uninitialized_fill(_a, *x, *x + inner_array, v);
				++x;
			}
		}

		///
		/// <your documentation>
		///
		template<typename I>
		void ia_copy (int add, I bIter, pointer* x)
		{
			I eIter = bIter;
			for(; add > 0; add -= SIZE)
			{
				size_type inner_array = (add >= SIZE) ? SIZE : add;
				eIter += inner_array;
				e = uninitialized_copy(_a, bIter, eIter, *x);
				bIter += inner_array;
				++x;
			}
		}
		
		///
		/// <your documentation>
		///
		void allocate(pointer* _b)
		{
			while(_b != ce)
			{
				*_b = _a.allocate(SIZE);
				++_b;
			}
			b = pb[0] + SIZE - 1;
			e = pe[0];
		}

		///
		/// <your documentation>
		///
		void rebuild(size_type s)
		{
			MyDeque x(*this, 2 * s);
            this->swap(x);
		}

		///
		/// <your documentation>
		///
		MyDeque (const MyDeque& that, size_type s) : _a (that._a) 
		{
            assert(s >= that.size());
            size_type outer_array = (s % SIZE) ? s / SIZE + 1 : s / SIZE;
			size_type copy_array = (that.size() % SIZE) ? that.size() / SIZE + 1 : that.size() / SIZE;
			(outer_array % 2) ? outer_array : outer_array += 1;
			allocator_type::rebind<pointer>::other _astar;
			cb = _astar.allocate(outer_array);
			pb = cb + outer_array / 2;
			pe = pb + copy_array;
			ce = cb + outer_array;
			allocate(cb);
			ia_copy(that.size(), that.begin(), pb);
			count = that.size();
            assert(valid());
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
			pb = cb = _astar.allocate(outer_array);
			pe = pb + outer_array;
			ce = cb + outer_array;
			allocate(cb);
			ia_fill(s, v);
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
			pb = cb = _astar.allocate(outer_array);
			pe = pb + outer_array;
			ce = cb + outer_array;
			allocate(cb);
			ia_copy(that.size(), that.begin(), pb);
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
				pointer* x = cb;
				while(x != ce)
				{
					_a.deallocate(*x, SIZE);
					++x;
				}
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
        MyDeque& operator = (const MyDeque& that) 
        {
            if (this == &that)
                return *this;

			// capacity = the number of elements from the allocated end to max end
			int capacity = (ce - pe) * SIZE + (e - *pe);
            if (that.size() == this->size())
			{
                // Equal Size
				std::copy(that.begin(), that.end(), this->begin());
			}
            else if (that.size() < this->size()) 
			{
                // Less than Size
				std::copy(that.begin(), that.end(), this->begin());
				count = that.size();
			}
            else if (that.size() <= capacity) 
			{
                // Less than or equal to capacity
				std::copy(that.begin(), that.begin() + size(), this->begin());
                uninitialized_copy(_a, that.begin() + that.size(), that.end(), this->end());
				count = that.size();
			}
            else 
			{
                // Greater than capacity
				clear();
				rebuild(that.size());
				uninitialized_copy(_a, that.begin(), that.end(), this->begin());
				count = that.size();
			}

			// Regenerate pe and e pointers
			size_type offsetBOA = pb - cb;
            size_type offsetBIA = b - *pb;
			size_type outer_array_index = (this->size() - 1 + offsetBIA) / SIZE + offsetBOA;
			size_type inner_array_index = (this->size() - 1 + offsetBIA) % SIZE;
			pe = cb + outer_array_index;
			e = *pe + inner_array_index;

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
			size_type offsetBOA = pb - cb;
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
        iterator insert (iterator iter, const_reference v) 
        {
            // capacity = the number of elements from the allocated end to max end
			int capacity = (ce - pe) * SIZE + (e - *pe);
			if(capacity < 1)
				rebuild(this->size() + 1);
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
         * <your documentation>
         */
        void pop_back () 
        {
            assert(!empty());
            resize(size() - 1);
            assert(valid());
        }

        /**
         * <your documentation>
         */
        void pop_front () 
        {
            destroy(_a, this->begin(), this->begin() + 1);
			++b;
			if(b - *pb == SIZE)
			{
				--pb;
				b = *pb;
			}
			--count;
            assert(valid());
        }

        // ----
        // push
        // ----

        /**
         * <your documentation>
         */
        void push_back (const_reference v) 
        {
			resize(this->size() + 1, v);
            assert(valid());
        }

        /**
         * <your documentation>
         */
        void push_front (const_reference v) 
        {
			// Check capacity
			if(cb == nullptr || *cb == b)
				rebuild(this->size() + 1);

			int ia_remain = SIZE - (b - *pb);
			if(ia_remain == 0)
			{
				++pb;
				b = *pb + SIZE - 1;
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
				size_type index = (s == 0) ? s : s - 1;
				size_type offsetBOA = pb - cb;
				size_type offsetBIA = b - *pb;
				size_type outer_array_index = (index + offsetBIA) / SIZE + offsetBOA;
				size_type inner_array_index = (index + offsetBIA) % SIZE;
				pe = cb + outer_array_index;
				e = *pe + inner_array_index;
                destroy(_a, this->begin() + s, this->end());
				count = s;
			}
			else if (s <= capacity)
			{
				int add = s - this->size();
				int ia_remain = SIZE - (e - *pe);
				if(ia_remain != 0)
				{
					int fillsize = std::min(add, ia_remain);
					uninitialized_fill(_a, e, e + fillsize, v);
					e += fillsize;
					add -= fillsize;
				}

				if(add != 0)
				{
					++pe;
					ia_fill(add, v, pe);
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
