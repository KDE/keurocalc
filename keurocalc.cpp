/***************************************************************************
                          keurocalc.cpp  -  main widget
                             -------------------
    begin                : sam d�c  1 23:40:19 CET 2001
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

#include <stdlib.h>

#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qdom.h>

#include <kapp.h>
#include <kaboutapplication.h>
#include <klocale.h>
#include <kglobal.h>
#include <kconfig.h>

#include <math.h>

#include "keurocalc.h"
#include "keurocalc.moc"
#include "preferences.h"

#define CURRENCIES 49
#define FIXED_CURRENCIES 12
#define EURO_CURRENCY FIXED_CURRENCIES
#define DOLLAR_CURRENCY (FIXED_CURRENCIES + 1)
#define VARIABLE_CURRENCIES 37

static struct currencyStruc {
	double rate;
	const char *symbol;
	const char *code;
	double rulesPrecision;
	double smallestCoinPrecision;
	const char *name;
	const char *newYorkName;
	int position;
} currency[CURRENCIES] =
{
	{ 13.7603,  "öS", "ATS",   1.0,  10.0, I18N_NOOP("Austrian schilling"), "N/A", -1 },
	{ 40.3399,   "FB", "BEF",   1.0,  10.0, I18N_NOOP("Belgian franc"), "N/A", -1 },
	{ 1.95583,   "DM", "DEM",   1.0,   1.0, I18N_NOOP("German deutschmark"), "N/A", -1 },
	{ 1.66386,  "Pts", "ESP", 100.0, 100.0, I18N_NOOP("Spanish peseta"), "N/A", -1 },
	{ 5.94573,   "mk", "FIM",   1.0,   1.0, I18N_NOOP("Finnish markka"), "N/A", -1 },
	{ 6.55957,   "FF", "FRF",   1.0,   1.0, I18N_NOOP("French franc"), "N/A", -1 },
	{ 3.40750,   "Dr", "GRD", 100.0, 100.0, I18N_NOOP("Greek drachma"), "N/A", -1 },
	{ .787564, "£IR", "IEP",   1.0,   1.0, I18N_NOOP("Irish punt"), "N/A", -1 },
	{ 19.3627,    "L", "ITL", 100.0,1000.0, I18N_NOOP("Italian lira"), "N/A", -1 },
	{ 40.3399, "FLux", "LUF",   1.0,  10.0, I18N_NOOP("Luxembourg franc"), "N/A", -1 },
	{ 2.20371,   "Fl", "NLG",   1.0,   1.0, I18N_NOOP("Dutch gulden"), "N/A", -1 },
	{ 2.00482,  "Esc", "PTE", 100.0, 100.0, I18N_NOOP("Portuguese escudo"), "N/A", -1 },
	{     1.0," €", "EUR",   1.0,   1.0, I18N_NOOP("EU euro"), "Euro/European Monetary Union", -1 },
	{     1.0,    "$", "USD",   1.0,   1.0, I18N_NOOP("US dollar"), "N/A", -1 },
	{     1.0,   "A$", "AUD",   1.0,   1.0, I18N_NOOP("Australian dollar"), "Dollar/Australia", -1 },
	{     1.0,   "Lv", "BGN",   1.0,   1.0, I18N_NOOP("Bulgarian lev"), "N/A", -1 },
	{     1.0,   "R$", "BRL",   1.0,   1.0, I18N_NOOP("Brazilian real"), "Real/Brazil", -1 },
	{     1.0, "Can$", "CAD",   1.0,   1.0, I18N_NOOP("Canadian dollar"), "Dollar/Canada", -1 },
	{     1.0,   "FS", "CHF",   1.0,   1.0, I18N_NOOP("Swiss franc"), "Franc/Switzerland", -1 },
	{     1.0,    "Y", "CNY",   1.0,   1.0, I18N_NOOP("Chinese yuan"), "Yuan/China, P.R.", -1 },
	{     1.0,  "£C", "CYP",   1.0,   1.0, I18N_NOOP("Cyprus pound"), "N/A", -1 },
	{     1.0,    "K", "CZK",   1.0,   1.0, I18N_NOOP("Czech koruna"), "N/A", -1 },
	{     1.0,  "DKr", "DKK",   1.0,   1.0, I18N_NOOP("Danish krone"), "Krone/Denmark", -1 },
	{     1.0,  "EKr", "EEK",   1.0,   1.0, I18N_NOOP("Estonian kroon"), "N/A", -1 },
	{     1.0,   "£", "GBP",   1.0,   1.0, I18N_NOOP("Great Britain pound"), "Pound/United Kingdom", -1 },
	{     1.0,  "HK$", "HKD",   1.0,   1.0, I18N_NOOP("Hong kong dollar"), "Dollar/Hong Kong", -1 },
	{     1.0,   "Ht", "HUF", 100.0, 100.0, I18N_NOOP("Hungarian forint"), "N/A", -1 },
	{     1.0,   "Rs", "INR",   1.0,   1.0, I18N_NOOP("Indian rupee"), "Rupee/India", -1 },
	{     1.0,  "Ikr", "ISK",   1.0,   1.0, I18N_NOOP("Icelandic krona"), "N/A", -1 },
	{     1.0,   "¥", "JPY", 100.0, 100.0, I18N_NOOP("Japanese yen"), "Yen/Japan", -1 },
	{     1.0,    "W", "KRW",1000.0,1000.0, I18N_NOOP("South Korean won"), "Won/South Korea", -1 }, // should be "₩"
	{     1.0, "SLRs", "LKR",   1.0,   1.0, I18N_NOOP("Sri Lankan rupee"), "Rupee/Sri Lanka", -1 },
	{     1.0,   "lt", "LTL",   1.0,   1.0, I18N_NOOP("Latvian lats"), "N/A", -1 },
	{     1.0,   "Ls", "LVL",   1.0,   1.0, I18N_NOOP("Lithuanian litas"), "N/A", -1 },
	{     1.0,   "Lm", "MTL",   1.0,   1.0, I18N_NOOP("Maltese lira"), "N/A", -1 },
	{     1.0, "Mex$", "MXN",   1.0,   1.0, I18N_NOOP("Mexican peso"), "Peso/Mexico", -1 },
	{     1.0,   "MR", "MYR",   1.0,   1.0, I18N_NOOP("Malaysian ringgit"), "Ringgit/Malaysia", -1 },
	{     1.0,  "NKr", "NOK",   1.0,   1.0, I18N_NOOP("Norvegian krone"), "Krone/Norway", -1 },
	{     1.0,  "NZ$", "NZD",   1.0,   1.0, I18N_NOOP("New Zealand dollar"), "Dollar/New Zealand", -1 },
	{     1.0, "zł", "PLN",   1.0,   1.0, I18N_NOOP("Polish sloty"), "N/A", -1 },
	{     1.0,    "L", "ROL",  10.0,10000., I18N_NOOP("Romanian leu"), "N/A", -1 },
	{     1.0,  "SKr", "SEK",   1.0,   1.0, I18N_NOOP("Swedish krona"), "Krona/Sweden", -1 },
	{     1.0,   "S$", "SGB",   1.0,   1.0, I18N_NOOP("Singapore dollar"), "Dollar/Singapore", -1 },
	{     1.0,  "SIT", "SIT",   1.0,   1.0, I18N_NOOP("Slovenian tolar"), "N/A", -1 },
	{     1.0,   "Sk", "SKK",   1.0,   1.0, I18N_NOOP("Slovakian koruna"), "N/A", -1 },
	{     1.0,   "Bt", "THB",   1.0,   1.0, I18N_NOOP("Thailandese baht"), "Baht/Thailand", -1 },
	{     1.0,   "Bt", "TWD",   1.0,   1.0, I18N_NOOP("Taiwanese new dollar"), "N.T. Dollar/Taiwan", -1 },
	{     1.0,    "$", "VEB",1000.0,1000.0, I18N_NOOP("Venezuelian bolivar"), "Bolivar/Venezuela", -1 },
	{     1.0,    "R", "ZAR",   1.0,   1.0, I18N_NOOP("South African rand"), "Rand/South Africa", -1 }
};

static const char
	*euroSymbol = " €",
	*dollarSymbol = " $",
	*urlECB = "http://www.ecb.int/stats/eurofxref/eurofxref-daily.xml",
	*urlNYFRB = "http://www.ny.frb.org/markets/fxrates/FXtoXML.cfm?FEXdate=%04d-%02d-%02d&FEXtime=1200";

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

	readOptions();

	displayNewCurrency();
	displayNewResult();
	resetInput();
	initButtons();

	position = currency[currencyNum].position;
	if (position < 0) position = 0;
	CurrencyList->setCurrentItem( position );

	setFixedSize(size());
}

KEuroCalc::~KEuroCalc()
{
}

void KEuroCalc::setPreferences(int newReference)
{	reference = (enum referenceMode) newReference;
	writeOptions();

	CurrencyList->clear();
	for (int num = 0; num < CURRENCIES; num++)
		currency[num].position = -1;

	initButtons();
	reset();
}

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

void KEuroCalc::httpData(KIO::Job *job, const QByteArray &array)
{
	job = 0; // Unused parameter
	variableRates += QString(array);
}

void KEuroCalc::httpResultECB(KIO::Job *job)
{
	QDomDocument document( "rates" );
	document.setContent( variableRates, true );
	QDomNodeList ratesList = document.elementsByTagName( "Cube" );
	bool date = false;
	int num, position = FIXED_CURRENCIES + 1;

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
			for (num = FIXED_CURRENCIES; num < CURRENCIES; num++)
				if ( code == QString::fromUtf8( currency[num].code ) )
					break;
			if ( num < CURRENCIES )
			{
				currency[num].rate =
					elt.attribute( "rate" ).toDouble() /
					currency[num].rulesPrecision;
				currency[num].position = position++;
				CurrencyList->insertItem
					( QString::fromUtf8( currency[num].code ) +
					  " - " +
					  i18n( currency[num].name ) );
			}
		}
	}
	SourceLabel->setText( i18n( "ECB" ) );
	if ( !date )
		DateLabel->setText( i18n( "Not loaded" ) );
	variableRates = "";
	newRatesList( EURO_CURRENCY );
}

void KEuroCalc::httpResultNYFRB(KIO::Job *job)
{
	QDomDocument document( "rates" );
	document.setContent( variableRates, true );
	QDomNodeList ratesList = document.elementsByTagName( "Rate" );
	bool date = false;
	int num, position = 0;

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
			for (num = FIXED_CURRENCIES; num < CURRENCIES; num++)
				if ( newYorkName == currency[num].newYorkName )
					break;
			if ( num < CURRENCIES )
			{
				currency[num].rate =
					( valueElement.attribute( "InUsd" ) == "false" ?
					  valueElement.text().toDouble() :
					  1.0 / valueElement.text().toDouble()
					) / currency[num].rulesPrecision;
				currency[num].position = position++;
				CurrencyList->insertItem
					( QString::fromUtf8( currency[num].code ) +
					  " - " +
					  i18n( currency[num].name ) );
				if ( !date )
				{
					DateLabel->setText( dateElement.text() );
					date = true;
				}
			}
		}
	}
	SourceLabel->setText( i18n( "NY FRB" ) );
	if ( !date )
		DateLabel->setText( i18n( "Not loaded" ) );
	variableRates = "";
	newRatesList( DOLLAR_CURRENCY );
}

void KEuroCalc::inputDot()
{
	inputDigit('.');
}

void KEuroCalc::inputZero()
{
	inputDigit('0');
}

void KEuroCalc::inputOne()
{
	inputDigit('1');
}

void KEuroCalc::inputTwo()
{
	inputDigit('2');
}

void KEuroCalc::inputThree()
{
	inputDigit('3');
}

void KEuroCalc::inputFour()
{
	inputDigit('4');
}

void KEuroCalc::inputFive()
{
	inputDigit('5');
}

void KEuroCalc::inputSix()
{
	inputDigit('6');
}

void KEuroCalc::inputSeven()
{
	inputDigit('7');
}

void KEuroCalc::inputEight()
{
	inputDigit('8');
}

void KEuroCalc::inputNine()
{
	inputDigit('9');
}

void KEuroCalc::inputPlus()
{
	inputOperator('+');
}

void KEuroCalc::inputMinus()
{
	inputOperator('-');
}

void KEuroCalc::inputAsterisk()
{
	inputOperator('x');
}

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
	double inputValue = atof( inputDisplay ),
	       currencyRate = currency[currencyNum].rate,
	       currencyPrecision = currency[currencyNum].rulesPrecision;

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
	double inputValue = atof( inputDisplay ),
	       currencyRate = currency[currencyNum].rate,
	       currencyPrecision = currency[currencyNum].rulesPrecision;

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
	double inputValue = atof( inputDisplay ),
	       currencyRate = currency[currencyNum].rate,
	       currencyPrecision = currency[currencyNum].rulesPrecision;

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
	double inputValue = atof( inputDisplay ),
	       currencyRate = currency[currencyNum].rate,
	       currencyPrecision = currency[currencyNum].rulesPrecision;

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
	double currencyRate = currency[currencyNum].rate,
	       currencyPrecision = currency[currencyNum].rulesPrecision;

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
	double currencyRate = currency[currencyNum].rate,
	       currencyPrecision = currency[currencyNum].rulesPrecision;

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

	for (num = 0; num < CURRENCIES; num++)
		if (currency[num].position == position)
			break;
	if ( num == CURRENCIES ) return;
	currencyNum = num;

	displayNewCurrency();

	currencyValue = referenceValue * currency[currencyNum].rate * currency[currencyNum].rulesPrecision;
	displayNewResult();

	writeOptions();
}

// Read options from preferences file
void KEuroCalc::readOptions()
{
	KConfig *config;
	QString option;

	config = KApplication::kApplication()->config();

	config->setGroup("General");

	option = config->readEntry("Currency", "N/A");
	for (currencyNum = 0; currencyNum < CURRENCIES; currencyNum++)
		if ( option == QString::fromUtf8( currency[currencyNum].code ) )
			break;
	if ( currencyNum == CURRENCIES ) currencyNum = 0;

	option = config->readEntry("Reference", "euroECB");
	if (option == "euroFixed")
		reference = euroFixed;
	else if (option == "euroECB")
		reference = euroECB;
	else if (option == "dollarNYFRB")
		reference = dollarNYFRB;
	else reference = euroECB;
}

// Write options to preferences file
void KEuroCalc::writeOptions()
{
	KConfig *config;

	config = KApplication::kApplication()->config();

	config->setGroup("General");

	config->writeEntry("Currency", QString::fromUtf8(currency[currencyNum].code) );

	switch (reference)
	{	case euroFixed:
			config->writeEntry("Reference", "euroFixed");
			break;
		case euroECB:
			config->writeEntry("Reference", "euroECB");
			break;
		case dollarNYFRB:
			config->writeEntry("Reference", "dollarNYFRB");
	}

	config->sync();
}

// Initialize the buttons without translation and the buttons with UTF-8
// text not correctly handled by uic
void KEuroCalc::initButtons()
{
	if (reference == euroFixed || reference == euroECB)
	{
		for (int num = 0; num < FIXED_CURRENCIES; num++)
		{
			currency[num].position = num;
			CurrencyList->insertItem
				( QString::fromUtf8( currency[num].code ) +
				  QString::fromUtf8( " - " ) +
				  i18n( currency[num].name )
				);
		}
	}
	if (reference == euroFixed)
	{
		SourceLabel->setText( i18n( "Fixed" ) );
		DateLabel->setText( "" );
		newRatesList( EURO_CURRENCY );
	}
	else if (reference == euroECB)
	{
		CurrencyList->insertItem( QString::fromUtf8
			( "--------------------------------------------" ) );
		KIO::SimpleJob *job = KIO::get( KURL( urlECB ), true, false );
		connect( job, SIGNAL(data(KIO::Job *, const QByteArray &)),
			 this, SLOT(httpData(KIO::Job *, const QByteArray &))
			);
		connect( job, SIGNAL(result(KIO::Job *)),
			 this, SLOT(httpResultECB(KIO::Job *))
			);
	}
	else if (reference == dollarNYFRB)
	{
		char url[128];
		QDate yesterday;
		// This is suboptimal: we should guess the date of latest working day at 12:00 in New York local time
		// Or much better: use a URL that does not depend on that date...

		yesterday = QDate::currentDate().addDays(-1);
		sprintf(url, urlNYFRB, yesterday.year(), yesterday.month(), yesterday.day());
		KIO::SimpleJob *job = KIO::get( KURL( url ), true, false );
		connect( job, SIGNAL(data(KIO::Job *, const QByteArray &)),
			 this, SLOT(httpData(KIO::Job *, const QByteArray &))
			);
		connect( job, SIGNAL(result(KIO::Job *)),
			 this, SLOT(httpResultNYFRB(KIO::Job *))
			);
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
	ReferenceButton->setText( QString::fromUtf8( reference == dollarNYFRB? dollarSymbol: euroSymbol ) );
	PercentButton->setText( QString::fromUtf8( "%" ) );
	PlusMinusButton->setText( QString::fromUtf8( "+/-" ) );
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
		float roundedReferenceValue, roundedCurrencyValue;
		double currencyPrecision;
		char currencySymbol[8];
		QString euroDisplay, currencyDisplay;

		roundedReferenceValue = floor(referenceValue * 100.0 + 0.5)
				 / 100.0;
		euroDisplay.setNum( roundedReferenceValue, 'f', 2 );
		normalize( euroDisplay );

		currencyPrecision = currency[currencyNum].rulesPrecision;
		strcpy( currencySymbol, currency[currencyNum].symbol );
		roundedCurrencyValue = floor(currencyValue * 100.0 / currencyPrecision + 0.5)
				     / 100.0 * currencyPrecision;
		currencyDisplay.setNum( roundedCurrencyValue, 'f', currencyPrecision < 100.0? 2: 0 );
		normalize( currencyDisplay );

		ResultDisplay->setText( euroDisplay + QString::fromUtf8( reference == dollarNYFRB ? dollarSymbol: euroSymbol ) + "\n" +
					currencyDisplay + " " + QString::fromUtf8( currencySymbol ) );
	}
}

// Update the currency button accordingly to new currency
void KEuroCalc::displayNewCurrency()
{
	double currencyRate = currency[currencyNum].rate;
	double currencyPrecision = currency[currencyNum].rulesPrecision;
	char currencySymbol[8];

	strcpy( currencySymbol, currency[currencyNum].symbol );
	CurrencyButton->setText( QString::fromUtf8( currencySymbol ) );
	if ( currency[currencyNum].position >= 0 )
	{
		QString rate;
		rate.setNum( currencyRate * currencyPrecision ); 
		RateLabel->setText( "1" + QString::fromUtf8( reference == dollarNYFRB ? dollarSymbol: euroSymbol ) + " = " +
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

