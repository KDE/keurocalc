/***************************************************************************
                          keurocalc.cpp  -  main widget
                             -------------------
    begin                : sam déc  1 23:40:19 CET 2001
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

#include <stdlib.h>

#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qdom.h>

#include <kapp.h>
#include <kaboutapplication.h>
#include <kglobal.h>
#include <kconfig.h>

#include <math.h>

#include "keurocalc.h"
#include "keurocalc.moc"
#include "preferences.h"
#include "currencies.h"

extern currencyStruc currency[CURRENCIES];

static const char
	*euroSymbol = " â‚¬",
	*dollarSymbol = " $",
	*urlECB = "http://www.ecb.int/stats/eurofxref/eurofxref-daily.xml",
	*urlNY_FRB = "http://www.ny.frb.org/markets/fxrates/FXtoXML.cfm?FEXdate=%04d-%02d-%02d&FEXtime=1200";

// Constructor
KEuroCalc::KEuroCalc(QWidget *parent, const char *name)
	: Calculator( parent, name ),
	  variableRates()
{
	int position;

	isSimpleValue = false;
	simpleValue = 0.0;
	referenceValue = 0.0;
	currencyValue = 0.0;

	memorySet = false;
	isSimpleMemory = false;
	simpleMemory = 0.0;
	referenceMemory = 0.0;

	readOptions( reference, currencyNum, rounding );

	displayNewCurrency();
	displayNewResult();
	resetInput();
	initButtons();
	startDownload();

	position = currency[currencyNum].position;
	if (position < 0) position = 0;
	CurrencyList->setCurrentItem( position );

	setFixedSize(size());
}

// Destructor
KEuroCalc::~KEuroCalc()
{
}

// Read options from preferences file
void KEuroCalc::readOptions(int &oldReference, int &oldCurrency, int &oldRounding )
{
	KConfig *config;
	QString option;

	config = KApplication::kApplication()->config();
	config->setGroup("General");

	option = config->readEntry("Reference", "EURO_ECB");
	if (option == "EURO_FIXED")
		oldReference = EURO_FIXED;
	else if (option == "EURO_ECB")
		oldReference = EURO_ECB;
	else if (option == "DOLLAR_NY_FRB")
		oldReference = DOLLAR_NY_FRB;
	else oldReference = EURO_ECB;

	option = config->readEntry("Currency", "USD");
	for (oldCurrency = 0; oldCurrency < CURRENCIES; oldCurrency++)
		if ( option == QString::fromUtf8( currency[oldCurrency].code ) )
			break;
	if ( oldCurrency == CURRENCIES ) oldCurrency = DOLLAR_CURRENCY;

	option = config->readEntry("Rounding", "OFFICIAL_RULES");
	if (option == "OFFICIAL_RULES")
		oldRounding = OFFICIAL_RULES;
	else if (option == "SMALLEST_COIN")
		oldRounding = SMALLEST_COIN;
	else if (option == "NO_ROUNDING")
		oldRounding = NO_ROUNDING;
	else oldRounding = OFFICIAL_RULES;
}

// Write options to preferences file
void KEuroCalc::writeOptions(int newReference, int newCurrency, int newRounding)
{
	KConfig *config;

	config = KApplication::kApplication()->config();
	config->setGroup("General");

	switch (newReference)
	{	case EURO_FIXED:
			config->writeEntry("Reference", "EURO_FIXED");
			break;
		case EURO_ECB:
			config->writeEntry("Reference", "EURO_ECB");
			break;
		case DOLLAR_NY_FRB:
			config->writeEntry("Reference", "DOLLAR_NY_FRB");
	}

	config->writeEntry("Currency", QString::fromUtf8(currency[newCurrency].code) );

	switch (newRounding)
	{	case OFFICIAL_RULES:
			config->writeEntry("Rounding", "OFFICIAL_RULES");
			break;
		case SMALLEST_COIN:
			config->writeEntry("Rounding", "SMALLEST_COIN");
			break;
		case NO_ROUNDING:
			config->writeEntry("Rounding", "NO_ROUNDING");
	}

	config->sync();
}

// Set new preferences dialog
void KEuroCalc::setPreferences(int newReference, int newCurrency, int newRounding)
{
	reference = newReference;
	currencyNum = newCurrency;
	rounding = newRounding;

	CurrencyList->clear();
	initButtons();
	startDownload();
	reset();
}

// Handle key press events
void KEuroCalc::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
		case Key_Period:
		case Key_Comma:
			inputDigit('.');
			break;
		case Key_0:
			ZeroButton->animateClick();
			break;
		case Key_1:
			OneButton->animateClick();
			break;
		case Key_2:
			TwoButton->animateClick();
			break;
		case Key_3:
			ThreeButton->animateClick();
			break;
		case Key_4:
			FourButton->animateClick();
			break;
		case Key_5:
			FiveButton->animateClick();
			break;
		case Key_6:
			SixButton->animateClick();
			break;
		case Key_7:
			SevenButton->animateClick();
			break;
		case Key_8:
			EightButton->animateClick();
			break;
		case Key_9:
			NineButton->animateClick();
			break;
		case Key_Plus:
			PlusButton->animateClick();
			break;
		case Key_Minus:
			MinusButton->animateClick();
			break;
		case Key_Asterisk:
			AsteriskButton->animateClick();
			break;
		case Key_Slash:
			SlashButton->animateClick();
			break;
		case Key_Backspace:
			BackspaceButton->animateClick();
			break;
		case Key_E:
			ReferenceButton->animateClick();
			break;
		case Key_Percent:
			PercentButton->animateClick();
			break;
		case Key_Enter:
		case Key_Return:
			SimpleValueButton->animateClick();
			break;
		case Key_S:
			PlusMinusButton->animateClick();
			break;
		case Key_Shift:
		case Key_Control:
		case Key_Meta:
		case Key_Alt:
		case Key_CapsLock:
		case Key_NumLock:
		case Key_ScrollLock:
			break;
		default:
			CurrencyButton->animateClick();
			break;
	}
}

// Add fixed rates
void KEuroCalc::addFixedRates()
{
	int num, position;
	double currencyPrecision;

	position = 0;
	for (num = 0; num < CURRENCIES; num++)
		if ( currency[num].fixedRate )
	{
		switch (rounding)
		{
			case OFFICIAL_RULES:
				currencyPrecision = currency[num].officialRulesPrecision;
				break;
			case SMALLEST_COIN:
				currencyPrecision = currency[num].smallestCoinPrecision;
				break;
			default:
				currencyPrecision = 1.0;
		}
		currency[num].rate =
			currency[num].fixedRate / currencyPrecision;
		currency[num].position = position++;
		CurrencyList->insertItem
			( QString::fromUtf8( currency[num].code ) +
			  QString::fromUtf8( " - " ) +
			  i18n( currency[num].name )
			);
	}
}

// Exchange rates received in an HTTP data stream
void KEuroCalc::httpData(KIO::Job *job, const QByteArray &array)
{
	job = 0; // Unused parameter
	variableRates += QString(array);
}

// Finished receiving data from European Central Bank
void KEuroCalc::httpResultECB(KIO::Job *job)
{
	QDomDocument document( "rates" );
	document.setContent( variableRates, true );
	QDomNodeList ratesList = document.elementsByTagName( "Cube" );
	bool date = false;
	int num, position;
	double currencyPrecision;

	job = 0; // Unused parameter

	for (uint i = 0; i < ratesList.count(); i++)
	{
		QDomElement elt = (const QDomElement &) ratesList.item(i);
		if ( elt.hasAttribute( "time" ) )
		{
			DateLabel->setText( elt.attribute( "time" ) );
			date = true;
		}
		else if ( elt.hasAttribute( "currency" ) && elt.hasAttribute( "rate" ) )
		{
			QString code(elt.attribute( "currency" ));
			for (num = 0; num < CURRENCIES; num++)
				if ( code == QString::fromUtf8( currency[num].code ) )
					break;
			if ( num < CURRENCIES )
			{
				switch (rounding)
				{
					case OFFICIAL_RULES:
						currencyPrecision = currency[num].officialRulesPrecision;
						break;
					case SMALLEST_COIN:
						currencyPrecision = currency[num].smallestCoinPrecision;
						break;
					default:
						currencyPrecision = 1.0;
				}
				currency[num].rate =
					elt.attribute( "rate" ).toDouble() / currencyPrecision;
				currency[num].position = -2;
			}
		}
	}
	position = FIXED_CURRENCIES + 1;
	for (num = 0; num < CURRENCIES; num++)
		if (currency[num].position == -2)
	{
		currency[num].position = position++;
		CurrencyList->insertItem
			( QString::fromUtf8( currency[num].code ) +
			  " - " +
			  i18n( currency[num].name ) );
	}
	if ( !date )
		DateLabel->setText( i18n( "Not loaded" ) );
	variableRates = "";
	newRatesList( EURO_CURRENCY );
}

// Finished receiving data from New York Federal Reserve Bank
void KEuroCalc::httpResultNY_FRB(KIO::Job *job)
{
	QDomDocument document( "rates" );
	document.setContent( variableRates, true );
	QDomNodeList ratesList = document.elementsByTagName( "Rate" );
	bool date = false;
	int num, position;
	double currencyPrecision;

	job = 0; // Unused parameter

	for (uint i = 0; i < ratesList.count(); i++)
	{
		QDomElement elt = (const QDomElement &) ratesList.item(i);
		QDomNodeList dateElements = elt.elementsByTagName( "Date" ),
			     countryElements = elt.elementsByTagName( "Country" ),
			     valueElements = elt.elementsByTagName( "Value" );
		if (dateElements.count() == 1 && countryElements.count() == 1 && valueElements.count() == 1)
		{
			QDomElement dateElement = (const QDomElement &) dateElements.item(0),
				    countryElement = (const QDomElement &) countryElements.item(0),
				    valueElement = (const QDomElement &) valueElements.item(0);
			QString newYorkName( countryElement.attribute( "UnitName" ) + "/" + countryElement.text() );
			for (num = 0; num < CURRENCIES; num++)
				if ( newYorkName == currency[num].newYorkName )
					break;
			if ( num < CURRENCIES )
			{
				switch (rounding)
				{
					case OFFICIAL_RULES:
						currencyPrecision = currency[num].officialRulesPrecision;
						break;
					case SMALLEST_COIN:
						currencyPrecision = currency[num].smallestCoinPrecision;
						break;
					default:
						currencyPrecision = 1.0;
				}
				if ( valueElement.attribute( "InUsd" ) == "false" )
					currency[num].rate =
						valueElement.text().toDouble() / currencyPrecision;
				else currency[num].rate =
					( 1.0 / valueElement.text().toDouble() ) / currencyPrecision;
				currency[num].position = -2;
				if ( !date )
				{
					DateLabel->setText( dateElement.text() );
					date = true;
				}
			}
		}
	}
	position = 0;
	for (num = 0; num < CURRENCIES; num++)
		if (currency[num].position == -2)
	{
		currency[num].position = position++;
		CurrencyList->insertItem
			( QString::fromUtf8( currency[num].code ) +
			  " - " +
			  i18n( currency[num].name ) );
	}
	if ( !date )
		DateLabel->setText( i18n( "Not loaded" ) );
	variableRates = "";
	newRatesList( DOLLAR_CURRENCY );
}

// Dot button or key pressed
void KEuroCalc::inputDot()
{
	inputDigit('.');
}

// Zero button or key pressed
void KEuroCalc::inputZero()
{
	inputDigit('0');
}

// One button or key pressed
void KEuroCalc::inputOne()
{
	inputDigit('1');
}

// Two button or key pressed
void KEuroCalc::inputTwo()
{
	inputDigit('2');
}

// Three button or key pressed
void KEuroCalc::inputThree()
{
	inputDigit('3');
}

// Four button or key pressed
void KEuroCalc::inputFour()
{
	inputDigit('4');
}

// Five button or key pressed
void KEuroCalc::inputFive()
{
	inputDigit('5');
}

// Six button or key pressed
void KEuroCalc::inputSix()
{
	inputDigit('6');
}

// Seven button or key pressed
void KEuroCalc::inputSeven()
{
	inputDigit('7');
}

// Eight button or key pressed
void KEuroCalc::inputEight()
{
	inputDigit('8');
}

// Nine button or key pressed
void KEuroCalc::inputNine()
{
	inputDigit('9');
}

// Plus button or key pressed
void KEuroCalc::inputPlus()
{
	inputOperator('+');
}

// Minus button or key pressed
void KEuroCalc::inputMinus()
{
	inputOperator('-');
}

// Multiply button or key pressed
void KEuroCalc::inputAsterisk()
{
	inputOperator('x');
}

// Divide button or key pressed
void KEuroCalc::inputSlash()
{
	inputOperator('/');
}

// Correct last entered digit
void KEuroCalc::inputBackspace()
{
	inputCorrect();
}

// Convert to euro, and add or substract too if needed
void KEuroCalc::validateReference()
{
	double inputValue, currencyRate, currencyPrecision;

	inputValue = atof( inputDisplay );
	currencyRate = currency[currencyNum].rate;
	switch (rounding)
	{
		case OFFICIAL_RULES:
			currencyPrecision = currency[currencyNum].officialRulesPrecision;
			break;
		case SMALLEST_COIN:
			currencyPrecision = currency[currencyNum].smallestCoinPrecision;
			break;
		default:
			currencyPrecision = 1.0;
	}
	if ( inputDisplay[10] == ' ' )
	{
		if ( isSimpleValue && *operatorDisplay == ' ' )
		{
			isSimpleValue = false;
			referenceValue = simpleValue;
			currencyValue = referenceValue * currencyRate * currencyPrecision;
			displayNewResult();
		}
		else KApplication::beep();
		return;
	}

	switch ( *operatorDisplay )
	{
		case ' ':
			isSimpleValue = false;
			referenceValue = inputValue;
			currencyValue = referenceValue * currencyRate * currencyPrecision;
			break;
		case '+':
			if ( isSimpleValue )
			{
				KApplication::beep();
				return;
			}
			referenceValue += inputValue;
			currencyValue = referenceValue * currencyRate * currencyPrecision;
			break;
		case '-':
			if ( isSimpleValue )
			{
				KApplication::beep();
				return;
			}
			referenceValue -= inputValue;
			currencyValue = referenceValue * currencyRate * currencyPrecision;
			break;
		case 'x':
			if ( isSimpleValue )
			{
				isSimpleValue = false;
				referenceValue = simpleValue * inputValue;
				currencyValue = referenceValue * currencyRate * currencyPrecision;
			}
			else
			{
				KApplication::beep();
				return;
			}
			break;
		case '/':
			if ( !isSimpleValue && inputValue != 0.0 )
			{
				isSimpleValue = true;
				simpleValue = referenceValue / inputValue;
			}
			else
			{
				KApplication::beep();
				return;
			}
	}
	resetInput();
	displayNewResult();
	displayMemoryButtons();
}

// Convert to currency, and add or substract too if needed
void KEuroCalc::validateCurrency()
{
	double inputValue, currencyRate, currencyPrecision;

	inputValue = atof( inputDisplay );
	currencyRate = currency[currencyNum].rate;
	switch (rounding)
	{
		case OFFICIAL_RULES:
			currencyPrecision = currency[currencyNum].officialRulesPrecision;
			break;
		case SMALLEST_COIN:
			currencyPrecision = currency[currencyNum].smallestCoinPrecision;
			break;
		default:
			currencyPrecision = 1.0;
	}

	if ( inputDisplay[10] == ' ' )
	{
		if ( isSimpleValue && *operatorDisplay == ' ' )
		{
			isSimpleValue = false;
			currencyValue = simpleValue;
			referenceValue = currencyValue / currencyRate / currencyPrecision;
			displayNewResult();
		}
		else KApplication::beep();
		return;
	}

	switch ( *operatorDisplay )
	{
		case ' ':
			isSimpleValue = false;
			currencyValue = inputValue;
			referenceValue = currencyValue / currencyRate / currencyPrecision;
			break;
		case '+':
			if ( isSimpleValue )
			{
				KApplication::beep();
				return;
			}
			currencyValue += inputValue;
			referenceValue = currencyValue / currencyRate / currencyPrecision;
			break;
		case '-':
			if ( isSimpleValue )
			{
				KApplication::beep();
				return;
			}
			currencyValue -= inputValue;
			referenceValue = currencyValue / currencyRate / currencyPrecision;
			break;
		case 'x':
			if ( isSimpleValue )
			{
				isSimpleValue = false;
				currencyValue = simpleValue * inputValue;
				referenceValue = currencyValue / currencyRate / currencyPrecision;
			}
			else
			{
				KApplication::beep();
				return;
			}
			break;
		case '/':
			if ( !isSimpleValue && inputValue != 0.0 )
			{
				isSimpleValue = true;
				simpleValue = currencyValue / inputValue;
			}
			else
			{
				KApplication::beep();
				return;
			}
	}
	resetInput();
	displayNewResult();
	displayMemoryButtons();
}

// Apply percentage, and add or substract too if needed
void KEuroCalc::validatePercent()
{
	double inputValue, currencyRate, currencyPrecision;

	inputValue = atof( inputDisplay );
	currencyRate = currency[currencyNum].rate;
	switch (rounding)
	{
		case OFFICIAL_RULES:
			currencyPrecision = currency[currencyNum].officialRulesPrecision;
			break;
		case SMALLEST_COIN:
			currencyPrecision = currency[currencyNum].smallestCoinPrecision;
			break;
		default:
			currencyPrecision = 1.0;
	}

	if ( inputDisplay[10] == ' ' )
	{
		KApplication::beep();
		return;
	}

	switch ( *operatorDisplay )
	{
		case ' ':
			if ( isSimpleValue )
				simpleValue = (inputValue * simpleValue) / 100.0;
			else
			{
				referenceValue = (inputValue * referenceValue) / 100.0;
				currencyValue = referenceValue * currencyRate * currencyPrecision;
			}
			break;
		case '+':
			if ( isSimpleValue )
				simpleValue = ((100.0 + inputValue) * simpleValue) / 100.0;
			else
			{
				referenceValue = ((100.0 + inputValue) * referenceValue) / 100.0;
				currencyValue = referenceValue * currencyRate * currencyPrecision;
			}
			break;
		case '-':
			if ( isSimpleValue )
				simpleValue = ((100.0 - inputValue) * simpleValue) / 100.0;
			else
			{
				referenceValue = ((100.0 - inputValue) * referenceValue) / 100.0;
				currencyValue = referenceValue * currencyRate * currencyPrecision;
			}
			break;
		case 'x':
		case '/':
			KApplication::beep();
			return;
	}
	resetInput();
	displayNewResult();
}

// Enter a simple value, and multiply or divide too if needed.
void KEuroCalc::validateSimpleValue()
{
	double inputValue, currencyRate, currencyPrecision;

	inputValue = atof( inputDisplay );
	currencyRate = currency[currencyNum].rate;
	switch (rounding)
	{
		case OFFICIAL_RULES:
			currencyPrecision = currency[currencyNum].officialRulesPrecision;
			break;
		case SMALLEST_COIN:
			currencyPrecision = currency[currencyNum].smallestCoinPrecision;
			break;
		default:
			currencyPrecision = 1.0;
	}

	if ( inputDisplay[10] == ' ' )
	{
		KApplication::beep();
		return;
	}

	switch ( *operatorDisplay )
	{
		case ' ':
			isSimpleValue = true;
			simpleValue = inputValue;
			break;
		case '+':
			if ( isSimpleValue )
				simpleValue += inputValue;
			else
			{
				KApplication::beep();
				return;
			}
			break;
		case '-':
			if ( isSimpleValue )
				simpleValue -= inputValue;
			else
			{
				KApplication::beep();
				return;
			}
			break;
		case 'x':
			if ( isSimpleValue )
				simpleValue *= inputValue;
			else
			{
				referenceValue *= inputValue;
				currencyValue = referenceValue * currencyRate * currencyPrecision;
			}
			break;
		case '/':
			if ( inputValue == 0.0 )
			{
				KApplication::beep();
				return;
			}
			if ( isSimpleValue )
				simpleValue /= inputValue;
			else
			{
				referenceValue /= inputValue;
				currencyValue = referenceValue * currencyRate * currencyPrecision;
			}
			break;
		default:
			KApplication::beep();
			return;
	}
	resetInput();
	displayNewResult();
	displayMemoryButtons();
}

// Change the sign of the result
void KEuroCalc::changeSign()
{
	double currencyRate, currencyPrecision;

	currencyRate = currency[currencyNum].rate;
	switch (rounding)
	{
		case OFFICIAL_RULES:
			currencyPrecision = currency[currencyNum].officialRulesPrecision;
			break;
		case SMALLEST_COIN:
			currencyPrecision = currency[currencyNum].smallestCoinPrecision;
			break;
		default:
			currencyPrecision = 1.0;
	}

	if ( isSimpleValue )
		simpleValue = -simpleValue;
	else
	{
		referenceValue = -referenceValue;
		currencyValue = referenceValue * currencyRate * currencyPrecision;
	}
	displayNewResult();
}

// Tranfer from display to memory
void KEuroCalc::memoryInput()
{
	memorySet = true;
	isSimpleMemory = isSimpleValue;
	simpleMemory = simpleValue;
	referenceMemory = referenceValue;

	displayMemoryButtons();
}

// Transfer from memory to display
void KEuroCalc::memoryRecall()
{
	double currencyRate, currencyPrecision;

	currencyRate = currency[currencyNum].rate;
	switch (rounding)
	{
		case OFFICIAL_RULES:
			currencyPrecision = currency[currencyNum].officialRulesPrecision;
			break;
		case SMALLEST_COIN:
			currencyPrecision = currency[currencyNum].smallestCoinPrecision;
			break;
		default:
			currencyPrecision = 1.0;
	}

	if ( !memorySet )
	{
		KApplication::beep();
		return;
	}

	switch ( *operatorDisplay )
	{
		case ' ':
			isSimpleValue = isSimpleMemory;
			if ( isSimpleMemory )
				simpleValue = simpleMemory;
			else
			{
				referenceValue = referenceMemory;
				currencyValue = referenceValue * currencyRate * currencyPrecision;
			}
			break;
		case '+':
			if ( isSimpleValue != isSimpleMemory )
			{
				KApplication::beep();
				return;
			}
			if ( isSimpleMemory )
				simpleValue += simpleMemory;
			else
			{
				referenceValue += referenceMemory;
				currencyValue = referenceValue * currencyRate * currencyPrecision;
			}
			break;
		case '-':
			if ( isSimpleValue != isSimpleMemory )
			{
				KApplication::beep();
				return;
			}
			if ( isSimpleMemory )
				simpleValue -= simpleMemory;
			else
			{
				referenceValue -= referenceMemory;
				currencyValue = referenceValue * currencyRate * currencyPrecision;
			}
			break;
		case 'x':
			if ( isSimpleMemory )
			{
				if ( isSimpleValue )
					simpleValue *= simpleMemory;
				else
				{
					referenceValue *= simpleMemory;
					currencyValue = referenceValue * currencyRate * currencyPrecision;
				}
			}
			else
			{
				if ( isSimpleValue )
				{
					isSimpleValue = false;
					referenceValue = simpleValue * referenceMemory;
					currencyValue = referenceValue * currencyRate * currencyPrecision;
				}
				else
				{
					KApplication::beep();
					return;
				}
			}
			break;
		case '/':
			if ( isSimpleMemory )
			{
				if ( simpleMemory == 0.0 )
				{
					KApplication::beep();
					return;
				}
				if ( isSimpleValue )
					simpleValue /= simpleMemory;
				else
				{
					referenceValue /= simpleMemory;
					currencyValue = referenceValue * currencyRate * currencyPrecision;
				}
			}
			else
			{
				if ( !isSimpleValue && referenceMemory != 0.0 )
				{
					isSimpleValue = true;
					simpleValue = referenceValue / referenceMemory;
				}
				else
				{
					KApplication::beep();
					return;
				}
			}
	}
	resetInput();
	displayNewResult();
	displayMemoryButtons();
}

// Add memory to display
void KEuroCalc::memoryPlus()
{
	if ( !memorySet || (isSimpleValue != isSimpleMemory) )
	{
		KApplication::beep();
		return;
	}
	if ( isSimpleMemory )
		simpleMemory += simpleValue;
	else
		referenceMemory += referenceValue;
}

// Substract memory from display
void KEuroCalc::memoryMinus()
{
	if ( !memorySet || (isSimpleValue != isSimpleMemory) )
	{
		KApplication::beep();
		return;
	}
	if ( isSimpleMemory )
		simpleMemory -= simpleValue;
	else
		referenceMemory -= referenceValue;
}

// Reset both input, result and memory
void KEuroCalc::reset()
{
	resetInput();

	isSimpleValue = false;
	simpleValue = 0.0;
	referenceValue = 0.0;
	currencyValue = 0.0;

	displayNewResult();

	memorySet = false;
	isSimpleMemory = false;
	simpleMemory = 0.0;
	referenceMemory = 0.0;

	displayMemoryButtons();
}

// Display "about" page
void KEuroCalc::displayAbout()
{
	KAboutApplication *d = new KAboutApplication;
	d->exec();
	delete d;
	AboutButton->setDown( false );
}

// Display help pages
void KEuroCalc::displayHelp()
{
	kapp->invokeHelp();
}

// Display settings pages
void KEuroCalc::displaySettings()
{
	Preferences *d = new Preferences(this);
	d->exec();
	delete d;
	SettingsButton->setDown( false );
}

// Select currency accordingly to drop down list position
void KEuroCalc::selectCurrency(int position)
{
	int num;
	double currencyRate, currencyPrecision;

	for (num = 0; num < CURRENCIES; num++)
		if (currency[num].position == position)
			break;
	if ( num == CURRENCIES ) return;
	currencyNum = num;

	displayNewCurrency();

	currencyRate = currency[currencyNum].rate;
	switch (rounding)
	{
		case OFFICIAL_RULES:
			currencyPrecision = currency[currencyNum].officialRulesPrecision;
			break;
		case SMALLEST_COIN:
			currencyPrecision = currency[currencyNum].smallestCoinPrecision;
			break;
		default:
			currencyPrecision = 1.0;
	}
	currencyValue = referenceValue * currencyRate * currencyPrecision;
	displayNewResult();
}

// Initialize the buttons without translation and the buttons with UTF-8
// text not correctly handled by uic
void KEuroCalc::initButtons()
{
	switch ( reference )
	{
		case EURO_FIXED:
			SourceLabel->setText( i18n( "Fixed" ) );
			DateLabel->setText( "" );
			break;
		case EURO_ECB:
			SourceLabel->setText( i18n( "ECB" ) );
			DateLabel->setText( i18n( "Loading..." ) );
			break;
		case DOLLAR_NY_FRB:
			SourceLabel->setText( i18n( "NY FRB" ) );
			DateLabel->setText( i18n( "Loading..." ) );
	}
	switch ( rounding )
	{
		case OFFICIAL_RULES:
			RoundingLabel->setText( i18n( "Off. rules" ) );
			break;
		case SMALLEST_COIN:
			RoundingLabel->setText( i18n( "Smallest coin" ) );
			break;
		case NO_ROUNDING:
			RoundingLabel->setText( i18n( "No rounding" ) );
	}
	DotButton->setText( KGlobal::locale()->decimalSymbol() );
	ZeroButton->setText( QString::fromUtf8( "0" ) );
	OneButton->setText( QString::fromUtf8( "1" ) );
	TwoButton->setText( QString::fromUtf8( "2" ) );
	ThreeButton->setText( QString::fromUtf8( "3" ) );
	FourButton->setText( QString::fromUtf8( "4" ) );
	FiveButton->setText( QString::fromUtf8( "5" ) );
	SixButton->setText( QString::fromUtf8( "6" ) );
	SevenButton->setText( QString::fromUtf8( "7" ) );
	EightButton->setText( QString::fromUtf8( "8" ) );
	NineButton->setText( QString::fromUtf8( "9" ) );
	BackspaceButton->setText( QString::fromUtf8( "<-" ) );

	MInputButton->setText( QString::fromUtf8( "Min" ) );
	MRecallButton->setText( QString::fromUtf8( "MR" ) );
	MMinusButton->setText( QString::fromUtf8( "M-" ) );
	MPlusButton->setText( QString::fromUtf8( "M+" ) );
	SlashButton->setText( QString::fromUtf8( "/" ) );
	AsteriskButton->setText( QString::fromUtf8( "X" ) );
	MinusButton->setText( QString::fromUtf8( "-" ) );
	PlusButton->setText( QString::fromUtf8( "+" ) );
	ReferenceButton->setText( QString::fromUtf8( reference == DOLLAR_NY_FRB? dollarSymbol: euroSymbol ) );
	PercentButton->setText( QString::fromUtf8( "%" ) );
	PlusMinusButton->setText( QString::fromUtf8( "+/-" ) );
}

// Start dowloading rates
void KEuroCalc::startDownload()
{
	int num;
	char url[128];
	QDate yesterday;
	KIO::SimpleJob *job;

	for (num = 0; num < CURRENCIES; num++)
	{
		currency[num].rate = 1.0;
		currency[num].position = -1;
	}
	switch (reference)
	{
		case EURO_FIXED:
			addFixedRates();
			newRatesList( EURO_CURRENCY );
			break;
		case EURO_ECB:
			addFixedRates();
			CurrencyList->insertItem( QString::fromUtf8
				( "--------------------------------------------" ) );
			job = KIO::get( KURL( urlECB ), true, false );
			connect( job, SIGNAL(data(KIO::Job *, const QByteArray &)),
				 this, SLOT(httpData(KIO::Job *, const QByteArray &))
				);
			connect( job, SIGNAL(result(KIO::Job *)),
				 this, SLOT(httpResultECB(KIO::Job *))
				);
			break;
		case DOLLAR_NY_FRB:
			yesterday = QDate::currentDate().addDays(-1);
			// This is suboptimal: we should guess the date of latest working day at 12:00 in New York local time
			// Or much better: use a URL that does not depend on that date...
			sprintf(url, urlNY_FRB, yesterday.year(), yesterday.month(), yesterday.day());
			job = KIO::get( KURL( url ), true, false );
			connect( job, SIGNAL(data(KIO::Job *, const QByteArray &)),
				 this, SLOT(httpData(KIO::Job *, const QByteArray &))
				);
			connect( job, SIGNAL(result(KIO::Job *)),
				 this, SLOT(httpResultNY_FRB(KIO::Job *))
				);
	}
}

// The rates list has changed, refresh the display
void KEuroCalc::newRatesList(int defaultCurrency)
{
	int position;

	position = currency[currencyNum].position;
	if (position < 0)			// If current currency does not exist in new rates list, change current currency
	{
		for (currencyNum = 0; currencyNum < CURRENCIES; currencyNum++)
		{
			position = currency[currencyNum].position;
			if (position == 0) break;
		}
		if (currencyNum == CURRENCIES)	// Handle case of empty list (no currencies at all)
		{
			currencyNum = defaultCurrency;
			currency[currencyNum].rate = 1.0;
			currency[currencyNum].position = position = 0;
			CurrencyList->insertItem
				( QString::fromUtf8( currency[currencyNum].code ) +
				  " - " +
				  i18n( currency[currencyNum].name ) );
		}
	}
	CurrencyList->setCurrentItem( position );
	displayNewCurrency();
	displayNewResult();
}

// Input a digit ('0' to '9' or '.')
void KEuroCalc::inputDigit(char c)
{
	const char *s;
	char *d;
	switch ( inputPos )
	{
		case atUnits:
			inputDisplay[10] = c;
			switch ( c )
			{
				case '.':
					inputDisplay[9] = '0';
					inputPos = afterUnits;
					break;
				case '0':
					break;
				default:
					inputPos = beforeUnits;
			}
			break;
		case beforeUnits:
			if ( inputDisplay[0] != ' ' )
			{
				KApplication::beep();
				return;
			}
			s = inputDisplay + 1;
			d = inputDisplay;
			while ( d < inputDisplay + 10 ) *d++ = *s++;
			*d = c;
			if ( c == '.' ) inputPos = afterUnits;
			break;
		case afterUnits:
			if ( inputDisplay[0] != ' ' )
			{
				KApplication::beep();
				return;
			}
			if ( c == '.' )
			{
				KApplication::beep();
				return;
			}
			s = inputDisplay + 1;
			d = inputDisplay;
			while ( d < inputDisplay + 10 ) *d++ = *s++;
			*d = c;
			break;
        }
	displayNewInput();
}

// Remove last entered digit
void KEuroCalc::inputCorrect()
{
	const char *s;
	char *d;

	switch ( inputPos )
	{
		case atUnits:
			if ( inputDisplay[10] == ' ' )
				*operatorDisplay = ' ';
			break;
		case beforeUnits:
		case afterUnits:
			s = inputDisplay + 9;
			d = inputDisplay + 10;
			while ( d > inputDisplay) *d-- = *s--;
			*d = ' ';
			break;
	}
	inputPos =
		inputDisplay[10] == ' '?
		atUnits:
		(QString(inputDisplay).find('.') == -1? beforeUnits: afterUnits);

	displayNewInput();
}

// Input an operator ('+', '-', 'x' or '/')
void KEuroCalc::inputOperator(char c)
{
	if ( inputDisplay[10] != ' '  )
		validateSimpleValue();

	*operatorDisplay = c;

	displayNewInput();
}

// Reset input area
void KEuroCalc::resetInput()
{
	strcpy(operatorDisplay, " ");
	strcpy( inputDisplay, "           " );
	inputPos = atUnits;
	OperatorDisplay->setText( operatorDisplay );
	InputDisplay->setText( inputDisplay );
}

// Display current input in input area
void KEuroCalc::displayNewInput()
{
	QString display(inputDisplay);
	normalize( display );

	OperatorDisplay->setText( operatorDisplay );
	InputDisplay->setText( display );
}

// Display latest computed value in results area
void KEuroCalc::displayNewResult()
{
	if ( isSimpleValue )
	{
		QString simpleDisplay;

		simpleDisplay.setNum( simpleValue );
		normalize( simpleDisplay );
		ResultDisplay->setText( simpleDisplay );
	}
	else
	{
		double currencyPrecision;
		float roundedReferenceValue, roundedCurrencyValue;
		QString referenceDisplay, currencyDisplay;
		char referenceSymbol[8], currencySymbol[8];

		if (rounding != NO_ROUNDING)
		{
			roundedReferenceValue = floor(referenceValue * 100.0 + 0.5) / 100.0;
			referenceDisplay.setNum( roundedReferenceValue, 'f', 2 );
		}
		else referenceDisplay.setNum( referenceValue );
		normalize( referenceDisplay );
		strcpy( referenceSymbol, reference == DOLLAR_NY_FRB ? dollarSymbol: euroSymbol );

		switch (rounding)
		{
			case OFFICIAL_RULES:
				currencyPrecision = currency[currencyNum].officialRulesPrecision;
				roundedCurrencyValue = floor(currencyValue * 100.0 / currencyPrecision + 0.5)
						     / 100.0 * currencyPrecision;
				currencyDisplay.setNum( roundedCurrencyValue, 'f', currencyPrecision < 100.0? 2: 0 );
				break;
			case SMALLEST_COIN:
				currencyPrecision = currency[currencyNum].smallestCoinPrecision;
				roundedCurrencyValue = floor(currencyValue * 100.0 / currencyPrecision + 0.5)
						     / 100.0 * currencyPrecision;
				currencyDisplay.setNum( roundedCurrencyValue, 'f', currencyPrecision < 100.0? 2: 0 );
				break;
			default:
				currencyDisplay.setNum( currencyValue );
		}
		normalize( currencyDisplay );
		strcpy( currencySymbol, currency[currencyNum].symbol );

		ResultDisplay->setText( referenceDisplay + QString::fromUtf8( referenceSymbol ) + "\n" +
					currencyDisplay + " " + QString::fromUtf8( currencySymbol ) );
	}
}

// Update the currency button accordingly to new currency
void KEuroCalc::displayNewCurrency()
{
	char referenceSymbol[8], currencySymbol[8];

	strcpy( currencySymbol, currency[currencyNum].symbol );
	CurrencyButton->setText( QString::fromUtf8( currencySymbol ) );
	if ( currency[currencyNum].position >= 0 )
	{
		double currencyRate, currencyPrecision;
		QString rate;

		currencyRate = currency[currencyNum].rate;
		switch (rounding)
		{
			case OFFICIAL_RULES:
				currencyPrecision = currency[currencyNum].officialRulesPrecision;
				break;
			case SMALLEST_COIN:
				currencyPrecision = currency[currencyNum].smallestCoinPrecision;
				break;
			default:
				currencyPrecision = 1.0;
		}
		rate.setNum( currencyRate * currencyPrecision ); 
		strcpy( referenceSymbol, reference == DOLLAR_NY_FRB ? dollarSymbol: euroSymbol );
		RateLabel->setText( "1" + QString::fromUtf8( referenceSymbol ) + " = " +
				    rate + " " + QString::fromUtf8( currencySymbol ) ); 
	}
	else RateLabel->setText( "" );
}

// Display the memory Plus and Minus buttons
void KEuroCalc::displayMemoryButtons()
{
	bool recallEnabled = memorySet,
	     sumsEnabled = memorySet && (isSimpleValue == isSimpleMemory);

	MRecallButton->setEnabled( recallEnabled );
	MMinusButton->setEnabled( sumsEnabled );
	MPlusButton->setEnabled( sumsEnabled );
}

// Normalize the display of a number
void KEuroCalc::normalize( QString &numberDisplay )
{
	int dotPos = numberDisplay.find('.'),
	    unitPos = (dotPos == -1? numberDisplay.length(): dotPos) - 3;

	if ( dotPos != -1 )
		numberDisplay.replace(	dotPos,
			 		KGlobal::locale()->decimalSymbol().length(),
					KGlobal::locale()->decimalSymbol() );

	while (unitPos > 0)
	{
		numberDisplay.insert(	unitPos,
					" ");
		unitPos -= 3;
	}
}

