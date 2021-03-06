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
 * @copyright Copyright (C) 2012, 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "Log.h"

// Qt
#include <QDateTime>
#include <QDebug>

// Standard
#include <iostream>
using namespace std;

namespace hoot
{

void Log::init()
{
}

void Log::log(WarningLevel level, const string& str)
{
  if (level >= _level)
  {
    QDateTime dt = QDateTime::currentDateTime();

    // takes the form: "09:34:21.635 WARN  <str>"
    cout << dt.toString("hh:mm:ss.zzz") << " " << getLevelString(level) << "  " << str << endl;
  }
}

void Log::log(WarningLevel level, const string& str, const string& filename,
  const string& /*functionName*/, int lineNumber)
{
  if (level >= _level)
  {
    QDateTime dt = QDateTime::currentDateTime();

    // takes the form: "09:34:21.635 WARN  <filename>(<lineNumber>) - "
    cout << dt.toString("hh:mm:ss.zzz") << " " << getLevelString(level) << "  " <<
            filename << "(" << lineNumber << ")" << " - " << str << endl;
  }
}

void Log::setLevel(WarningLevel l)
{
  _level = l;
}

}
