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
 * @copyright Copyright (C) 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SIGMAAGGREGATOR_H
#define SIGMAAGGREGATOR_H

#include <tbs/stats/SampleStats.h>

#include "ValueAggregator.h"

namespace hoot
{

using namespace std;

class SigmaAggregator : public ValueAggregator
{
public:

  static string className() { return "hoot::SigmaAggregator"; }

  /**
   * quantile - A value from 0 to 1 for the quantile.
   */
  SigmaAggregator() {};

  virtual double aggregate(vector<double>& d) const
  {
    return tbs::SampleStats(d).calculateUnbiasedStandardDeviation();
  }

  virtual QString toString() const { return QString("Sigma Aggregator"); }

private:
  double _quantile;
};

}


#endif // SIGMAAGGREGATOR_H
