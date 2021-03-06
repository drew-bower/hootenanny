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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSMSCHEMA_H
#define OSMSCHEMA_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QString>

// Standard
#include <stdint.h>
#include <vector>

namespace hoot
{

  using namespace std;

enum TagValueType
{
  Boolean,
  Text,
  Enumeration,
  Int,
  Unknown
};

enum EdgeType
{
  CanHave,
  IsA,
  SimilarTo,
  ParentOf,
  AssociatedWith
};

struct OsmSchemaCategory {
  typedef enum Type
  {
    Empty = 0,
    Poi = 1,
    Building = 2,
    Transportation = 4,
    Use = 8,
    Name = 16,
    PseudoName = 32,
    All = Poi | Building | Transportation | Use | Name
  } Type;

  OsmSchemaCategory() : _type(Empty) {}
  OsmSchemaCategory(OsmSchemaCategory::Type t) : _type(t) {}

  static OsmSchemaCategory building() { return OsmSchemaCategory(Building); }
  static OsmSchemaCategory poi() { return OsmSchemaCategory(Poi); }
  static OsmSchemaCategory transportation() { return OsmSchemaCategory(Transportation); }
  static OsmSchemaCategory use() { return OsmSchemaCategory(Use); }
  static OsmSchemaCategory name() { return OsmSchemaCategory(Name); }
  static OsmSchemaCategory pseudoName() { return OsmSchemaCategory(PseudoName); }

  bool operator==(OsmSchemaCategory t) const { return t._type == _type; }
  bool operator!=(OsmSchemaCategory t) const { return t._type != _type; }

  Type getEnum() const { return _type; }

  static OsmSchemaCategory fromString(const QString& s)
  {
    if (s == "poi")
    {
      return Poi;
    }
    else if (s == "building")
    {
      return Building;
    }
    else if (s == "transportation")
    {
      return Transportation;
    }
    else if (s == "use")
    {
      return Use;
    }
    else if (s == "name")
    {
      return Name;
    }
    else if (s == "pseudoname")
    {
      return PseudoName;
    }
    else if (s == "")
    {
      return Empty;
    }
    else
    {
      throw IllegalArgumentException("Unknown category. " + s);
    }
  }

  static OsmSchemaCategory fromStringList(const QStringList& s);

  /**
   * Returns true if all of the categories in c are in this. If this is empty then it always returns
   * true.
   */
  bool contains(const OsmSchemaCategory& c) const
  {
    return (getEnum() & c.getEnum()) == c.getEnum();
  }

  /**
   * Returns true if any of the categories in c are the same as the categories in this. If one or
   * both are emtpy then it returns false.
   */
  bool intersects(const OsmSchemaCategory& c) const
  {
    return (getEnum() & c.getEnum()) != Empty;
  }

  QString toString() const { return toStringList().join(","); }

  QStringList toStringList() const
  {
    QStringList result;

    if (_type & Poi)
    {
      result << "poi";
    }
    if (_type & Building)
    {
      result << "building";
    }
    if (_type & Transportation)
    {
      result << "transportation";
    }
    if (_type & Use)
    {
      result << "use";
    }
    if (_type & Name)
    {
      result << "name";
    }
    if (_type & PseudoName)
    {
      result << "pseudoname";
    }

    return result;
  }

private:
  OsmSchemaCategory::Type _type;
};

inline OsmSchemaCategory operator&(const OsmSchemaCategory t1, const OsmSchemaCategory t2)
{
  return OsmSchemaCategory((OsmSchemaCategory::Type)((int)t1.getEnum() & (int)t2.getEnum()));
}

inline OsmSchemaCategory operator|(const OsmSchemaCategory t1, const OsmSchemaCategory t2)
{
  return OsmSchemaCategory((OsmSchemaCategory::Type)((int)t1.getEnum() | (int)t2.getEnum()));
}

inline OsmSchemaCategory operator|(const OsmSchemaCategory::Type t1, const OsmSchemaCategory t2)
{
  return OsmSchemaCategory((OsmSchemaCategory::Type)((int)t1 | (int)t2.getEnum()));
}

inline OsmSchemaCategory operator|(const OsmSchemaCategory t1, const OsmSchemaCategory::Type t2)
{
  return OsmSchemaCategory((OsmSchemaCategory::Type)((int)t1.getEnum() | (int)t2));
}

// explicitly put these types in their own name scope. Use with OsmGeometries::Node, etc.
struct OsmGeometries {
  enum e
  {
    Node=0x01,
    Area=0x02,
    LineString=0x04,
    ClosedWay=0x08,
    Way=Area | LineString | ClosedWay,
    Relation=0x10
  };

  static uint16_t fromString(const QString& s);
};

struct TagEdge
{
public:

  TagEdge()
  {
    show = true;
  }

  enum EdgeType type;
  double similarToWeight;
  double averageWeight;
  bool show;
  double w1;
  double w2;
};

struct TagVertex
{

  TagVertex()
  {
    influence = -1.0;
    valueType = Unknown;
    childWeight = -1.0;
    mismatchScore = -1.0;
    geometries = 0;
  }

  bool isEmpty() const
  {
    return name.isEmpty();
  }

  bool isValid() const { return !isEmpty(); }

  QString toString() const
  {
    QString result = QString("name: %1\n").arg(name)
        + QString("key: %1\n").arg(key)
        + QString("value: %1\n").arg(value)
        + QString("influence: %1\n").arg(influence)
        + QString("childWeight: %1\n").arg(childWeight)
        + QString("mismatchScore: %1\n").arg(mismatchScore)
        + QString("valueType: %1\n").arg(valueType)
        + QString("aliases: %1\n").arg(Tgs::toString(aliases))
        + QString("geometries: %1\n").arg(geometries)
        + QString("categories: %1\n").arg(Tgs::toString(categories));
    return result;
  }

  QString name;
  QString key;
  QString value;
  double influence;
  double childWeight;

  /**
   * The mismatchScore is used only with wildcard enumerated types. (e.g. addr:housenumber=*).
   * This score is the score returned when two wildcard enumerated types are compared that have
   * different values.
   */
  double mismatchScore;
  enum TagValueType valueType;
  QStringList aliases;

  /**
   * Each tag can have categories associated with it. This can help when grouping a number of
   * disparate tags into a category. E.g. building=school, amenity=restaurant and tourism=attraction
   * are all POIs, but don't fit neatly into a single hierarchy.
   *
   * If a category is set on an ancestor then it is also set in the category tag.
   */
  QStringList categories;
  uint16_t geometries;
};


class OsmSchemaData;
class Relation;

/**
 * This class is reentrant, but not thread safe.
 */
class OsmSchema
{
public:

  static QString errorCircularKey() { return "error:circular"; }

  OsmSchema();

  virtual ~OsmSchema();

  void addAssociatedWith(QString name1, QString name2);

  void addIsA(QString name1, QString name2);

  void addSimilarTo(QString name1, QString name2, double weight);

  QString average(const QString& kvp1, double w1, const QString& kvp2, double w2, double& best);

  QString average(const QString& kvp1, const QString& kvp2, double& best);

  /**
   * ONLY FOR UNIT TESTING. Be a good neighbor and call loadDefault() when you're done.
   */
  void createTestingGraph();

  /**
   * Searches for the first common ancestor between two key value pairs. If there is no common
   * ancestor then an empty TagVertex is returned.
   */
  const TagVertex& getFirstCommonAncestor(const QString& kvp1, const QString& kvp2);

  vector<TagVertex> getAssociatedTags(QString name);

  OsmSchemaCategory getCategories(const Tags& t) const;
  OsmSchemaCategory getCategories(const QString& k, const QString& v) const;
  OsmSchemaCategory getCategories(const QString& kvp) const;

  vector<TagVertex> getChildTags(QString name);

  static OsmSchema& getInstance();

  double getIsACost() const;

  vector<TagVertex> getTagByCategory(OsmSchemaCategory c) const;

  const TagVertex& getTagVertex(const QString& kvp) const;

  /**
   * Returns true if at least one tag in the set of specified tags is part of the specified
   * category.
   */
  bool hasCategory(const Tags& t, const QString& category) const;

  /**
   * Returns true if the specified kvp is part of the specified category.
   */
  bool hasCategory(const QString& kvp, const QString& category) const;

  bool isAncestor(const QString& childKvp, const QString& parentKvp);

  bool isArea(const Tags& t, ElementType type) const;
  bool isArea(const ConstElementPtr& e) const;

  bool isAreaForStats(const Tags& t, ElementType type) const;
  bool isAreaForStats(const ConstElementPtr& e) const;

  bool isBuilding(const Tags& t, ElementType type) const;
  bool isBuilding(const ConstElementPtr& e) const;

  /**
   * Returns true if this is a building:part. This is mutually exclusive with isBuilding.
   */
  bool isBuildingPart(const Tags& t, ElementType type) const;

  /**
   * Returns true if the element is a highway type (e.g. road, primary, path, etc.)
   *
   * This is not an exhaustive list, be sure and check the function to make sure it will do what
   * you expect in your instance.
   */
  bool isLinearHighway(const Tags& t, ElementType type);

  /**
   * Returns true if the element is a linear object (e.g. road, etc.)
   *
   * This is not an exhaustive list, be sure and check the function to make sure it will do what
   * you expect in your instance.
   */
  bool isLinear(const Element& e);

  /**
   */
  bool isLinearWaterway(const Element &e);

  /**
   * Returns true if this is a list of values. Right now this just looks for a semicolon in value,
   * but in the future the list of valid list keys may be stored in the schema file.
   */
  bool isList(const QString& key, const QString& value);

  /**
   * Returns true if the kvp contains metadata about the feature as opposed to real information
   * about the features.
   *
   * Tags such as "hoot:status", "source", "uuid" are all metadata.
   *
   * Tags such as "name", "highway" and "height" are not metadata.
   */
  bool isMetaData(const QString& key, const QString& value);

  bool isMultiLineString(const Relation& r) const;

  bool isPoi(const Element& e);

  /**
   * Return true if this tag can contain free-form text.
   */
  bool isTextTag(const QString& key) { return getTagVertex(key).valueType == Text; }

  /**
   * Loads the default configuration. This should only be used by unit tests.
   */
  void loadDefault();

  double score(const QString& kvp1, const QString& kvp2);

  /**
   * Sets the cost when traversing up the tree to a parent node. This is useful for strict score
   * checking rather than equivalent tags.
   */
  void setIsACost(double cost);

  QString toGraphvizString() const;

  static QString toKvp(const QString& key, const QString& value);

  void update();

  void updateOrCreateVertex(const TagVertex& tv);

private:
  // the templates we're including take a crazy long time to include, so I'm isolating the
  // implementation.
  OsmSchemaData* d;
  static OsmSchema* _theInstance;
  TagVertex _empty;
};

}

#endif // OSMSCHEMA_H
