/***************************************************************************
                      preferences.cpp  -  preferences widget
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

#include <qbuttongroup.h>
#include <qlistbox.h>

#include <kcolorbutton.h>
#include <kcolordialog.h>
#include <qlabel.h>

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
	QColor oldDisplayColor;

	parent->readOptions( oldReference, oldCurrency,  oldRounding, oldDisplayColor );

	referenceGroup->setButton( oldReference );

	for (int num = 0; num < numCurrencies; num++)
		defaultCurrencyList->insertItem
			( currency[num].code + " - " + currency[num].name );
	defaultCurrencyList->setCurrentItem( oldCurrency );

	roundingGroup->setButton( oldRounding );

	displayColorResult->setPaletteBackgroundColor( oldDisplayColor );
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

	QColor newDisplayColor;
	newDisplayColor = displayColorResult->paletteBackgroundColor();

	calc->writeOptions( newReference, newCurrency, newRounding, newDisplayColor );
	calc->setPreferences( newReference, newCurrency, newRounding, newDisplayColor );

	calc->ResultDisplay->setPaletteBackgroundColor(newDisplayColor);
	calc->InputDisplay->setPaletteBackgroundColor(newDisplayColor);
	calc->OperatorDisplay->setPaletteBackgroundColor(newDisplayColor);

	close();
}

// Cancel button pressed
void Preferences::cancel()
{
	close();
}

// Change display color button pressed
void Preferences::changeDisplayColor()
{
	QColor myColor;
	int result;

	result = KColorDialog::getColor
		( myColor, displayColorResult->paletteBackgroundColor() );
	if ( result == KColorDialog::Accepted )
		displayColorResult->setPaletteBackgroundColor( myColor );
}
