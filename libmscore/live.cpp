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

#include <QProcess>
#include "score.h"


namespace Ms {


//---------------------------------------------------------
//   liveSync
//    sync score with hg repo
//---------------------------------------------------------



void Score::liveCommit()
{
      QStringList params;
      params << "commit" << "-m" << "'Autosync'";
      local.start("hg", params);
      local.waitForFinished(-1);
      qDebug(local.readAllStandardError());
      qDebug(local.readAllStandardOutput());
}

void Score::livePull()
{
      pull.start("hg", QStringList("pull"));
//      pull.waitForFinished(-1);
      qDebug(pull.readAllStandardError());
      qDebug(pull.readAllStandardOutput());
}

void Score::liveMerge()
{
      local.start("hg", QStringList("merge"));
      local.waitForFinished(-1);
      qDebug(local.readAllStandardError());
      qDebug(local.readAllStandardOutput());

      local.start("hg", QStringList("update"));
      local.waitForFinished(-1);
      qDebug(local.readAllStandardError());
      qDebug(local.readAllStandardOutput());

      QStringList params_merge;
      params_merge << "commit" << "-m" << "'Merge distant changes'";
      local.start("hg", params_merge);
      local.waitForFinished(-1);
      qDebug(local.readAllStandardError());
      qDebug(local.readAllStandardOutput());
}

void Score::livePush()
{
      push.start("hg", QStringList("push"));
//      push.waitForFinished(-1);
      qDebug(push.readAllStandardError());
      qDebug(push.readAllStandardOutput());
}

bool Score::liveSync() // currently doesn't timeout hanging pulls/pushes
{
      bool receivedChanges = false;
      switch(liveState) {
            case LIVE_IDLE:
                  livePull();
                  liveState = LIVE_PULLING;
            case LIVE_PULLING:
                  if(pull.state() == QProcess::NotRunning) {
                        liveState = LIVE_PULLED;
                        }
                  else {
                        break;
                        }
            case LIVE_PULLED:
                  liveMerge();
                  livePush();
                  liveState = LIVE_PUSHING;
            case LIVE_PUSHING:
                  if(push.state() == QProcess::NotRunning) {
                        liveState = LIVE_IDLE;
                        receivedChanges = true;
                        }
                  }
      return receivedChanges;
}


}
