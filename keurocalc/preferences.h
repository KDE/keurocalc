/***************************************************************************
                      preferences.h  -  preferences widget
                             -------------------
    begin                : lun avr 12 18:25:02 CET 2004
    copyright            : (C) 2001-2019 by Éric Bischoff
    email                : bischoff@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

#include "ui_settingsdialog.h"

class KEuroCalc;
class Currencies;

class Preferences : public QDialog, public Ui::SettingsDialog
{
  Q_OBJECT 

public:
    Preferences(KEuroCalc *parent, const Currencies *currencies);
    virtual ~Preferences();

public slots:
    virtual void ok();
    virtual void cancel();
    virtual void changeDisplayColor();

// TODO: The following should move to the ui file!
private:
	class QButtonGroup *referenceGroup;
	class QButtonGroup *roundingGroup;
};

#endif
