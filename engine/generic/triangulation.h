
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file generic/triangulation.h
 *  \brief Deals with triangulations of arbitrary dimension.
 */

#ifndef __TRIANGULATION_H
#ifndef __DOXYGEN
#define __TRIANGULATION_H
#endif

#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "regina-core.h"
#include "output.h"
#include "generic/simplex.h"
#include "maths/nperm.h"
#include "utilities/nmarkedvector.h"

namespace regina {

template <int> class Isomorphism;
template <int> class Triangulation;

/**
 * \weakgroup generic
 * @{
 */

/**
 * Provides core functionality for <i>dim</i>-dimensional triangulations.
 *
 * Such a triangulation is represented by the class Triangulation<dim>,
 * which uses this as a base class.  End users should not need to refer
 * to TriangulationBase directly.
 *
 * See the Triangulation class notes for further information.
 *
 * \ifacespython This base class is not present, but the "end user" class
 * Triangulation<dim> is.
 *
 * \tparam dim the dimension of the triangulation.  This must be at least 2.
 */
template <int dim>
class REGINA_API TriangulationBase :
        public Output<TriangulationBase<dim>>,
        public boost::noncopyable {
    static_assert(dim >= 2, "Triangulation requires dimension >= 2.");

    public:
        typedef typename std::vector<Simplex<dim>*>::const_iterator
                SimplexIterator;
            /**< Used to iterate through top-dimensional simplices. */

    protected:
        NMarkedVector<Simplex<dim>> simplices_;
            /**< The top-dimensional simplices that form the triangulation. */

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Default constructor.
         *
         * Creates an empty triangulation.
         */
        TriangulationBase();
        /**
         * Creates a new copy of the given triangulation.
         *
         * @param copy the triangulation to copy.
         */
        TriangulationBase(const TriangulationBase& copy);
        /**
         * Destroys this triangulation.
         *
         * The simplices within this triangulation will also be destroyed.
         */
        ~TriangulationBase();

        /*@}*/
        /**
         * \name Simplices
         */
        /*@{*/

        /**
         * Returns the number of top-dimensional simplices in the
         * triangulation.
         *
         * @return The number of top-dimensional simplices.
         */
        size_t size() const;
        /**
         * Deprecated routine that returns the number of top-dimensional
         * simplices in the triangulation.
         *
         * \deprecated Simply call size() instead.
         *
         * @return the number of top-dimensional simplices.
         */
        size_t getNumberOfSimplices() const;
        /**
         * Returns all top-dimensional simplices in the triangulation.
         *
         * The reference that is returned will remain valid for as long as
         * the triangulation exists: even as simplices are added and/or
         * removed, it will always reflect the simplices that are currently
         * in the triangulation.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all top-dimensional simplices.
         */
        const std::vector<Simplex<dim>*>& simplices() const;
        /**
         * Deprecated routine that returns all of the top-dimensional
         * simplices in the triangulation.
         *
         * \deprecated Simply call simplices() instead.
         *
         * See simplices() for further details.
         */
        const std::vector<Simplex<dim>*>& getSimplices() const;
        /**
         * Returns the top-dimensional simplex at the given index in the
         * triangulation.
         *
         * Note that indexing may change when a simplex is added to or
         * removed from the triangulation.
         *
         * @param index specifies which simplex to return; this
         * value should be between 0 and size()-1 inclusive.
         * @return the <i>index</i>th top-dimensional simplex.
         */
        Simplex<dim>* simplex(size_t index);
        /**
         * Deprecated routine that returns the top-dimensional simplex at
         * the given index in the triangulation.
         *
         * \deprecated Simply call simplex() instead.
         *
         * See simplex() for further details.
         */
        Simplex<dim>* getSimplex(size_t index);
        /**
         * Returns the top-dimensional simplex at the given index in the
         * triangulation.
         *
         * Note that indexing may change when a simplex is added to or
         * removed from the triangulation.
         *
         * @param index specifies which simplex to return; this
         * value should be between 0 and size()-1 inclusive.
         * @return the <i>index</i>th top-dimensional simplex.
         */
        const Simplex<dim>* simplex(size_t index) const;
        /**
         * Deprecated routine that returns the top-dimensional simplex at
         * the given index in the triangulation.
         *
         * \deprecated Simply call simplex() instead.
         *
         * See simplex() for further details.
         */
        const Simplex<dim>* getSimplex(size_t index) const;
        /**
         * Returns the index of the given top-dimensional simplex in the
         * triangulation.
         *
         * Note that indexing may change when a simplex is added to or
         * removed from the triangulation.
         *
         * \pre The given simplex is contained in this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.  If you are passing the result of some other
         * routine that \e might return null (such as
         * Simplex<dim>::adjacentSimplex), you should explicitly
         * test for null beforehand.
         *
         * @param simplex specifies which simplex to find in the triangulation.
         * @return the index of the specified simplex; this will be an
         * integer between 0 and size()-1 inclusive.
         */
        size_t simplexIndex(const Simplex<dim>* simplex) const;
        /**
         * Creates a new top-dimensional simplex and adds it to this
         * triangulation.
         *
         * The new simplex will have an empty description.
         * All (<i>dim</i>+1) facets of the new simplex will be boundary facets.
         *
         * The new simplex will become the last simplex in this
         * triangulation; that is, it will have index size()-1.
         *
         * @return the new simplex.
         */
        Simplex<dim>* newSimplex();
        /**
         * Creates a new top-dimensional simplex with the given
         * description and adds it to this triangulation.
         *
         * All (<i>dim</i>+1) facets of the new simplex will be boundary facets.
         *
         * Descriptions are optional, may have any format, and may be empty.
         * How descriptions are used is entirely up to the user.
         *
         * The new simplex will become the last simplex in this
         * triangulation; that is, it will have index size()-1.
         *
         * @param desc the description to give to the new simplex.
         * @return the new simplex.
         */
        Simplex<dim>* newSimplex(const std::string& desc);
        /**
         * Removes the given top-dimensional simplex from this triangulation.
         *
         * The given simplex will be unglued from any adjacent simplices
         * (if any), and will be destroyed immediately.
         *
         * \pre The given simplex is a top-dimensional simplex in this
         * triangulation.
         *
         * @param simplex the simplex to remove.
         */
        void removeSimplex(Simplex<dim>* simplex);
        /**
         * Removes the top-dimensional simplex at the given index in
         * this triangulation.
         *
         * This is equivalent to calling <tt>removeSimplex(simplex(index))</tt>.
         *
         * The given simplex will be unglued from any adjacent simplices
         * (if any), and will be destroyed immediately.
         *
         * @param index specifies which top-dimensionalsimplex to remove; this
         * must be between 0 and size()-1 inclusive.
         */
        void removeSimplexAt(size_t index);
        /**
         * Removes all simplices from the triangulation.
         * As a result, this triangulation will become empty.
         *
         * All of the simplices that belong to this triangulation will
         * be destroyed immediately.
         */
        void removeAllSimplices();
        /**
         * Swaps the contents of this and the given triangulation.
         *
         * All top-dimensional simplices that belong to this triangulation
         * will be moved to \a other, and all top-dimensional simplices
         * that belong to \a other will be moved to this triangulation.
         *
         * Any pointers or references to Simplex<dim> objects will remain valid.
         *
         * @param other the triangulation whose contents should be
         * swapped with this.
         */
        void swapContents(Triangulation<dim>& other);
        /**
         * Moves the contents of this triangulation into the given
         * destination triangulation, without destroying any pre-existing
         * contents.
         *
         * All top-dimensional simplices that currently belong to \a dest
         * will remain there (and will keep the same indices in \a dest).
         * All top-dimensional simplices that belong to this triangulation
         * will be moved into \a dest also (but in general their indices will
         * change).
         *
         * This triangulation will become empty as a result.
         *
         * Any pointers or references to Simplex<dim> objects will remain valid.
         *
         * @param dest the triangulation into which simplices should be moved.
         */
        void moveContentsTo(Triangulation<dim>& dest);

        /*@}*/
        /**
         * \name Basic Properties
         */
        /*@{*/

        /**
         * Determines whether this triangulation is empty.
         * An empty triangulation is one with no simplices at all.
         *
         * @return \c true if and only if this triangulation is empty.
         */
        bool isEmpty() const;

        /**
         * Determines if this triangulation has any boundary facets.
         *
         * This routine returns \c true if and only if the
         * triangulation contains some top-dimension simplex with at
         * least one facet that is not glued to an adjacent simplex.
         *
         * @return \c true if and only if there are boundary facets.
         */
        bool hasBoundaryFacets() const;

        /*@}*/
        /**
         * \name Isomorphism Testing
         */
        /*@{*/

        /**
         * Determines if this triangulation is combinatorially identical
         * to the given triangulation.
         *
         * Here "identical" means that the triangulations have the same
         * number of top-dimensional simplices, with gluings between the same
         * pairs of numbered simplices using the same gluing permutations.
         * In other words, "identical" means that the triangulations
         * are isomorphic via the identity isomorphism.
         *
         * For the less strict notion of \e isomorphic triangulations,
         * which allows relabelling of the top-dimensional simplices and their
         * vertices, see isIsomorphicTo() instead.
         *
         * This test does \e not examine the textual simplex descriptions,
         * as seen in Simplex<dim>::getDescription(); these may still differ.
         * It also does not test whether lower-dimensional faces are
         * numbered identically (vertices, edges and so on); this routine
         * is only concerned with top-dimensional simplices.
         *
         * (At the time of writing, two identical triangulations will
         * always number their lower-dimensional faces in the same way.
         * However, it is conceivable that in future versions of Regina there
         * may be situations in which identical triangulations can acquire
         * different numberings for vertices, edges, and so on.)
         *
         * @param other the triangulation to compare with this one.
         * @return \c true if and only if the two triangulations are
         * combinatorially identical.
         */
        bool isIdenticalTo(const Triangulation<dim>& other) const;
#if 0
        /**
         * Determines if this triangulation is combinatorially
         * isomorphic to the given triangulation.
         *
         * Two triangulations are \e isomorphic if and only it is
         * possible to relabel their top-dimensional simplices and the
         * (<i>dim</i>+1) vertices of each simplex in a way that makes
         * the two triangulations combinatorially identical, as returned
         * by isIdenticalTo().
         *
         * Equivalently, two triangulations are isomorphic if and only if
         * there is a one-to-one and onto boundary complete combinatorial
         * isomorphism from this triangulation to \a other, as described
         * in the Isomorphism class notes.
         *
         * In particular, note that this triangulation and \a other must
         * contain the same number of top-dimensional simplices for such an
         * isomorphism to exist.
         *
         * If the triangulations are isomorphic, then this routine returns
         * one such boundary complete isomorphism (i.e., one such relabelling).
         * The isomorphism will be newly constructed, and to assist with
         * memory management, it will be returned as a std::auto_ptr.
         * Thus, to test whether an isomorphism exists without having to
         * explicitly manage with the isomorphism itself, you can just call
         * <tt>if (isIsomorphicTo(other).get())</tt>, in which case the newly
         * created isomorphism (if it exists) will be automatically
         * destroyed.
         *
         * There may be many such isomorphisms between the two triangulations.
         * If you need to find \e all such isomorphisms, you may call
         * findAllIsomorphisms() instead.
         *
         * If you need to ensure that top-dimensional simplices are labelled
         * the same in both triangulations (i.e., that the triangulations are
         * related by the \e identity isomorphism), you should call the
         * stricter test isIdenticalTo() instead.
         *
         * \todo \opt Improve the complexity by choosing a simplex
         * mapping from each component and following gluings to
         * determine the others.
         *
         * @param other the triangulation to compare with this one.
         * @return details of the isomorphism if the two triangulations
         * are combinatorially isomorphic, or a null pointer otherwise.
         */
        std::auto_ptr<Isomorphism<dim>> isIsomorphicTo(
            const Triangulation& other) const;
// TODO
        /**
         * Determines if an isomorphic copy of this triangulation is
         * contained within the given triangulation, possibly as a
         * subcomplex of some larger component (or components).
         *
         * Specifically, this routine determines if there is a boundary
         * incomplete combinatorial isomorphism from this triangulation
         * to \a other.  Boundary incomplete isomorphisms are described
         * in detail in the Isomorphism class notes.
         *
         * In particular, note that facets of top-dimensional simplices that
         * lie on the boundary of this triangulation need not correspond to
         * boundary facets of \a other, and that \a other may contain more
         * top-dimensional simplices than this triangulation.
         *
         * If a boundary incomplete isomorphism is found, the details of
         * this isomorphism are returned.  The isomorphism is newly
         * constructed, and so to assist with memory management is
         * returned as a std::auto_ptr.  Thus, to test whether an
         * isomorphism exists without having to explicitly deal with the
         * isomorphism itself, you can call
         * <tt>if (isContainedIn(other).get())</tt> and the newly
         * created isomorphism (if it exists) will be automatically
         * destroyed.
         *
         * If more than one such isomorphism exists, only one will be
         * returned.  For a routine that returns all such isomorphisms,
         * see findAllSubcomplexesIn().
         *
         * @param other the triangulation in which to search for an
         * isomorphic copy of this triangulation.
         * @return details of the isomorphism if such a copy is found,
         * or a null pointer otherwise.
         */
        std::auto_ptr<Isomorphism<dim>> isContainedIn(
            const Triangulation& other) const;

        /**
         * Finds all ways in which this triangulation is combinatorially
         * isomorphic to the given triangulation.
         *
         * This routine behaves identically to isIsomorphicTo(), except that
         * instead of returning just one isomorphism, all such isomorphisms
         * are returned.
         *
         * See the isIsomorphicTo() notes for additional information.
         *
         * The isomorphisms that are found will be inserted into the
         * given list.  These isomorphisms will be newly created, and
         * the caller of this routine is responsible for destroying
         * them.  The given list will not be emptied before the new
         * isomorphisms are inserted.
         *
         * \ifacespython Not present.
         *
         * @param other the triangulation to compare with this one.
         * @param results the list in which any isomorphisms found will
         * be stored.
         * @return the number of isomorphisms that were found.
         */
        unsigned long findAllIsomorphisms(const Triangulation& other,
            std::list<Isomorphism<dim>*>& results) const;

        /**
         * Finds all ways in which an isomorphic copy of this triangulation
         * is contained within the given triangulation, possibly as a
         * subcomplex of some larger component (or components).
         *
         * This routine behaves identically to isContainedIn(), except
         * that instead of returning just one isomorphism (which may be
         * boundary incomplete and need not be onto), all such isomorphisms
         * are returned.
         *
         * See the isContainedIn() notes for additional information.
         *
         * The isomorphisms that are found will be inserted into the
         * given list.  These isomorphisms will be newly created, and
         * the caller of this routine is responsible for destroying
         * them.  The given list will not be emptied before the new
         * isomorphisms are inserted.
         *
         * \ifacespython Not present.
         *
         * @param other the triangulation in which to search for
         * isomorphic copies of this triangulation.
         * @param results the list in which any isomorphisms found will
         * be stored.
         * @return the number of isomorphisms that were found.
         */
        unsigned long findAllSubcomplexesIn(const Triangulation& other,
            std::list<Isomorphism<dim>*>& results) const;

        /**
         * Relabel the top-dimensional simplices and their vertices so that
         * this triangulation is in canonical form.  This is essentially
         * the lexicographically smallest labelling when the facet
         * gluings are written out in order.
         *
         * Two triangulations are isomorphic if and only if their canonical
         * forms are identical.
         *
         * The lexicographic ordering assumes that the facet gluings are
         * written in order of simplex index and then facet number.
         * Each gluing is written as the destination simplex index
         * followed by the gluing permutation (which in turn is written
         * as the images of 0,1,...,<i>dim</i> in order).
         *
         * \pre This routine currently works only when the triangulation
         * is connected.  It may be extended to work with disconnected
         * triangulations in later versions of Regina.
         *
         * @return \c true if the triangulation was changed, or \c false
         * if the triangulation was in canonical form to begin with.
         */
        bool makeCanonical();

        /*@}*/
        /**
         * \name Building Triangulations
         */
        /*@{*/

        /**
         * Inserts a copy of the given triangulation into this triangulation.
         *
         * The new triangles will be inserted into this triangulation
         * in the order in which they appear in the given triangulation,
         * and the numbering of their vertices (0-2) will not change.
         * They will be given the same descriptions as appear in the
         * given triangulation.
         *
         * @param source the triangulation whose copy will be inserted.
         */
        void insertTriangulation(const Triangulation& source);

        /**
         * Inserts into this triangulation a set of triangles and their
         * gluings as described by the given integer arrays.
         *
         * This routine is provided to make it easy to hard-code a
         * medium-sized triangulation in a C++ source file.  All of the
         * pertinent data can be hard-coded into a pair of integer arrays at
         * the beginning of the source file, avoiding an otherwise tedious
         * sequence of many joinTo() calls.
         *
         * An additional \a nTriangles triangles will be inserted into
         * this triangulation.  The relationships between these triangles
         * should be stored in the two arrays as follows.  Note that the
         * new triangles are numbered from 0 to (\a nTriangles - 1), and
         * individual triangle edges are numbered from 0 to 2.
         *
         * The \a adjacencies array describes which triangle edges are
         * joined to which others.  Specifically, <tt>adjacencies[f][e]</tt>
         * should contain the number of the triangle joined to edge \a e
         * of triangle \a f.  If this edge is to be left as a
         * boundary edge, <tt>adjacencies[f][e]</tt> should be -1.
         *
         * The \a gluings array describes the particular gluing permutations
         * used when joining these triangle edges together.  Specifically,
         * <tt>gluings[f][e][0..2]</tt> should describe the permutation
         * used to join edge \a e of triangle \a f to its adjacent
         * triangle.  These three integers should be 0, 1 and 2 in some
         * order, so that <tt>gluings[f][e][i]</tt> contains the image of
         * \a i under this permutation.  If edge \a e of triangle \a f
         * is to be left as a boundary edge, <tt>gluings[f][e][0..2]</tt>
         * may contain anything (and will be duly ignored).
         *
         * It is the responsibility of the caller of this routine to
         * ensure that the given arrays are correct and consistent.
         * No error checking will be performed by this routine.
         *
         * Note that, for an existing triangulation, dumpConstruction()
         * will output a pair of C++ arrays that can be copied into a
         * source file and used to reconstruct the triangulation via
         * this routine.
         *
         * \ifacespython Not present.
         *
         * @param nSimplices the number of additional simplices to insert.
         * @param adjacencies describes which of the new triangle edges
         * are to be identified.  This array must have initial
         * dimension at least \a nTriangles.
         * @param gluings describes the specific gluing permutations by
         * which these new triangle edges should be identified.  This
         * array must also have initial dimension at least \a nTriangles.
         */
        void insertConstruction(
            size_t nSimplices,
            const int adjacencies[][dim+1],
            const int gluings[][dim+1][dim+1]);

        /*@}*/
        /**
         * \name Exporting Triangulations
         */
        /*@{*/

        /**
         * Constructs the isomorphism signature for this triangulation.
         *
         * An <i>isomorphism signature</i> is a compact text representation of
         * a triangulation.  Unlike dehydrations for 3-manifold triangulations,
         * an isomorphism signature uniquely determines a triangulation up
         * to combinatorial isomorphism (assuming the dimension is known in
         * advance).
         * That is, two triangulations of dimension \a dim are combinatorially
         * isomorphic if and only if their isomorphism signatures are the same.
         *
         * The isomorphism signature is constructed entirely of printable
         * characters, and has length proportional to <tt>n log n</tt>,
         * where \a n is the number of top-dimenisonal simplices.
         *
         * Isomorphism signatures are more general than dehydrations:
         * they can be used with any triangulation (including closed,
         * bounded and/or disconnected triangulations, as well
         * as triangulations with large numbers of triangles).
         *
         * The time required to construct the isomorphism signature of a
         * triangulation is <tt>O(n^2 log^2 n)</tt>.
         *
         * The routine fromIsoSig() can be used to recover a
         * triangulation from an isomorphism signature.  The triangulation
         * recovered might not be identical to the original, but it will be
         * combinatorially isomorphic.
         *
         * If \a relabelling is non-null (i.e., it points to some
         * Isomorphism pointer \a p), then it will be modified to point
         * to a new isomorphism that describes the precise relationship
         * between this triangulation and the reconstruction from fromIsoSig().
         * Specifically, the triangulation that is reconstructed from
         * fromIsoSig() will be combinatorially identical to
         * <tt>relabelling.apply(this)</tt>.
         *
         * For a full and precise description of the isomorphism signature
         * format for 3-manifold triangulations, see <i>Simplification paths
         * in the Pachner graphs of closed orientable 3-manifold
         * triangulations</i>, Burton, 2011, <tt>arXiv:1110.6080</tt>.
         * The format for other dimensions is essentially the same, but with
         * minor dimension-specific adjustments.
         *
         * \ifacespython The isomorphism argument is not present.
         * Instead there are two routines: fromIsoSig(), which returns a
         * string only, and fromIsoSigDetail(), which returns a pair
         * (signature, relabelling).
         *
         * \pre If \a relabelling is non-null, then this triangulation
         * must be non-empty and connected.  The facility to return a
         * relabelling for disconnected triangulations may be added to
         * Regina in a later release.
         *
         * \warning Do not mix isomorphism signatures between dimensions!
         * It is possible that the same string could corresponding to both a
         * \a p-dimensional triangulation and a \a q-dimensional triangulation
         * for different \a p and \a q.
         *
         * @param relabelling if non-null, this will be modified to point to a
         * new isomorphism describing the relationship between this
         * triangulation and that reconstructed from fromIsoSig(), as
         * described above.
         * @return the isomorphism signature of this triangulation.
         */
        std::string isoSig(Isomorphism<dim>** relabelling = 0) const;

        /**
         * Returns C++ code that can be used with insertConstruction()
         * to reconstruct this triangulation.
         *
         * The code produced will consist of the following:
         *
         * - the declaration and initialisation of two integer arrays,
         *   describing the triangle gluings in this trianguation;
         * - two additional lines that declare a new Dim2Triangulation and
         *   call insertConstruction() to rebuild this triangulation.
         *
         * The main purpose of this routine is to generate the two integer
         * arrays, which can be tedious and error-prone to code up by hand.
         *
         * Note that the number of lines of code produced grows linearly
         * with the number of triangles.  If this triangulation is very
         * large, the returned string will be very large as well.
         *
         * @return the C++ code that was generated.
         */
        std::string dumpConstruction() const;

        /*@}*/
        /**
         * \name Importing Triangulations
         */
        /*@{*/

        /**
         * Recovers a full triangulation from an isomorphism signature.
         *
         * See isoSig() for more information on isomorphism signatures.
         * It will be assumed that the signature describes a triangulation of
         * dimension \a dim.
         *
         * The triangulation that is returned will be newly created.
         *
         * Calling isoSig() followed by fromIsoSig() is not guaranteed to
         * produce an identical triangulation to the original, but it
         * \e is guaranteed to produce a combinatorially isomorphic
         * triangulation.
         *
         * For a full and precise description of the isomorphism signature
         * format for 3-manifold triangulations, see <i>Simplification paths
         * in the Pachner graphs of closed orientable 3-manifold
         * triangulations</i>, Burton, 2011, <tt>arXiv:1110.6080</tt>.
         * The format for other dimensions is essentially the same, but with
         * minor dimension-specific adjustments.
         *
         * \warning Do not mix isomorphism signatures between dimensions!
         * It is possible that the same string could corresponding to both a
         * \a p-dimensional triangulation and a \a q-dimensional triangulation
         * for different \a p and \a q.
         *
         * @param sig the isomorphism signature of the
         * triangulation to construct.  Note that, unlike dehydration
         * strings for 3-manifold triangulations, case is important for
         * isomorphism signatures.
         * @return a newly allocated triangulation if the reconstruction was
         * successful, or null if the given string was not a valid
         * isomorphism signature.
         */
        static Triangulation* fromIsoSig(const std::string& sig);

        /**
         * Deduces the number of top-dimensional simplices in a
         * connected triangulation from its isomorphism signature.
         *
         * See isoSig() for more information on isomorphism signatures.
         * It will be assumed that the signature describes a triangulation of
         * dimension \a dim.
         *
         * If the signature describes a connected triangulation, this
         * routine will simply return the size of that triangulation
         * (e.g., the number of tetrahedra in the case \a dim = 3).
         * You can also pass an isomorphism signature that describes a
         * disconnected triangulation; however, this routine will only
         * return the number of simplices in the first connected component.
         * If you need the total number of simplices in a disconnected
         * triangulation, you will need to reconstruct the full triangulation
         * by calling fromIsoSig() instead.
         *
         * This routine is very fast, since it only examines the first
         * few characters of the isomorphism signature (in which the size
         * of the first component is encoded).  However, it is therefore
         * possible to pass an invalid isomorphism signature and still
         * receive a positive result.  If you need to \e test whether a
         * signature is valid or not, you must call fromIsoSig()
         * instead, which will examine the entire signature in full.
         *
         * \warning Do not mix isomorphism signatures between dimensions!
         * It is possible that the same string could corresponding to both a
         * \a p-dimensional triangulation and a \a q-dimensional triangulation
         * for different \a p and \a q.
         *
         * @param sig an isomorphism signature of a \a dim-dimensional
         * triangulation.  Note that, unlike dehydration strings for
         * 3-manifold triangulations, case is important for isomorphism
         * signatures.
         * @return the number of top-dimensional simplices in the first
         * connected component, or 0 if this could not be determined
         * because the given string was not a valid isomorphism signature.
         */
        static size_t isoSigComponentSize(const std::string& sig);
#endif
        /*@}*/
        /**
         * \name Output
         */
        /*@{*/

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        /*@}*/

    private:
#if 0
        /**
         * Determines if an isomorphic copy of this triangulation is
         * contained within the given triangulation.
         *
         * If the argument \a completeIsomorphism is \c true, the
         * isomorphism must be onto and boundary complete.
         * That is, this triangulation must be combinatorially
         * isomorphic to the given triangulation.
         *
         * If the argument \a completeIsomorphism is \c false, the
         * isomorphism may be boundary incomplete and may or may not be
         * onto.  That is, this triangulation must appear as a
         * subcomplex of the given triangulation, possibly with some
         * original boundary edges joined to new triangles.
         *
         * See the Dim2Isomorphism class notes for further details
         * regarding boundary complete and boundary incomplete
         * isomorphisms.
         *
         * The isomorphisms found, if any, will be appended to the
         * list \a results.  This list will not be emptied before
         * calculations begin.  All isomorphisms will be newly created,
         * and the caller of this routine is responsible for destroying
         * them.
         *
         * If \a firstOnly is passed as \c true, only the first
         * isomorphism found (if any) will be returned, after which the
         * routine will return immediately.  Otherwise all isomorphisms
         * will be returned.
         *
         * @param other the triangulation in which to search for an
         * isomorphic copy of this triangulation.
         * @param results the list in which any isomorphisms found will
         * be stored.
         * @param completeIsomorphism \c true if isomorphisms must be
         * onto and boundary complete, or \c false if neither of these
         * restrictions should be imposed.
         * @param firstOnly \c true if only one isomorphism should be
         * returned (if any), or \c false if all isomorphisms should be
         * returned.
         * @return the total number of isomorphisms found.
         */
        unsigned long findIsomorphisms(const Triangulation& other,
                std::list<Isomorphism<dim>*>& results,
                bool completeIsomorphism, bool firstOnly) const;

        /**
         * Internal to isoSig().
         *
         * Constructs a candidate isomorphism signature for a single
         * component of this triangulation.  This candidate signature
         * assumes that the given simplex with the given labelling
         * of its vertices becomes simplex zero with vertices 0..dim
         * under the "canonical isomorphism".
         *
         * @param tri the triangulation under consideration.
         * @param simp the index of some simplex in this triangulation.
         * @param vertices some ordering of the vertices of the
         * given tetrahedron.
         * @param relabelling if this is non-null, it will be filled with the
         * canonical isomorphism; in this case it must already have been
         * constructed for the correct number of simplices.
         * @return the candidate isomorphism signature.
         */
        static std::string isoSigFrom(const Triangulation& tri, unsigned simp,
            const NPerm<dim+1>& vertices, Isomorphism<dim>* relabelling);
#endif
};

/**
 * A <i>dim</i>-dimensional triangulation, built by gluing
 * <i>dim</i>-dimensional simplices along their (<i>dim</i>-1)-dimensional
 * facets.  Typically (but not necessarily) such triangulations are used
 * to represent <i>dim</i>-manifolds.
 *
 * TODO: Flesh out with more documentation here.
 *
 * For dimensions 2 and 3, this template is specialised and offers
 * \e much more functionality.  In order to use these specialised
 * classes, you will need to include the corresponding headers
 * (dim2/dim2triangulation.h for \a dim = 2, or triangulation/ntriangulation.h
 * for \a dim = 3).  For convenience, there are typedefs available for
 * these specialised classes (Dim2Triangulation and NTriangulation
 * respectively).
 *
 * \ifacespython Python does not support templates.  For \a dim = 2 and 3,
 * this class is available in Python under the names Triangulation2 and
 * Triangulation3 respectively (as well as the typedefs mentioned above).
 * Higher-dimensional classes are not available in Python for the time being.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 */
template <int dim>
class REGINA_API Triangulation : public TriangulationBase<dim> {
    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Default constructor.
         *
         * Creates an empty triangulation.
         */
        Triangulation();
        /**
         * Creates a new copy of the given triangulation.
         *
         * @param copy the triangulation to copy.
         */
        Triangulation(const Triangulation& copy);

        /*@}*/
    private:
        /**
         * Clears any calculated properties and declares them all unknown.
         * This must be called by any internal function that changes the
         * triangulation.
         */
        void clearAllProperties();

    friend class SimplexBase<dim>;
    friend class TriangulationBase<dim>;
};

/*@}*/

// Inline functions for TriangulationBase

template <int dim>
inline TriangulationBase<dim>::TriangulationBase() {
}

template <int dim>
TriangulationBase<dim>::TriangulationBase(const TriangulationBase<dim>& copy) {
    // We don't fire a change event here since this is a constructor.
    // There should be nobody listening on events yet.

    SimplexIterator me, you;
    for (you = copy.simplices_.begin(); you != copy.simplices_.end(); ++you)
        simplices_.push_back(new Simplex<dim>((*you)->getDescription(),
            static_cast<Triangulation<dim>*>(this)));

    // Copy the internal simplex data, including gluings.
    int f;
    for (me = simplices_.begin(), you = copy.simplices_.begin();
            me != simplices_.end(); ++me, ++you) {
        for (f = 0; f <= dim; ++f) {
            if ((*you)->adj_[f]) {
                (*me)->adj_[f] = simplices_[(*you)->adj_[f]->index()];
                (*me)->gluing_[f] = (*you)->gluing_[f];
            } else
                (*me)->adj_[f] = 0;
        }
    }
}

template <int dim>
inline TriangulationBase<dim>::~TriangulationBase() {
    for (auto it = simplices_.begin(); it != simplices_.end(); ++it)
        delete *it;
}

template <int dim>
inline size_t TriangulationBase<dim>::size() const {
    return simplices_.size();
}

template <int dim>
inline size_t TriangulationBase<dim>::getNumberOfSimplices() const {
    return simplices_.size();
}

template <int dim>
inline const std::vector<Simplex<dim>*>& TriangulationBase<dim>::simplices()
        const {
    return (const std::vector<Simplex<dim>*>&)(simplices_);
}

template <int dim>
inline const std::vector<Simplex<dim>*>& TriangulationBase<dim>::getSimplices()
        const {
    return (const std::vector<Simplex<dim>*>&)(simplices_);
}

template <int dim>
inline Simplex<dim>* TriangulationBase<dim>::simplex(size_t index) {
    return simplices_[index];
}

template <int dim>
inline Simplex<dim>* TriangulationBase<dim>::getSimplex(size_t index) {
    return simplices_[index];
}

template <int dim>
inline const Simplex<dim>* TriangulationBase<dim>::simplex(size_t index) const {
    return simplices_[index];
}

template <int dim>
inline const Simplex<dim>* TriangulationBase<dim>::getSimplex(size_t index)
        const {
    return simplices_[index];
}

template <int dim>
inline size_t TriangulationBase<dim>::simplexIndex(const Simplex<dim>* simplex)
        const {
    return simplex->markedIndex();
}

template <int dim>
Simplex<dim>* TriangulationBase<dim>::newSimplex() {
    ChangeEventSpan<Triangulation<dim>> span(
        static_cast<Triangulation<dim>*>(this));
    Simplex<dim>* s = new Simplex<dim>(static_cast<Triangulation<dim>*>(this));
    simplices_.push_back(s);
    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
    return s;
}

template <int dim>
Simplex<dim>* TriangulationBase<dim>::newSimplex(const std::string& desc) {
    ChangeEventSpan<Triangulation<dim>> span(
        static_cast<Triangulation<dim>*>(this));
    Simplex<dim>* s = new Simplex<dim>(desc,
        static_cast<Triangulation<dim>*>(this));
    simplices_.push_back(s);
    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
    return s;
}

template <int dim>
inline void TriangulationBase<dim>::removeSimplex(Simplex<dim>* simplex) {
    ChangeEventSpan<Triangulation<dim>> span(
        static_cast<Triangulation<dim>*>(this));

    simplex->isolate();
    simplices_.erase(simplices_.begin() + simplex->markedIndex());
    delete simplex;

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
inline void TriangulationBase<dim>::removeSimplexAt(size_t index) {
    ChangeEventSpan<Triangulation<dim>> span(
        static_cast<Triangulation<dim>*>(this));

    Simplex<dim>* simplex = simplices_[index];
    simplex->isolate();
    simplices_.erase(simplices_.begin() + index);
    delete simplex;

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
inline void TriangulationBase<dim>::removeAllSimplices() {
    ChangeEventSpan<Triangulation<dim>> span(
        static_cast<Triangulation<dim>*>(this));

    for (auto it = simplices_.begin(); it != simplices_.end(); ++it)
        delete *it;
    simplices_.clear();

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

template <int dim>
void TriangulationBase<dim>::swapContents(Triangulation<dim>& other) {
    ChangeEventSpan<Triangulation<dim>> span1(
        static_cast<Triangulation<dim>*>(this));
    ChangeEventSpan<Triangulation<dim>> span2(&other);

    simplices_.swap(other.simplices_);

    SimplexIterator it;
    for (it = simplices_.begin(); it != simplices_.end(); ++it)
        (*it)->tri_ = static_cast<Triangulation<dim>*>(this);
    for (it = other.simplices_.begin(); it != other.simplices_.end(); ++it)
        (*it)->tri_ = &other;

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
    static_cast<Triangulation<dim>&>(other).clearAllProperties();
}

template <int dim>
void TriangulationBase<dim>::moveContentsTo(Triangulation<dim>& dest) {
    ChangeEventSpan<Triangulation<dim>> span1(
        static_cast<Triangulation<dim>*>(this));
    ChangeEventSpan<Triangulation<dim>> span2(&dest);

    SimplexIterator it;
    for (it = simplices_.begin(); it != simplices_.end(); ++it) {
        // This is an abuse of NMarkedVector, since for a brief moment
        // each triangle belongs to both vectors simplices_ and dest.simplices_.
        // However, the subsequent clear() operation does not touch the
        // markings (indices), and so we end up with the correct result
        // (i.e., the markings are correct for dest).
        (*it)->tri_ = &dest;
        dest.simplices_.push_back(*it);
    }
    simplices_.clear();

    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
    static_cast<Triangulation<dim>&>(dest).clearAllProperties();
}

template <int dim>
inline bool TriangulationBase<dim>::isEmpty() const {
    return simplices_.empty();
}

template <int dim>
inline bool TriangulationBase<dim>::hasBoundaryFacets() const {
    for (auto it = simplices_.begin(); it != simplices_.end(); ++it)
        if ((*it)->hasBoundary())
            return true;
    return false;
}

template <int dim>
bool TriangulationBase<dim>::isIdenticalTo(const Triangulation<dim>& other)
        const {
    if (simplices_.size() != other.simplices_.size())
        return false;

    SimplexIterator me, you;
    int f;
    for (me = simplices_.begin(), you = other.simplices_.begin();
            me != simplices_.end(); ++me, ++you) {
        for (f = 0; f <= dim; ++f) {
            if ((*you)->adj_[f]) {
                if ((*me)->adj_[f] != simplices_[(*you)->adj_[f]->index()])
                    return false;
                if ((*me)->gluing_[f] != (*you)->gluing_[f])
                    return false;
            } else {
                if ((*me)->adj_[f])
                    return false;
            }
        }
    }

    return true;
}

template <int dim>
inline void TriangulationBase<dim>::writeTextShort(std::ostream& out) const {
    if (simplices_.size() == 0)
        out << "Empty " << dim << "-dimensional triangulation";
    else
        out << "Triangulation with " << simplices_.size() << ' ' << dim << '-'
            << (simplices_.size() == 1 ? "simplex" : "simplices");
}

template <int dim>
void TriangulationBase<dim>::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << "\n\n";

    Simplex<dim>* simp;
    Simplex<dim>* adj;
    size_t pos;
    int i, j;
    NPerm<dim+1> gluing;

    out << "  Simplex  |  glued to:";
    for (i = dim; i >= 0; --i) {
        out << "     (";
        for (j = 0; j <= dim; ++j)
            if (j != i)
                out << regina::digit(j);
        out << ')';
    }
    out << '\n';
    out << "  ---------+-----------";
    for (i = dim; i >= 0; --i)
        for (j = 0; j < 7 + dim; ++j)
            out << '-';
    out << '\n';
    for (pos=0; pos < simplices_.size(); pos++) {
        simp = simplices_[pos];
        out << "     " << std::setw(4) << pos << "  |           ";
        for (i = dim; i >= 0; --i) {
            adj = simp->adjacentSimplex(i);
            if (! adj) {
                for (j = 0; j < dim - 1; ++j)
                    out << ' ';
                out << "boundary";
            } else {
                gluing = simp->adjacentGluing(i);
                out << std::setw(4) << adj->markedIndex() << " (";
                for (j = 0; j <= dim; ++j) {
                    if (j != i)
                        out << regina::digit(gluing[j]);
                }
                out << ")";
            }
        }
        out << '\n';
    }
    out << '\n';
}

#if 0
void Triangulation<dim>::insertTriangulation(const Triangulation<dim>& X) {
    ChangeEventSpan span(this);

    size_t nOrig = getNumberOfTriangles();
    size_t nX = X.getNumberOfTriangles();

    size_t triPos;
    for (triPos = 0; triPos < nX; ++triPos)
        newTriangle(X.simplices_[triPos]->getDescription());

    // Make the gluings.
    size_t adjPos;
    Simplex<dim>* tri;
    Simplex<dim>* adjTri;
    NPerm3 adjPerm;
    int edge;
    for (triPos = 0; triPos < nX; ++triPos) {
        tri = X.simplices_[triPos];
        for (edge = 0; edge < 3; ++edge) {
            adjTri = tri->adjacentSimplex(edge);
            if (adjTri) {
                adjPos = X.triangleIndex(adjTri);
                adjPerm = tri->adjacentGluing(edge);
                if (adjPos > triPos ||
                        (adjPos == triPos && adjPerm[edge] > edge)) {
                    simplices_[nOrig + triPos]->joinTo(edge,
                        simplices_[nOrig + adjPos], adjPerm);
                }
            }
        }
    }
}

void Triangulation<dim>::insertConstruction(size_t nTriangles,
        const int adjacencies[][3], const int gluings[][3][3]) {
    if (nTriangles == 0)
        return;

    Simplex<dim>** tri = new Simplex<dim>*[nTriangles];

    unsigned i, j;
    NPerm3 p;

    ChangeEventSpan span(this);

    for (i = 0; i < nTriangles; ++i)
        tri[i] = newTriangle();

    for (i = 0; i < nTriangles; ++i)
        for (j = 0; j < 3; ++j)
            if (adjacencies[i][j] >= 0 &&
                    ! tri[i]->adjacentSimplex(j)) {
                p = NPerm3(gluings[i][j][0], gluings[i][j][1],
                    gluings[i][j][2]);
                tri[i]->joinTo(j, tri[adjacencies[i][j]], p);
            }

    delete[] tri;
}

std::string Triangulation<dim>::dumpConstruction() const {
    std::ostringstream ans;
    ans <<
"/**\n";
    if (! getPacketLabel().empty())
        ans <<
" * 2-manifold triangulation: " << getPacketLabel() << "\n";
    ans <<
" * Code automatically generated by dumpConstruction().\n"
" */\n"
"\n";

    if (simplices_.empty()) {
        ans <<
"/* This triangulation is empty.  No code is being generated. */\n";
        return ans.str();
    }

    ans <<
"/**\n"
" * The following arrays describe the individual gluings of\n"
" * triangle edges.\n"
" */\n"
"\n";

    size_t nTriangles = simplices_.size();
    Simplex<dim>* tri;
    NPerm3 perm;
    size_t p;
    int e, i;

    ans << "const int adjacencies[" << nTriangles << "][3] = {\n";
    for (p = 0; p < nTriangles; ++p) {
        tri = simplices_[p];

        ans << "    { ";
        for (e = 0; e < 3; ++e) {
            if (tri->adjacentSimplex(e)) {
                ans << triangleIndex(tri->adjacentSimplex(e));
            } else
                ans << "-1";

            if (e < 2)
                ans << ", ";
            else if (p != nTriangles - 1)
                ans << "},\n";
            else
                ans << "}\n";
        }
    }
    ans << "};\n\n";

    ans << "const int gluings[" << nTriangles << "][3][3] = {\n";
    for (p = 0; p < nTriangles; ++p) {
        tri = simplices_[p];

        ans << "    { ";
        for (e = 0; e < 3; ++e) {
            if (tri->adjacentSimplex(e)) {
                perm = tri->adjacentGluing(e);
                ans << "{ ";
                for (i = 0; i < 3; ++i) {
                    ans << perm[i];
                    if (i < 2)
                        ans << ", ";
                    else
                        ans << " }";
                }
            } else
                ans << "{ 0, 0, 0 }";

            if (e < 2)
                ans << ", ";
            else if (p != nTriangles - 1)
                ans << " },\n";
            else
                ans << " }\n";
        }
    }
    ans << "};\n\n";

    ans <<
"/**\n"
" * The following code actually constructs a 2-manifold triangulation\n"
" * based on the information stored in the arrays above.\n"
" */\n"
"\n"
"Triangulation<dim> tri;\n"
"tri.insertConstruction(" << nTriangles << ", adjacencies, gluings);\n"
"\n";

    return ans.str();
}
#endif

// Inline functions for Triangulation

template <int dim>
inline Triangulation<dim>::Triangulation() : TriangulationBase<dim>() {
}

template <int dim>
inline Triangulation<dim>::Triangulation(const Triangulation& copy) :
        TriangulationBase<dim>(copy) {
}

template <int dim>
inline void Triangulation<dim>::clearAllProperties() {
}

} // namespace regina

#endif
