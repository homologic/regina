
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

#include "angle/nanglestructurelist.h"
#include "enumerate/ndoubledescription.h"
#include "enumerate/ntreetraversal.h"
#include "maths/nmatrixint.h"
#include "progress/nprogresstracker.h"
#include "surfaces/nnormalsurface.h"
#include "triangulation/ntriangulation.h"
#include "utilities/xmlutils.h"

// Property IDs:
#define PROPID_ALLOWSTRICT 1
#define PROPID_ALLOWTAUT 2

namespace regina {

typedef std::vector<NAngleStructure*>::const_iterator StructureIteratorConst;

void* NAngleStructureList::Enumerator::run(void*) {
    // Form the matching equations.
    NMatrixInt* eqns = NAngleStructureVector::makeAngleEquations(triang);

    if (list->tautOnly_ && triang->getNumberOfTetrahedra() > 0) {
        // For now just stick to arbitrary precision arithmetic.
        // TODO: Use native integer types when the angle equation matrix
        // is sufficiently small / simple.
        if (tracker)
            tracker->newStage("Enumerating taut angle structures");

        NTautEnumeration<LPConstraintNone, BanNone, NInteger> search(triang);
        while (search.next(tracker)) {
            list->structures.push_back(search.buildStructure());
            if (tracker && tracker->isCancelled())
                break;
        }

        if (! (tracker && tracker->isCancelled()))
            triang->insertChildLast(list);

        if (tracker)
            tracker->setFinished();
    } else {
        // For the empty triangulation, we fall through here regardless
        // of whether we want taut or all vertex angle structures (but
        // either way, the answer is the same - just one empty structure).
        //
        // For all other triangulations, we fall through here if we are
        // after all vertex angle structures.
        if (tracker)
            tracker->newStage("Enumerating vertex angle structures");

        // Find the angle structures.
        NDoubleDescription::enumerateExtremalRays<NAngleStructureVector>(
            StructureInserter(*list, triang), *eqns, 0 /* constraints */,
            tracker);

        // All done!
        if (! (tracker && tracker->isCancelled()))
            triang->insertChildLast(list);

        if (tracker)
            tracker->setFinished();
    }

    delete eqns;
    return 0;
}

NAngleStructureList* NAngleStructureList::enumerate(NTriangulation* owner,
        bool tautOnly, NProgressTracker* tracker) {
    NAngleStructureList* ans = new NAngleStructureList(tautOnly);
    Enumerator* e = new Enumerator(ans, owner, tracker);

    if (tracker) {
        if (! e->start(0, true)) {
            delete ans;
            return 0;
        }
        return ans;
    } else {
        e->run(0);
        delete e;
        return ans;
    }
}

NTriangulation* NAngleStructureList::getTriangulation() const {
    return dynamic_cast<NTriangulation*>(getTreeParent());
}

void NAngleStructureList::writeTextShort(std::ostream& o) const {
    o << structures.size() << " vertex angle structure";
    if (structures.size() != 1)
        o << 's';
    o << " (" << (tautOnly_ ? "taut only" : "no restrictions")  << ')';
}

void NAngleStructureList::writeTextLong(std::ostream& o) const {
    writeTextShort(o);
    o << ":\n";

    for (StructureIteratorConst it = structures.begin();
            it != structures.end(); it++) {
        (*it)->writeTextShort(o);
        o << '\n';
    }
}

void NAngleStructureList::writeXMLPacketData(std::ostream& out) const {
    using regina::xml::xmlValueTag;

    // Write the enumeration parameters.
    out << "  <angleparams tautonly=\"" << (tautOnly_ ? 'T' : 'F') << "\"/>\n";

    // Write the individual structures.
    for (StructureIteratorConst it = structures.begin();
            it != structures.end(); it++)
        (*it)->writeXMLData(out);

    // Write the properties.
    if (doesSpanStrict.known())
        out << "  " << xmlValueTag("spanstrict", doesSpanStrict.value())
            << '\n';
    if (doesSpanTaut.known())
        out << "  " << xmlValueTag("spantaut", doesSpanTaut.value())
            << '\n';
}

NPacket* NAngleStructureList::internalClonePacket(NPacket* /* parent */)
        const {
    NAngleStructureList* ans = new NAngleStructureList(tautOnly_);
    transform(structures.begin(), structures.end(),
        back_inserter(ans->structures), FuncNewClonePtr<NAngleStructure>());

    if (doesSpanStrict.known())
        ans->doesSpanStrict = doesSpanStrict;
    if (doesSpanTaut.known())
        ans->doesSpanTaut = doesSpanTaut;

    return ans;
}

void NAngleStructureList::calculateSpanStrict() const {
    if (structures.empty()) {
        doesSpanStrict = false;
        return;
    }

    unsigned long nTets = getTriangulation()->getNumberOfTetrahedra();
    if (nTets == 0) {
        doesSpanStrict = true;
        return;
    }

    // We run into trouble if there's a 0 or pi angle that never changes.
    NRational* fixedAngles = new NRational[nTets * 3];
    unsigned long nFixed = 0;

    // Get the list of bad unchanging angles from the first structure.
    StructureIteratorConst it = structures.begin();
    const NAngleStructure* s = *it;

    NRational angle;
    unsigned long tet;
    int edges;
    for (tet = 0; tet < nTets; tet++)
        for (edges = 0; edges < 3; edges++) {
            angle = s->getAngle(tet, edges);
            if (angle == NRational::zero || angle == NRational::one) {
                fixedAngles[3 * tet + edges] = angle;
                nFixed++;
            } else
                fixedAngles[3 * tet + edges] = NRational::undefined;
        }

    if (nFixed == 0) {
        doesSpanStrict = true;
        delete[] fixedAngles;
        return;
    }

    // Run through the rest of the structures to see if these bad angles
    // do ever change.
    for (it++; it != structures.end(); it++) {
        s = *it;
        for (tet = 0; tet < nTets; tet++)
            for (edges = 0; edges < 3; edges++) {
                if (fixedAngles[3 * tet + edges] == NRational::undefined)
                    continue;
                if (s->getAngle(tet, edges) != fixedAngles[3 * tet + edges]) {
                    // Here's a bad angle that finally changed.
                    fixedAngles[3 * tet + edges] = NRational::undefined;
                    nFixed--;
                    if (nFixed == 0) {
                        doesSpanStrict = true;
                        delete[] fixedAngles;
                        return;
                    }
                }
            }
    }

    // Some of the bad angles never changed.
    doesSpanStrict = false;
    delete[] fixedAngles;
}

void NAngleStructureList::calculateSpanTaut() const {
    doesSpanTaut = (find_if(structures.begin(), structures.end(),
        std::mem_fun(&NAngleStructure::isTaut)) != structures.end());
}

} // namespace regina

