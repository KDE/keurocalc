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
#include <qlistbox.h>

#include <klocale.h>

#include "preferences.h"
#include "preferences.moc"
#include "keurocalc.h"
#include "currencies.h"

extern int numCurrencies;
extern currencyStruc *currency;

// Constructor
Preferences::Preferences(KEuroCalc *parent, const char *name)
	: SettingsDialog( parent, name )
{
	int oldReference, oldCurrency, oldRounding;

	parent->readOptions( oldReference, oldCurrency,  oldRounding );

	referenceGroup->setButton( oldReference );

	for (int num = 0; num < numCurrencies; num++)
		defaultCurrencyList->insertItem
			( currency[num].code + " - " + currency[num].name );
	defaultCurrencyList->setCurrentItem( oldCurrency );

	roundingGroup->setButton( oldRounding );
}

// Destructor
Preferences::~Preferences()
{
}

// OK button pressed
void Preferences::ok()
{
	KEuroCalc *calc = (KEuroCalc *) parentWidget();
	int newReference = referenceGroup->selectedId(),
	    newCurrency = defaultCurrencyList->currentItem(),
	    newRounding = roundingGroup->selectedId();

	calc->writeOptions( newReference, newCurrency, newRounding );
	calc->setPreferences( newReference, newCurrency, newRounding );

	close();
}

// Cancel button pressed
void Preferences::cancel()
{
	close();
}
