
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#ifndef __REGINA_PERMGROUP_H
#ifndef __DOXYGEN
#define __REGINA_PERMGROUP_H
#endif

/*! \file maths/permgroup.h
 *  \brief Implements groups of permutations on \a n objects.
 */

#include "maths/perm.h"

namespace regina {

/**
 * Represents a group of permutations on \a n elements.  This is a subgroup
 * of the symmetric group `S_n`.
 *
 * Groups are stored internally using Sims tables (see Knuth volume 4A for
 * a description of how these work).  This means that, even though such a group
 * could have size factorial in \a n, the storage space required is only
 * quadratic in \a n.
 *
 * PermGroup objects are, in their current implementation, entirely
 * stack-based.  This means they cannot support fast move or swap operations.
 * However, since their size is quadratic in \a n, copy operations involve
 * significantly more overhead than (for example) just copying a Perm object
 * (which just holds a single machine-native integer).  This decision is a
 * deliberate trade-off between speed versus space; the implication for
 * end users is that you should be economical about copying PermGroup objects,
 * and work with them in-place where possible.
 *
 * \python Python does not support templates.  For each
 * \a n = 2,...,16, this class is available in Python under the
 * corresponding name PermGroup2, PermGroup3, ..., PermGroup16.
 *
 * \tparam n the number of objects being permuted.
 * This must be between 2 and 16 inclusive.
 *
 * \ingroup maths
 */
template <int n>
class PermGroup {
    public:
        /**
         * The iterator type for this group.
         *
         * Unlike most iterator types, the dereference operator for this
         * iterator type returns by value (not by reference).  This is because
         * the individual permutations in a group are generated (not stored),
         * based upon an internal group representation that is typically
         * _much_ smaller than the group itself.
         *
         * Both \a iterator and \a const_iterator are the same type, since
         * a PermGroup only offers read-only access to its group members.
         */
        class iterator {
            public:
                using value_type = Perm<n>;
                    /**< Indicates what type the iterator points to. */
                using iterator_category = std::input_iterator_tag;
                    /**< Declares this to be an input iterator type. */
                using difference_type = typename Perm<n>::Index;
                    /**< The type obtained by subtracting iterators. */
                using pointer = const value_type*;
                    /**< A pointer to \a value_type. */
                using reference = Perm<n>;
                    /**< The type returned by the dereference operator,
                         which for this iterator type is by value.
                         See the iterator class notes for details. */
            public:
                /**
                 * Creates a new uninitialised iterator.
                 *
                 * \nopython The only way to create an iterator over a
                 * PermGroup \a g is to iterate over \a g (i.e., to call
                 * `g.__iter__()`).
                 */
                iterator();
                /**
                 * Creates a copy of the given iterator.
                 *
                 * \nopython The only way to create an iterator over a
                 * PermGroup \a g is to iterate over \a g (i.e., to call
                 * `g.__iter__()`).
                 */
                iterator(const iterator&);

                /**
                 * Makes this a copy of the given iterator.
                 *
                 * \return a reference to this iterator.
                 */
                iterator& operator = (const iterator&);

                /**
                 * Compares this with the given iterator for equality.
                 *
                 * \param rhs the iterator to compare this with.
                 * \return \c true if the iterators point to the same
                 * permutation, or \c false if they do not.
                 */
                bool operator == (const iterator& rhs) const;
                /**
                 * Compares this with the given iterator for inequality.
                 *
                 * \param rhs the iterator to compare this with.
                 * \return \c false if the iterators point to the same
                 * permutation, or \c true if they do not.
                 */
                bool operator != (const iterator& rhs) const;

                /**
                 * The preincrement operator.
                 *
                 * \nopython For Python users, this class implements the Python
                 * iterator interface instead.  See __next__() for details.
                 *
                 * \return a reference to this iterator after the increment.
                 */
                iterator& operator ++ ();
                /**
                 * The postincrement operator.
                 *
                 * \nopython For Python users, this class implements the Python
                 * iterator interface instead.  See __next__() for details.
                 *
                 * \return a copy of this iterator before the increment took
                 * place.
                 */
                iterator operator ++ (int);
#ifdef __APIDOCS
                /**
                 * Returns the current permutation and increments this iterator.
                 *
                 * \nocpp For C++ users, ChildIterator provides the usual
                 * iterator preincrement, postincrement and dereferencing
                 * operators (++ and *) instead.
                 *
                 * \exception StopIteration The iterator is already past-the-end
                 * when this function is called.
                 *
                 * \return the permutation that this iterator is pointing to,
                 * before the increment takes place.
                 */
                Perm<n> __next__();
#endif

                /**
                 * Returns the permutation that this iterator is currently
                 * pointing to.
                 *
                 * \pre This iterator is dereferenceable (in particular,
                 * it is not past-the-end).
                 *
                 * \nopython For Python users, this class implements the Python
                 * iterator interface instead.  See __next__() for details.
                 *
                 * \return the corresponding permutation.
                 */
                Perm<n> operator * () const;

                /**
                 * Identifies whether this iterator is dereferenceable.
                 *
                 * \nopython For Python users, this class implements the Python
                 * iterator interface instead.  See __next__() for details.
                 *
                 * \return \c true if and only if this is dereferenceable
                 * (i.e., not past-the-end).
                 */
                operator bool() const;

                friend class PermGroup;
        };

        /**
         * The iterator type for this group.
         *
         * Both \a iterator and \a const_iterator are the same type, since
         * a PermGroup only offers read-only access to its group members.
         * See the PermGroup::iterator class for further details.
         */
        using const_iterator = iterator;

    public:
        /**
         * Constructs the trivial group, containing only the identity
         * permutation.
         */
        PermGroup();
        /**
         * Constructs the symmetric group `S_k`, formed from all
         * permutations of 1,...,\a k.  The elements (\a k + 1),...,\a n
         * will remain fixed under all permutations in this group.
         *
         * The size of this group will be `k!`.
         *
         * \param k indicates how many elements should be permuted; this
         * must be between 0 and \a n inclusive.
         */
        PermGroup(int k);
        /**
         * Creates a new copy of the given group.
         *
         * \param src the group to copy.
         */
        PermGroup(const PermGroup& src);
        /**
         * Generates the subgroup of all elements in the given group
         * that pass the given membership test.
         *
         * Specifically, this generates the subgroup of all permutations \a p
         * in \a parent for which `test(p, args...)` returns \c true.
         *
         * The argument \a test should be a function or some other callable
         * object.  It must return a boolean, and its first argument should
         * be a permutation (either by value as type `Perm<n>`, or by
         * const reference as type `const Perm<n>&`).  If there are
         * any additional arguments supplied in the list \a args, these
         * will be forwarded through as additional arguments to \a test.
         *
         * Note that \a test will not necessarily be called for _all_
         * permutations in \a parent, since this routine will deduce
         * some subgroup members using the standard subgroup properties
         * (e.g., closure and inverse).  It is, however, guaranteed that
         * the _only_ permutations passed to \a test will be permutations
         * that are already known to belong to \a parent.
         *
         * If you wish to modify a group directly, you might wish to call
         * restrict() instead.  Calling `p.restrict(test, args...)` is
         * functionally identical to calling `p = PermGroup(p, test, args...)`,
         * but the in-place conversion allows for a slightly more streamlined
         * implementation.
         *
         * \pre The given membership test does actually define a subgroup
         * (that is, it behaves appropriately with respect to identity,
         * inverse and closure).
         *
         * \param parent the "starting" group of all permutations under
         * consideration.
         * \param test a function (or other callable object) that determines
         * which permutations in \a parent become members of this subgroup.
         * \param any additional arguments that should be passed to \a test,
         * following the initial permutation argument.
         */
        template <typename Test, typename... Args>
        PermGroup(const PermGroup& parent, Test&& test, Args&&... args);

        /**
         * Sets this to be a copy of the given group.
         *
         * \param src the group to copy.
         * \return a reference to this group.
         */
        PermGroup& operator = (const PermGroup& src);

        /**
         * Returns the total number of elements in this group.
         */
        typename Perm<n>::Index size() const;

        /**
         * Determines whether the given permutation belongs to this group.
         *
         * Regardless of the size of this group, the running time for
         * this routine is small polynomial in \a n.
         *
         * \param p the permutation whose membership we wish to test.
         * \return \c true if and only if \a p belongs to this group.
         */
        bool contains(Perm<n> p) const;

        /**
         * Indicates whether this and the given group are identical.
         *
         * This does _not_ test group isomorphism, and it does _not_
         * test whether the two groups use the same internal representation.
         * Instead it tests _membership_; that is, whether or not the two
         * groups contain precisely the same set of permutations.
         *
         * As a result, this test is not trivial.  It _is_ small polynomial
         * time in \a n, but it is not as fast as (for example) directly
         * comparing the internal representations.
         *
         * \other the group to compare this with.
         * \return \c true if and only if this and the given group contain
         * the same permutations.
         */
        bool operator == (const PermGroup& other) const;
        /**
         * Indicates whether this and the given group are different.
         *
         * This does _not_ test group isomorphism, and it does _not_
         * test whether the two groups use the same internal representation.
         * Instead it tests _membership_; that is, whether or not the two
         * groups contain precisely the same set of permutations.
         *
         * As a result, this test is not trivial.  It _is_ small polynomial
         * time in \a n, but it is not as fast as (for example) directly
         * comparing the internal representations.
         *
         * \other the group to compare this with.
         * \return \c true if and only if there is some permutation that
         * belongs to one group but not the other.
         */
        bool operator != (const PermGroup& other) const;

        /**
         * Returns a C++ iterator pointing to the first element of this group.
         *
         * The iterator range from begin() to end() runs through all
         * permutations in this group.  The order of iteration is arbitrary,
         * and may change in future releases of Regina.
         *
         * \nopython For Python users, PermGroup implements the Python iterable
         * interface.  You can iterate over the elements of this group in the
         * same way that you would iterate over any native Python container.
         *
         * \return an iterator pointing to the first element of this group.
         */
        auto begin() const;
        /**
         * Returns a C++ iterator beyond the last element of this group.
         *
         * The iterator range from begin() to end() runs through all
         * permutations in this group.  The order of iteration is arbitrary,
         * and may change in future releases of Regina.
         *
         * \nopython For Python users, PermGroup implements the Python iterable
         * interface.  You can iterate over the elements of this group in the
         * same way that you would iterate over any native Python container.
         *
         * \return an iterator beyond the last element of this group.
         */
        auto begin() const;
#ifdef __APIDOCS
        /**
         * Returns a Python iterator over the elements of this group.
         *
         * The order of iteration is arbitrary, and may change in future
         * releases of Regina.
         *
         * \nocpp For C++ users, PermGroup provides the usual begin() and end()
         * functions instead.  In particular, you can iterate over the elements
         * of this group in the usual way using a range-based \c for loop.
         *
         * \return an iterator over the elements of this group.
         */
        auto __iter__() const;
#endif

        /**
         * Converts this into the subgroup of all elements within this
         * group that pass the given membership test.
         *
         * Specifically, this generates the subgroup of all permutations \a p
         * in this group for which `test(p, args...)` returns \c true.
         *
         * Calling `p.restrict(test, args...)` is functionally identical
         * to calling `p = PermGroup(p, test, args...)`.  The reason for
         * offering restrict() as a separate function is that the in-place
         * conversion allows restrict() to be a little more streamlined.
         *
         * The argument \a test should be a function or some other callable
         * object.  It must return a boolean, and its first argument should
         * be a permutation (either by value as type `Perm<n>`, or by
         * const reference as type `const Perm<n>&`).  If there are
         * any additional arguments supplied in the list \a args, these
         * will be forwarded through as additional arguments to \a test.
         *
         * Note that \a test will not necessarily be called for _all_
         * permutations in this group, since this routine will deduce
         * some subgroup members using the standard subgroup properties
         * (e.g., closure and inverse).  It is, however, guaranteed that
         * the _only_ permutations passed to \a test will be permutations
         * that were originally part of this group.
         *
         * \pre The given membership test does actually define a subgroup
         * (that is, it behaves appropriately with respect to identity,
         * inverse and closure).
         *
         * \param test a function (or other callable object) that determines
         * which permutations in this group will be kept.
         * \param any additional arguments that should be passed to \a test,
         * following the initial permutation argument.
         */
        template <typename Test, typename... Args>
        void restrict(Test&& test, Args&&... args);
};

} // namespace regina

#endif
