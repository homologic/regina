
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "census/ncensus.h"
#include "packet/ncontainer.h"
#include "testsuite/census/testcensus.h"

using regina::NBoolSet;
using regina::NCensus;
using regina::NContainer;

class NCensusTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NCensusTest);

    CPPUNIT_TEST(rawCounts);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void rawCounts() {
            unsigned nAll[] = { 1, 5, 61, 1581 };
            rawCountsCompare(1, 3, nAll, "closed",
                NBoolSet::sBoth, NBoolSet::sBoth, NBoolSet::sFalse, 0);

            unsigned nOrientable[] = { 1, 4, 35, 454, 13776 };
            rawCountsCompare(1, 3, nOrientable, "closed orbl",
                NBoolSet::sBoth, NBoolSet::sTrue, NBoolSet::sFalse, 0);
        }

        static void rawCountsCompare(unsigned minTets, unsigned maxTets,
                const unsigned* realAns, const char* censusType,
                NBoolSet finiteness, NBoolSet orientability,
                NBoolSet boundary, int nBdryFaces) {
            NContainer* census;

            for (unsigned nTets = minTets; nTets <= maxTets; nTets++) {
                census = new NContainer();
                NCensus::formCensus(census, nTets, finiteness, orientability,
                    boundary, nBdryFaces, 0);

                std::ostringstream msg;
                msg << "Census count for " << nTets << " tetrahedra ("
                    << censusType << ") should be " << realAns[nTets]
                    << ", not " << census->getNumberOfChildren() << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    census->getNumberOfChildren() == realAns[nTets]);
                delete census;
            }
        }
};

void addNCensus(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NCensusTest::suite());
}

