/***************************************************************************
                      preferences.h  -  preferences widget
                             -------------------
    begin                : lun avr 12 18:25:02 CET 2004
    copyright            : (C) 2001-2004 by �ric Bischoff
    email                : e.bischoff@noos.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KPREF_H
#define KPREF_H

#include "settingsdialog.h"
#include "keurocalc.h"

class Preferences : public SettingsDialog
{
  Q_OBJECT 

public:
    Preferences(KEuroCalc *parent=0, const char *name=0);
    ~Preferences();

public slots:
    virtual void ok();
    virtual void cancel();
};

#endif
