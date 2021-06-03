
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file surfaces/coordregistry.h
 *  \brief Provides access to a registry of all normal coordinate systems
 *  that can be used to create and store normal surfaces in 3-manifold
 *  triangulations.
 *
 *  Each time a new coordinate system is created, the file
 *  coordregistry-impl.h must be updated to include it.  Instructions on
 *  how to do this are included in coordregistry-impl.h.
 *
 *  External routines can access the registry by calling one of the
 *  forCoords() template functions defined in coordregistry.h.
 *
 *  \warning You should not include this header unless it is necessary,
 *  since it will automatically import every header for every coordinate
 *  system in the registry.
 */

// The old registry macros will silently compile but do nothing.
// This could lead to nasty surprises, so throw an error if it looks like
// people are still trying to use them.
#ifdef __FLAVOUR_REGISTRY_BODY
#error "The old REGISTER_FLAVOUR macros have been removed.  Use forCoords() instead."
#endif

#ifndef __COORDREGISTRY_H
#ifndef __DOXYGEN
#define __COORDREGISTRY_H
#endif

#include "surfaces/normalcoords.h"
#include "utilities/registryutils.h"

namespace regina {

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * Allows the user to call a template function whose template parameter
 * matches a given value of NormalCoords, which is not known
 * until runtime.  In essence, this routine contains a switch/case statement
 * that runs through all possible coordinate sytems.
 *
 * The advantages of this routine are that (i) the user does not need to
 * repeatedly type such switch/case statements themselves; and (ii) if
 * a new coordinate system is added then only a small amount of code
 * needs to be extended to incorporate it.
 *
 * This function can only work with coordinate systems in which
 * you can create and store normal surfaces.  All other coordinate systems are
 * considered invalid for our purposes here.
 *
 * In detail: the function object \a func must define a templated
 * bracket operator, so that <tt>func.operator()<NormalInfo<c>>(...)</tt> is
 * defined for any valid NormalCoords enum value \a c.  Then, when the user
 * calls <tt>forCoords(coords, func, defaultReturn, ...)</tt>, this routine
 * will call <tt>func.operator()<NormalInfo<coords>>(...)</tt> and pass back
 * the corresponding return value.  If \a coords does not denote a valid
 * coordinate system as described above, then forCoords() will pass back
 * \a defaultReturn instead.
 *
 * There are two other ways in which you can call forCoords() (these are
 * declared separately below):
 *
 * - If you do not with to supply your own \a defaultReturn (e.g., objects of
 *   the return type are expensive to construct), you can pass a ReturnDefault
 *   object in its place.  In this case, if \a coords does not denote a
 *   valid coordinate system, Regina will pass back a default-constructed
 *   value of the correct return type.
 *
 * - If \a func is a void function, then forCoords() will be void also,
 *   and there is no \a defaultReturn argument to pass at all.
 *
 * \pre If \a func returns a value, then the function object must have a
 * typedef \a ReturnType indicating the return type of the corresponding
 * templated bracket operator.  Inheriting from Returns<...> is a convenient
 * way to ensure this.
 *
 * \pre If \a func does not return a value, then the type
 * \a FunctionObject::ReturnType must not exist (even after any reference
 * specifiers are removed from the type \a FunctionObject).
 *
 * \ifacespython Not present.
 *
 * @param coords the given normal coordinate system.
 * @param func the function object whose bracket operator we will
 * call with a NormalInfo<coords> object.
 * @param defaultReturn the value to return if the given
 * coordinate system is invalid.
 * @param args any additional arguments to pass to the bracket operator
 * for \a func.  These will be copied/moved, so if you wish to pass
 * references then you may need to wrap them in std::ref or std::cref.
 * @return the return value from the corresponding bracket
 * operator of \a func, or \a defaultReturn if the given
 * coordinate system is invalid.
 */
template <typename FunctionObject, typename... Args>
typename ReturnsTraits<FunctionObject>::ReturnType
forCoords(NormalCoords coords, FunctionObject&& func,
        typename ReturnsTraits<FunctionObject>::ReturnType defaultReturn,
        Args&&... args);

/**
 * A variant of forCoords() for normal surface coordinates that does
 * not require the user to specify a default return value in advance.
 *
 * For detailed documentation, see the full version
 * forCoords(coords, func, defaultReturn, args...).
 *
 * \ifacespython Not present.
 *
 * @param coords the given normal surface coordinate system.
 * @param func the function object whose bracket operator we will
 * call with a NormalInfo<coords> object.
 * @param args any additional arguments to pass to the bracket operator
 * for \a func.  These will be copied/moved, so if you wish to pass
 * references then you may need to wrap them in std::ref or std::cref.
 * @return the return value from the corresponding bracket
 * operator of \a func, or a default-constructed return value if the given
 * coordinate system is invalid.
 */
template <typename FunctionObject, typename... Args>
typename ReturnsTraits<FunctionObject>::ReturnType
forCoords(NormalCoords coords, FunctionObject&& func, ReturnDefault,
        Args&&... args);

/**
 * A variant of forCoords() for normal surface coordinates that is
 * used for void functions.
 *
 * For detailed documentation, see the full version
 * forCoords(coords, func, defaultReturn, args...).
 *
 * \ifacespython Not present.
 *
 * @param coords the given normal surface coordinate system.
 * @param func the function object whose bracket operator we will
 * call with a NormalInfo<coords> object.
 * @param args any additional arguments to pass to the bracket operator
 * for \a func.  These will be copied/moved, so if you wish to pass
 * references then you may need to wrap them in std::ref or std::cref.
 */
template <typename FunctionObject, typename... Args>
typename ReturnsTraits<FunctionObject>::Void
forCoords(NormalCoords coords, FunctionObject&& func, Args&&... args);

/*@}*/

} // namespace regina

// Import template implementations:
#include "surfaces/coordregistry-impl.h"

#endif
