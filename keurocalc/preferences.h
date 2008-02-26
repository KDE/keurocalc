/***************************************************************************
                      preferences.h  -  preferences widget
                             -------------------
    begin                : lun avr 12 18:25:02 CET 2004
    copyright            : (C) 2001-2005 by Éric Bischoff
    email                : ebischoff@nerim.net
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

class KEuroCalc;

class Preferences : public SettingsDialog
{
  Q_OBJECT 

public:
    Preferences(KEuroCalc *parent, const char *name=0);
    ~Preferences();

public slots:
    virtual void ok();
    virtual void cancel();
    virtual void changeDisplayColor();

};

#endif
