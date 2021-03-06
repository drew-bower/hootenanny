/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/MapReprojector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/point/PlacesPoiMatchCreator.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>

// Qt
#include <QDebug>
#include <QDir>
#include <QBuffer>
#include <QByteArray>


#include "../../TestUtils.h"

namespace hoot
{

class PlacesPoiMatchCreatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PlacesPoiMatchCreatorTest);
  CPPUNIT_TEST(runIsCandidateTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runIsCandidateTest()
  {


    OsmMapPtr map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0.0;
    env.MinY = 0.0;
    env.MaxX = 1.0;
    env.MaxY = 1.0;
    MapReprojector::reprojectToPlanar(map, env);

    //to be a match candidate: needs to be a node, in bounds (only if a bounds has been set), and
    //be a poi; being a poi means its tagged as a poi OR has a name tag

    PlacesPoiMatchCreator uut;
    Tags tags;

    tags.appendValue("name", "node1");
    tags.appendValue("poi", "yes");
    NodePtr node1 = TestUtils::createNode(map, Status::Unknown1, 10.0, 10.0, 250.0, tags);
    CPPUNIT_ASSERT(uut.isMatchCandidate(node1, map));

    tags.clear();
    tags.appendValue("name", "node2");
    NodePtr node2 = TestUtils::createNode(map, Status::Unknown1, 10.0, 10.0, 250.0, tags);
    CPPUNIT_ASSERT(uut.isMatchCandidate(node2, map));

    tags.clear();
    tags.appendValue("poi", "yes");
    NodePtr node3 = TestUtils::createNode(map, Status::Unknown1, 10.0, 10.0, 250.0, tags);
    CPPUNIT_ASSERT(uut.isMatchCandidate(node3, map));

    NodePtr node4 = TestUtils::createNode(map, Status::Unknown1, 10.0, 10.0, 250.0);
    CPPUNIT_ASSERT(!uut.isMatchCandidate(node4, map));

    QList<NodePtr> wayNodes;
    wayNodes.append(node1);
    wayNodes.append(node2);
    WayPtr way1 = TestUtils::createWay(map, wayNodes, Status::Unknown1);
    CPPUNIT_ASSERT(!uut.isMatchCandidate(way1, map));

    //x1, x2, y1, y2; bounds expected to be in WGS84
    uut.setBounds(Envelope(0.0, 0.6, 0.0, 0.6));
    CPPUNIT_ASSERT(uut.isMatchCandidate(node1, map));

    //See PlacesPoiMatchCreator::setBoundable for a explanation of why a new object has to be
    //created here.
    PlacesPoiMatchCreator uut2;
    uut2.setBounds(Envelope(0.0, 0.4, 0.0, 0.4));
    CPPUNIT_ASSERT(!uut2.isMatchCandidate(node1, map));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PlacesPoiMatchCreatorTest, "quick");

}
