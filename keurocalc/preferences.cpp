/***************************************************************************
                      preferences.cpp  -  preferences widget
                             -------------------
    begin                : lun avr 12 18:25:02 CET 2004
    copyright            : (C) 2001-2008 by Éric Bischoff
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

#include <QButtonGroup>
#include <QLabel>
#include <QCheckBox>

#include <kcolorbutton.h>
#include <kcolordialog.h>
#include <klocale.h>

#include "preferences.h"
#include "preferences.moc"
#include "keurocalc.h"

// Constructor
Preferences::Preferences(KEuroCalc *parent, const Currencies *currencies)
	: QDialog(parent), Ui::SettingsDialog()
{
	int oldReference, oldCurrency, oldRounding;
	QColor oldDisplayColor;
	QPalette palette;
	bool oldSplashScreen;

	setupUi(this);
// TODO: The following should move to the ui file!
	referenceGroup = new QButtonGroup();
	referenceGroup->addButton( radioButtonEuroFixed, EURO_FIXED );
	referenceGroup->addButton( radioButtonEuroECB, EURO_ECB );
	referenceGroup->addButton( radioButtonDollarNYFRB, DOLLAR_NY_FRB );
	roundingGroup = new QButtonGroup();
	roundingGroup->addButton( radioButtonOfficialRules, OFFICIAL_RULES );
	roundingGroup->addButton( radioButtonSmallestCoin, SMALLEST_COIN );
	roundingGroup->addButton( radioButtonNoRounding, NO_ROUNDING );

	parent->readOptions( oldReference, oldCurrency, oldRounding, oldDisplayColor, oldSplashScreen );

	referenceGroup->button( oldReference )->setChecked( true );

	for (int num = 0; num < currencies->number(); num++)
		defaultCurrencyList->addItem
			( currencies->code(num) + " - " + currencies->name(num) );
	defaultCurrencyList->setCurrentRow( oldCurrency );

	roundingGroup->button( oldRounding )->setChecked( true );

	palette.setColor( backgroundRole(), oldDisplayColor );
	displayColorResult->setPalette( palette );

	checkBoxSplashScreen->setChecked( oldSplashScreen );
}

// Destructor
Preferences::~Preferences()
{
}

// OK button pressed
void Preferences::ok()
{
	KEuroCalc *calc = (KEuroCalc *) parentWidget();
	int newReference = referenceGroup->checkedId(),
	    newCurrency = defaultCurrencyList->currentRow(),
	    newRounding = roundingGroup->checkedId();
	
	QPalette palette = displayColorResult->palette();
	QColor newDisplayColor = palette.color( backgroundRole() );

	bool newSplashScreen = checkBoxSplashScreen->isChecked();

	calc->writeOptions( newReference, newCurrency, newRounding, newDisplayColor, newSplashScreen );
	calc->setPreferences( newReference, newCurrency, newRounding, newDisplayColor, newSplashScreen );

	calc->ResultDisplay->setPalette( palette );
	calc->InputDisplay->setPalette( palette );
	calc->OperatorDisplay->setPalette( palette );

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
	QPalette palette = displayColorResult->palette();
	QColor myColor;
	int result;

	result = KColorDialog::getColor
		( myColor, palette.color( backgroundRole() ) );
	if ( result == KColorDialog::Accepted )
	{
		QPalette palette;

		palette.setColor( backgroundRole(), myColor );
		displayColorResult->setPalette( palette );
	}
}
