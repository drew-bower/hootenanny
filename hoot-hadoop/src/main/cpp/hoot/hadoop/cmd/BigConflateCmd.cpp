/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/TileConflator.h>
#include <hoot/core/util/Settings.h>
#include <hoot/hadoop/HadoopTileWorker.h>
#include <hoot/hadoop/HadoopTileWorker2.h>
#include <hoot/hadoop/PaintNodesDriver.h>
#include <hoot/hadoop/stats/MapStatsDriver.h>

// Pretty Pipes
#include <pp/mapreduce/Job.h>

// Qt
#include <QImage>

namespace hoot
{

class BigConflateCmd : public BaseCommand
{
public:

  static string className() { return "hoot::BigConflateCmd"; }

  BigConflateCmd()
  {
  }

  QString getHelp() const
  {
    // 80 columns
    //  | <---                                                                      ---> |
    return getName() + " (input1.pbf) (input2.pbf) (output.pbf) [pixelSize] \n"
        "[maxNodeCount] [--local]\n"
        "  Reads the nodes from inputs, conflates and writes the result to output.pbf.\n"
        "  * input1.pbf - Input 1 .pbf dir -- must reside on HDFS.\n"
        "  * input2.pbf - Input 2 .pbf dir -- must reside on HDFS.\n"
        "  * output.pbf - Output .pbf dir -- must reside on HDFS.\n"
        "  * pixelSize - Optional pixel size in degrees (0.1 is reasonable for global \n"
        "    data)\n"
        "  * maxNodeCount - Optional max node count.\n"
        "  * --local - Runs the job locally rather than using Hadoop.";
  }

  virtual QString getName() const { return "big-conflate"; }

  int runSimple(QStringList args)
  {
    if (args.size() < 3 || args.size() > 6)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three to six parameters.").arg(getName()));
    }
    QString in1 = args[0];
    QString in2 = args[1];
    QString out = args[2];
    bool local = args.contains("--local");
    double pixelSize = conf().getDouble(HadoopTileWorker2::pixelSizeKey());
    int maxNodeCount = conf().getInt(HadoopTileWorker2::maxNodeCountKey());
    if (args.size() >= 4)
    {
      bool ok;
      pixelSize = QString(args[3]).toDouble(&ok);
      if (ok == false)
      {
        cout << getHelp() << endl << endl;
        throw HootException("Error parsing pixelSize.");
      }
    }
    if (args.size() >= 5)
    {
      bool ok;
      maxNodeCount = QString(args[4]).toInt(&ok);
      if (ok == false)
      {
        cout << getHelp() << endl << endl;
        throw HootException("Error parsing maxNodeCount.");
      }
    }

    if (local)
    {
      pp::Job::setDefaultJobTracker("local");
    }

    shared_ptr<TileWorker> worker(new HadoopTileWorker());
    TileConflator uut(worker);
    // ~240m
    uut.setBuffer(pixelSize);
    uut.setMaxNodesPerBox(maxNodeCount);

    uut.setSources(in1, in2);

    uut.conflate(out);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, BigConflateCmd)

}
