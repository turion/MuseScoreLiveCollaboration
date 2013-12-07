//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2002-2012 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

/**
 \file
 sync score for live collaborative editing
*/

#include "score.h"


namespace Ms {


//---------------------------------------------------------
//   liveSync
//    sync score with hg repo
//---------------------------------------------------------

void Score::liveSync()
{
      QProcess p;
      QStringList params;
      QString name = absoluteFilePath();
      QFileInfo fi(name);
      QString path = fi.absolutePath();
      p.setWorkingDirectory(path);
      params << "commit" << "-m" << "'Autosync'";
      p.start("hg", params);
      p.waitForFinished(-1);
      qDebug(p.readAllStandardError());
      qDebug(p.readAllStandardOutput());

      p.start("hg", QStringList("pull"));
      p.waitForFinished(-1);
      qDebug(p.readAllStandardError());
      qDebug(p.readAllStandardOutput());

      p.start("hg", QStringList("merge"));
      p.waitForFinished(-1);
      qDebug(p.readAllStandardError());
      qDebug(p.readAllStandardOutput());

      QStringList params_merge;
      params_merge << "commit" << "-m" << "'Merge distant changes'";
      p.start("hg", params_merge);
      p.waitForFinished(-1);
      qDebug(p.readAllStandardError());
      qDebug(p.readAllStandardOutput());

      p.start("hg", QStringList("push"));
      p.waitForFinished(-1);
      qDebug(p.readAllStandardError());
      qDebug(p.readAllStandardOutput());
}


}
