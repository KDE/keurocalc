/***************************************************************************
                      preferences.cpp  -  preferences widget
                             -------------------
    begin                : lun avr 12 18:25:02 CET 2004
    copyright            : (C) 2001-2004 by Éric Bischoff
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

#include <qbuttongroup.h>
#include <qcombobox.h>

#include <klocale.h>

#include "preferences.h"
#include "preferences.moc"
#include "keurocalc.h"
#include "currencies.h"

extern currencyStruc currency[CURRENCIES];

Preferences::Preferences(KEuroCalc *parent, const char *name)
	: SettingsDialog( parent, name )
{
	referenceGroup->setButton( parent->getReference() );	

	for (int num = 0; num < CURRENCIES; num++)
		defaultCurrencyList->insertItem
			( QString::fromUtf8( currency[num].code ) +
			  QString::fromUtf8( " - " ) +
			  i18n( currency[num].name )
			);
	defaultCurrencyList->setCurrentItem( parent->getCurrencyNum() );
}

Preferences::~Preferences()
{
}

void Preferences::ok()
{
	KEuroCalc *calc = (KEuroCalc *) parentWidget();

	calc->setPreferences
		( referenceGroup->selectedId(),
	 	  defaultCurrencyList->currentItem()
		);
	close();
}

void Preferences::cancel()
{
	close();
}
