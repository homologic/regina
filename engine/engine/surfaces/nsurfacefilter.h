
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nsurfacefilter.h
 *  \brief Contains a packet that filters through normal surfaces.
 */

#ifndef __NSURFACEFILTER_H
#ifndef __DOXYGEN
#define __NSURFACEFILTER_H
#endif

#include "packet/npacket.h"
#include "property/npropertyholder.h"

class NNormalSurface;
class NNormalSurfaceList;

/**
 * A packet that accepts or rejects normal surfaces.
 * Different subclasses of NSurfaceFilter represent different filtering
 * methods.
 *
 * <b>When deriving classes from NSurfaceFilter:</b>
 * <ul>
 *   <li>Follow the instructions on the \ref HowTo page for
 *   adding a new type of normal surface filter.</li>
 *   <li>A copy constructor <tt>class(const class& cloneMe)</tt> should
 *   be declared and implemented.  You may assume that parameter
 *   \a cloneMe is of the same class as that whose constructor you are
 *   writing.</li>
 *   <li>Virtual functions accept(), writeTextLong() and writeFilter()
 *   should be overridden.</li>
 *   <li>Static function readFilter() should be implemented as described
 *   in the documentation below.</li>
 *   <li>Virtual functions getFilterID() and getFilterName()
 *   should be redeclared but not reimplemented.
 *   The registry utilities will take care of their implementation.</li>
 *   <li><tt>public static const int filterID</tt> should be declared.
 *   The registry utilities will take care of assigning it a value.</li>
 * </ul>
 *
 * If a derived class is to have properties of its own, the following
 * points should be noted:
 * <ul>
 *   <li> Property IDs 1-1000 are reserved for use by NSurfaceFilter itself.
 *   <li> Routine writeProperties() <b>must</b> begin by calling the
 *   corresponding superclass routine.
 * </ul>
 *
 * \todo \feature Implement property \a lastAppliedTo, which necessitates an
 * about-to-die event notification system in NPacket.
 */
class NSurfaceFilter : public NPacket, public NPropertyHolder {
    public:
        /**
         * Contains the integer ID for this type of surface filter.
         * Each distinct filtering class must have a unique ID, and this
         * should be a non-negative integer.  See filterregistry.h for
         * further details.
         *
         * This member must be declared for every filtering class that
         * will be instantiated.  A value need not be assigned;
         * filterregistry.h will take care of this task when you register
         * the filtering class.
         *
         * \ifaces Not present.
         */
        static const int filterID;

        static const int packetType;

    public:
        /**
         * Creates a new default surface filter.  This will simply accept
         * all normal surfaces.
         */
        NSurfaceFilter();
        /**
         * Creates a new default surface filter.  This will simply accept
         * all normal surfaces.  Note that the given parameter is
         * ignored.
         *
         * @param cloneMe this parameter is ignored.
         */
        NSurfaceFilter(const NSurfaceFilter& cloneMe);
        /**
         * Destroys this surface filter.
         */
        virtual ~NSurfaceFilter();

        /**
         * Decides whether or not the given normal surface is accepted by this
         * filter.
         *
         * The default implementation simply returns \c true.
         *
         * @param surface the normal surface under investigation.
         * @return \c true if and only if the given surface is accepted
         * by this filter.
         */
        virtual bool accept(NNormalSurface& surface) const;

        /**
         * Returns the unique integer ID corresponding to the filtering
         * method that is this particular subclass of NSurfaceFilter.
         *
         * @return the unique integer filtering method ID.
         */
        virtual int getFilterID() const;
        /**
         * Returns a string description of the filtering method that is
         * this particular subclass of NSurfaceFilter.
         *
         * @return a string description of this filtering method.
         */
        virtual NString getFilterName() const;

        /**
         * Reads the details of a normal surface filter from the
         * specified file and returns a newly created filter containing
         * that information.  You may assume that the filter is of the
         * same class as the class in which you are implementing this
         * routine.  The newly created filter must also be of this type.
         *
         * The general packet information and the filter ID may be
         * assumed to have already been read from the file, and should
         * <b>not</b> be reread.  The readFilter() routine should read
         * exactly what writeFilter() writes, and vice versa.
         *
         * Properties should not be read from here; this will be done later
         * by another routine.
         *
         * \a parent represents the packet which will become the new
         * filter's parent in the tree structure.  This information is
         * for reference only, and need not be used.
         * See the description of parameter \a parent in
         * NPacket::readPacket() for further details.
         *
         * \pre The given file is open for reading and all above
         * conditions have been satisfied.
         *
         * \ifaces Not present.
         *
         * @param in the file from which to read the filter.
         * @param parent the packet which will become the new filter's
         * parent in the tree structure, or 0 if the new filter is to be
         * tree matriarch.
         * @return the filter read from file, or 0 if an error occurred.
         */
        static NSurfaceFilter* readFilter(NFile& in, NPacket* parent);

        virtual int getPacketType() const;
        virtual NString getPacketName() const;
        virtual void writeTextShort(ostream& out) const;
        virtual void writePacket(NFile& out) const;
        static NSurfaceFilter* readPacket(NFile& in, NPacket* parent);
        virtual bool dependsOnParent() const;

    protected:
        /**
         * Writes the details of this filter to file.
         *
         * You may assume that general packet information and the filter
         * ID have already been written.  Only the actual data stored for
         * this particular subclass of NSurfaceFilter need be written.
         *
         * Properties should not be written from here; this will be done later
         * by another routine.
         *
         * \pre The given file is open for writing and satisfies the
         * assumptions listed above.
         *
         * @param out the file to be written to.
         */
        virtual void writeFilter(NFile& out) const;
        /**
         * Writes to file any properties associated specifically with
         * this particular subclass of NSurfaceFilter.
         * This should consist of a call to the superclass implementation
         * followed by a series of writePropertyHeader() and
         * writePropertyFooter() calls with property information being
         * written inside these pairs.
         *
         * This routine should <b>not</b> call writeAllPropertiesFooter().
         *
         * @param out the file to be written to.
         */
        virtual void writeProperties(NFile& out) const;
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void readIndividualProperty(NFile& infile, unsigned propType);
        virtual void initialiseAllProperties();
};

// Inline functions for NSurfaceFilter

inline NSurfaceFilter::NSurfaceFilter() {
}
inline NSurfaceFilter::NSurfaceFilter(const NSurfaceFilter& cloneMe) {
}
inline NSurfaceFilter::~NSurfaceFilter() {
}

inline bool NSurfaceFilter::accept(NNormalSurface&) const {
    return true;
}

inline void NSurfaceFilter::writeFilter(NFile&) const {
}

inline void NSurfaceFilter::writeTextShort(ostream& o) const {
    o << getFilterName();
}

inline bool NSurfaceFilter::dependsOnParent() const {
    return false;
}

inline void NSurfaceFilter::initialiseAllProperties() {
}

#endif

