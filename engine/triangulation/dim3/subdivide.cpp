
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include <set>
#include <vector>

#include "triangulation/dim3.h"

namespace regina {

bool Triangulation<3>::idealToFinite() {
    // The call to isValid() ensures the skeleton has been calculated.
    if (isValid() && ! isIdeal())
        return false;

    size_t numOldTet = simplices_.size();
    if (! numOldTet)
        return false;

    // Any simplex or facet locks at all will be a problem here.
    if (hasLocks())
        throw LockViolation("An attempt was made to subdivide a "
            "triangulation with one or more locked tetrahedra or triangles");

    // Since staging is new here, we will use the "raw" simplex routines
    // that do not generate change events / snapshots, check locks, etc.
    Triangulation<3> staging;

    auto* newTet = new Tetrahedron<3>*[32*numOldTet];
    for (size_t i=0; i<32*numOldTet; i++)
        newTet[i] = staging.newSimplexRaw();

    int tip[4];
    int interior[4];
    int edge[4][4];
    int vertex[4][4];

    int nDiv = 0;
    for (int j=0; j<4; j++) {
        tip[j] = nDiv++;
        interior[j] = nDiv++;

        for (int k=0; k<4; k++)
            if (j != k) {
                edge[j][k] = nDiv++;
                vertex[j][k] = nDiv++;
            }
    }

    // First glue all of the tetrahedra inside the same
    // old tetrahedron together.
    for (size_t i=0; i<numOldTet; i++) {
        // Glue the tip tetrahedra to the others.
        for (int j=0; j<4; j++)
            newTet[tip[j] + i * nDiv]->joinRaw(j,
                newTet[interior[j] + i * nDiv], Perm<4>());

        // Glue the interior tetrahedra to the others.
        for (int j=0; j<4; j++) {
            for (int k=0; k<4; k++)
                if (j != k) {
                    newTet[interior[j] + i * nDiv]->joinRaw(k,
                        newTet[vertex[k][j] + i * nDiv], Perm<4>());
                }
        }

        // Glue the edge tetrahedra to the others.
        for (int j=0; j<4; j++)
            for (int k=0; k<4; k++)
                if (j != k) {
                    if (j < k)
                        newTet[edge[j][k] + i * nDiv]->joinRaw(j,
                            newTet[edge[k][j] + i * nDiv], Perm<4>(j,k));

                    for (int l=0; l<4; l++)
                        if ( (l != j) && (l != k) )
                            newTet[edge[j][k] + i * nDiv]->joinRaw(l,
                                newTet[vertex[j][l] + i * nDiv], Perm<4>(k,l));
                }
    }

    // Now deal with the gluings between the pieces inside adjacent tetrahedra.
    for (size_t i=0; i<numOldTet; i++) {
        Tetrahedron<3>* ot = tetrahedron(i);
        for (int j=0; j<4; j++)
            if (ot->adjacentTetrahedron(j)) {
                 size_t oppTet = ot->adjacentTetrahedron(j)->index();
                 Perm<4> p = ot->adjacentGluing(j);

                 // Do each gluing from one side only.
                 if (oppTet < i || (oppTet == i && p[j] < j))
                    continue;

                 // First deal with the tip tetrahedra.
                 for (int k=0; k<4; k++)
                     if (j != k)
                          newTet[tip[k] + i * nDiv]->joinRaw(j,
                              newTet[tip[p[k]] + oppTet * nDiv], p);

                 // Next the edge tetrahedra.
                 for (int k=0; k<4; k++)
                     if (j != k)
                         newTet[edge[j][k] + i * nDiv]->joinRaw(k,
                             newTet[edge[p[j]][p[k]] + oppTet * nDiv], p);

                 // Finally, the vertex tetrahedra.
                 for (int k=0; k<4; k++)
                     if (j != k)
                         newTet[vertex[j][k] + i * nDiv]->joinRaw(k,
                             newTet[vertex[p[j]][p[k]] + oppTet * nDiv], p);

            }
    }

    delete[] newTet;

    // Now remove any new tetrahedra that touch an ideal or invalid vertex.
    // We do this by making a list first, then actually doing the deletion
    // (since the first deletion will destroy the skeleton).

    staging.ensureSkeleton();

    std::vector<Tetrahedron<3>*> tetList;
    for (Vertex<3>* v : staging.vertices())
        if (v->isIdeal() || ! v->isValid())
            for (auto& emb : *v)
                tetList.push_back(emb.tetrahedron());

    // Just above, we computed the skeleton for staging so we could query
    // its vertices.  We need to delete this computed property now, since
    // we are about to edit the staging triangulation further using
    // removeSimplexRaw() with no surrounding ChangeEventSpan.
    // This means the skeleton will become incorrect, and we do not want
    // this incorrect skeleton to be moved into this triangulation as
    // part of the final swap().
    staging.clearAllProperties();

    for (auto t : tetList)
        staging.removeSimplexRaw(t);

    // We are now ready to change the main triangulation.
    // This is where the change event and snapshot will be fired.
    swap(staging);
    return true;
}

void Triangulation<3>::puncture(Tetrahedron<3>* tet) {
    if (! tet) {
        // Preconditions disallow empty triangulations, but anyway:
        if (simplices_.empty())
            return;

        tet = simplices_.front();
    }

    ChangeEventGroup span(*this);

    // We will attach a pair of triangular prisms to face 123 of tet.
    // We will join the rectangular walls of the prisms together, and
    // one triangular end from each will join to form the new S^2 boundary.
    Tetrahedron<3>* prism[2][3];

    // Create the new tetrahedra in an order that ensures that the new
    // S^2 boundary will appear in the final two tetrahedra.
    int i, j;
    for (j = 0; j < 3; ++j)
        for (i = 0; i < 2; ++i)
            prism[i][j] = newTetrahedron();

    prism[0][0]->join(0, prism[0][1], Perm<4>(3,0,1,2));
    prism[0][1]->join(0, prism[0][2], Perm<4>(3,0,1,2));

    prism[1][0]->join(1, prism[1][1], Perm<4>(3,0,1,2));
    prism[1][1]->join(1, prism[1][2], Perm<4>(3,2,0,1));

    prism[0][0]->join(1, prism[1][0], Perm<4>(1,2,3,0));
    prism[0][0]->join(2, prism[1][0], Perm<4>(1,2,3,0));
    prism[0][1]->join(1, prism[1][1], Perm<4>(1,2,3,0));
    prism[0][1]->join(2, prism[1][1], Perm<4>(1,2,3,0));
    prism[0][2]->join(1, prism[1][2], Perm<4>(0,1,3,2));
    prism[0][2]->join(2, prism[1][2], Perm<4>(0,1,3,2));

    Tetrahedron<3>* adj = tet->adjacentTetrahedron(0);
    if (adj) {
        Perm<4> gluing = tet->adjacentGluing(0);
        tet->unjoin(0);
        prism[1][0]->join(0, adj, gluing);
    }

    tet->join(0, prism[0][0], Perm<4>(3,0,1,2));
}

void Triangulation<3>::connectedSumWith(const Triangulation<3>& other) {
    if (other.simplices_.empty())
        return;
    if (simplices_.empty()) {
        insertTriangulation(other);
        return;
    }

    // From here we can assume that each triangulation contains at least
    // one tetrahedron.

    ChangeEventGroup span(*this);

    // Insert the other triangulation *before* puncturing this, so that
    // things work in the case where we sum a triangulation with itself.
    unsigned long n = simplices_.size();
    insertTriangulation(other);

    // Make the puncture and record the resulting new boundary triangles.
    puncture(simplices_.front());
    Tetrahedron<3>* bdry[2] = {
        simplices_[simplices_.size() - 2],
        simplices_[simplices_.size() - 1]
    };

    // Pop open a triangle in the second triangulation, and join the
    // two resulting boundary triangles to the boundary from the puncture.
    //
    // Even if the triangle we picked is a boundary triangle (i.e., has
    // degree 1, not degree 2), the overall effect remains correct.

    Triangle<3>* open = simplices_[n]->triangle(0);
    if (open->degree() == 2) {
        const FaceEmbedding<3, 2> emb1 = open->front();
        const FaceEmbedding<3, 2> emb2 = open->back();

        emb1.tetrahedron()->unjoin(emb1.vertices()[3]);

        // We choose the gluing permutations so that, if both triangulations
        // are oriented, the connected sum respects this orientation.
        bool even = (emb1.vertices().sign() > 0);

        if (even) {
            bdry[0]->join(0, emb1.tetrahedron(),
                emb1.vertices() * Perm<4>(3,0,1,2));
            bdry[1]->join(0, emb2.tetrahedron(),
                emb2.vertices() * Perm<4>(3,0,2,1));
        } else {
            bdry[0]->join(0, emb1.tetrahedron(),
                emb1.vertices() * Perm<4>(3,0,2,1));
            bdry[1]->join(0, emb2.tetrahedron(),
                emb2.vertices() * Perm<4>(3,0,1,2));
        }
    } else {
        const FaceEmbedding<3, 2> emb1 = open->front();

        // We choose the gluing permutations so that, if both triangulations
        // are oriented, the connected sum respects this orientation.
        bool even = (emb1.vertices().sign() > 0);

        if (even) {
            bdry[0]->join(0, emb1.tetrahedron(),
                emb1.vertices() * Perm<4>(3,0,1,2));
        } else {
            bdry[0]->join(0, emb1.tetrahedron(),
                emb1.vertices() * Perm<4>(3,0,2,1));
        }
    }
}

} // namespace regina
