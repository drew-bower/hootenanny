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
 * @copyright Copyright (C) 2013, 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HIGHWAYSNAPMERGERCREATOR_H
#define HIGHWAYSNAPMERGERCREATOR_H

// hoot
#include <hoot/core/conflate/MergerCreator.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class HighwaySnapMergerCreator : public MergerCreator, public Configurable
{
public:
  static string className() { return "hoot::HighwaySnapMergerCreator"; }

  HighwaySnapMergerCreator();

  /**
   * If there is a single HighwayMatch, a single HighwaySnapMerger will be created and returned.
   */
  virtual bool createMergers(const MatchSet& matches, vector<Merger*>& mergers) const;

  virtual vector<Description> getAllCreators() const;

  virtual bool isConflicting(const ConstOsmMapPtr& map, const Match* m1, const Match* m2) const;

  virtual void setConfiguration(const Settings &conf);

private:
  Meters _minSplitSize;
};

}

#endif // HIGHWAYSNAPMERGERCREATOR_H