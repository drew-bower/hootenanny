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
#ifndef EXTRACTWAYSVISITOR_H
#define EXTRACTWAYSVISITOR_H

#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>

namespace hoot
{

class ExtractWaysVisitor : public ElementConstOsmMapVisitor
{
public:

  ExtractWaysVisitor(vector<ConstWayPtr>& w) : _w(w) {}

  virtual void visit(const shared_ptr<const Element>& e)
  {
    if (e->getElementType() == ElementType::Way)
    {
      ConstWayPtr w = dynamic_pointer_cast<const Way>(e);
      _w.push_back(w);
    }
  }

  /**
   * Convenience method.
   */
  static vector<ConstWayPtr> extractWays(const ConstOsmMapPtr& map, const ConstElementPtr& e)
  {
    vector<ConstWayPtr> result;
    ExtractWaysVisitor v(result);
    v.setOsmMap(map.get());
    e->visitRo(*map, v);
    return result;
  }

private:
  vector<ConstWayPtr>& _w;
};

}

#endif // EXTRACTWAYSVISITOR_H
