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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapReprojector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/conflate/RubberSheet.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

/**
 * @ingroup cmd
 * @page ConvertCmd Convert Command
 * @code
--convert (input) (output)
 * @endcode
 *
 * The convert command converts from one format to another.
 *
 * @param input The input file to be cleaned. Supports common geospatial file types.
 * @param output The output file. Supports common geospatial file types.
 *
 * @sa @ref convert.ops
 */
class ConvertCmd : public BaseCommand
{
public:

  static string className() { return "hoot::ConvertCmd"; }

  ConvertCmd() { }

  virtual QString getHelp() const
  {
    return getName() + " (input) (output)\n"
        "  Convert from one format to another. This command is memory bound.\n"
        "  * input - Input (e.g. .osm file).\n"
        "  * output - Output file (e.g. .osm file).";
  }

  virtual QString getName() const { return "convert"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    // if there is both a partial reader and writer
    if (OsmMapReaderFactory::getInstance().hasPartialReader(args[0]) &&
        OsmMapWriterFactory::getInstance().hasPartialWriter(args[1]))
    {
      // stream the data rather than bulk read/write
      streamPartial(args[0], args[1]);
    }
    else
    {
      shared_ptr<OsmMap> map(new OsmMap());
      loadMap(map, args[0], true, Status::Unknown1);

      // Apply any user specified operations.
      NamedOp(ConfigOptions().getConvertOps()).apply(map);

      MapReprojector::reprojectToWgs84(map);

      saveMap(map, args[1]);
    }

    return 0;
  }

  void streamPartial(QString in, QString out)
  {
    LOG_DEBUG("Streaming data conversion.");
    shared_ptr<OsmMapReader> reader = OsmMapReaderFactory::getInstance().createReader(in);
    reader->open(in);
    shared_ptr<PartialOsmMapReader> pr = dynamic_pointer_cast<PartialOsmMapReader>(reader);
    shared_ptr<OsmMapWriter> writer = OsmMapWriterFactory::getInstance().createWriter(out);
    writer->open(out);
    shared_ptr<PartialOsmMapWriter> pw = dynamic_pointer_cast<PartialOsmMapWriter>(writer);

    pr->initializePartial();
    while (pr->hasMoreElements())
    {
      pw->writePartial(pr->readNextElement());
    }
    pr->finalizePartial();
    pw->finalizePartial();
  }
};

HOOT_FACTORY_REGISTER(Command, ConvertCmd)

}

