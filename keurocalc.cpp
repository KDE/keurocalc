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

#define CURRENCIES 39
#define FIXED_CURRENCIES 12
#define VARIABLE_CURRENCIES 27

static struct currencyStruc {
	double rate;
	const char *symbol;
	const char *code;
	double euRulesPrecision;
	double smallestCoinPrecision;
	const char *name;
	int position;
} currency[CURRENCIES] =
{
	{ 13.7603,  "öS", "ATS",   1.0,  10.0, I18N_NOOP("Austrian schilling"), 0 },
	{ 40.3399,   "FB", "BEF",   1.0,  10.0, I18N_NOOP("Belgian franc"), 1 },
	{ 1.95583,   "DM", "DEM",   1.0,   1.0, I18N_NOOP("German deutschmark"), 2 },
	{ 1.66386,  "Pts", "ESP", 100.0, 100.0, I18N_NOOP("Spanish peseta"), 3 },
	{ 5.94573,   "mk", "FIM",   1.0,   1.0, I18N_NOOP("Finnish markka"), 4 },
	{ 6.55957,   "FF", "FRF",   1.0,   1.0, I18N_NOOP("French franc"), 5 },
	{ 3.40750,   "Dr", "GRD", 100.0, 100.0, I18N_NOOP("Greek drachma"), 6 },
	{ .787564, "£IR", "IEP",   1.0,   1.0, I18N_NOOP("Irish punt"), 7 },
	{ 19.3627,    "L", "ITL", 100.0,1000.0, I18N_NOOP("Italian lira"), 8 },
	{ 40.3399, "FLux", "LUF",   1.0,  10.0, I18N_NOOP("Luxembourg franc"), 9 },
	{ 2.20371,   "Fl", "NLG",   1.0,   1.0, I18N_NOOP("Dutch gulden"), 10 },
	{ 2.00482,  "Esc", "PTE", 100.0, 100.0, I18N_NOOP("Portuguese escudo"), 11 },
	{     1.0,   "A$", "AUD",   1.0,   1.0, I18N_NOOP("Australian dollar"), -1 },
	{     1.0,   "Lv", "BGN",   1.0,   1.0, I18N_NOOP("Bulgarian lev"), -1 },
	{     1.0, "Can$", "CAD",   1.0,   1.0, I18N_NOOP("Canadian dollar"), -1 },
	{     1.0,   "FS", "CHF",   1.0,   1.0, I18N_NOOP("Swiss franc"), -1 },
	{     1.0,  "£C", "CYP",   1.0,   1.0, I18N_NOOP("Cyprus pound"), -1 },
	{     1.0,    "K", "CZK",   1.0,   1.0, I18N_NOOP("Czech koruna"), -1 },
	{     1.0,  "DKr", "DKK",   1.0,   1.0, I18N_NOOP("Danish krone"), -1 },
	{     1.0,  "EKr", "EEK",   1.0,   1.0, I18N_NOOP("Estonian kroon"), -1 },
	{     1.0,   "£", "GBP",   1.0,   1.0, I18N_NOOP("Great Britain pound"), -1 },
	{     1.0,  "HK$", "HKD",   1.0,   1.0, I18N_NOOP("Hong kong dollar"), -1 },
	{     1.0,   "Ht", "HUF", 100.0, 100.0, I18N_NOOP("Hungarian forint"), -1 },
	{     1.0,  "Ikr", "ISK",   1.0,   1.0, I18N_NOOP("Icelandic krona"), -1 },
	{     1.0,   "¥", "JPY", 100.0, 100.0, I18N_NOOP("Japanese yen"), -1 },
	{     1.0,    "W", "KRW",1000.0,1000.0, I18N_NOOP("South Korean won"), -1 }, // should be "₩"
	{     1.0,   "lt", "LTL",   1.0,   1.0, I18N_NOOP("Latvian lats"), -1 },
	{     1.0,   "Ls", "LVL",   1.0,   1.0, I18N_NOOP("Lithuanian litas"), -1 },
	{     1.0,   "Lm", "MTL",   1.0,   1.0, I18N_NOOP("Maltese lira"), -1 },
	{     1.0,  "NKr", "NOK",   1.0,   1.0, I18N_NOOP("Norvegian krone"), -1 },
	{     1.0,  "NZ$", "NZD",   1.0,   1.0, I18N_NOOP("New Zealand dollar"), -1 },
	{     1.0, "zł", "PLN",   1.0,   1.0, I18N_NOOP("Polish sloty"), -1 },
	{     1.0,    "L", "ROL",  10.0,10000., I18N_NOOP("Romanian leu"), -1 },
	{     1.0,  "SKr", "SEK",   1.0,   1.0, I18N_NOOP("Swedish krona"), -1 },
	{     1.0,   "S$", "SGB",   1.0,   1.0, I18N_NOOP("Singapore dollar"), -1 },
	{     1.0,  "SIT", "SIT",   1.0,   1.0, I18N_NOOP("Slovenian tolar"), -1 },
	{     1.0,   "Sk", "SKK",   1.0,   1.0, I18N_NOOP("Slovakian koruna"), -1 },
	{     1.0,    "$", "USD",   1.0,   1.0, I18N_NOOP("US dollar"), -1 },
	{     1.0,    "R", "ZAR",   1.0,   1.0, I18N_NOOP("South African rand"), -1 }
};

static const char
	*euroSymbol = " €",
	*ratesUrl = "http://www.ecb.int/stats/eurofxref/eurofxref-daily.xml";

KEuroCalc::KEuroCalc(QWidget *parent, const char *name)
	: Calculator( parent, name ),
	  variableRates()
{
	int currencyNum, position;

	isSimpleValue = false;
	simpleValue = 0.0;
	euroValue = 0.0;
	currencyValue = 0.0;

	memorySet = false;
	isSimpleMemory = false;
	simpleMemory = 0.0;
	euroMemory = 0.0;

	readOptions( currencyNum );
	position = currency[currencyNum].position;
	if (position < 0)
		currencyNum = position = 0;
	CurrencyList->setCurrentItem( position );
	displayNewCurrency( currencyNum );
	displayNewResult();

	resetInput();

	initButtons();

	setFixedSize(size());
}

KEuroCalc::~KEuroCalc()
{
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
			EuroButton->animateClick();
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

void KEuroCalc::httpResult(KIO::Job *job)
{
	QDomDocument document( "rates" );
	document.setContent( variableRates, true );
	QDomNodeList ratesList = document.elementsByTagName( "Cube" );
	bool date = false;
	int position = FIXED_CURRENCIES + 1;
	int currencyNum;

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
			for (currencyNum = FIXED_CURRENCIES; currencyNum < CURRENCIES; currencyNum++)
				if ( code == QString::fromUtf8( currency[currencyNum].code ) )
					break;
			if ( currencyNum < CURRENCIES )
			{
				currency[currencyNum].rate =
					elt.attribute( "rate" ).toDouble() /
					currency[currencyNum].euRulesPrecision;
				currency[currencyNum].position = position++;
				CurrencyList->insertItem
					( QString::fromUtf8( currency[currencyNum].code ) +
					  " - " +
					  i18n( currency[currencyNum].name ) );
			}
		}
	}
	if ( !date )
		DateLabel->setText( i18n( "Not loaded" ) );

	readOptions( currencyNum );
	position = currency[currencyNum].position;
	if (position < 0)
		currencyNum = position = 0;
	CurrencyList->setCurrentItem( position );
	displayNewCurrency( currencyNum );
	displayNewResult();
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
void KEuroCalc::validateEuro()
{
	double inputValue = atof( inputDisplay );

	if ( inputDisplay[10] == ' ' )
	{
		if ( isSimpleValue && *operatorDisplay == ' ' )
		{
			isSimpleValue = false;
			euroValue = simpleValue;
			currencyValue = euroValue * currencyRate * currencyPrecision;
			displayNewResult();
		}
		else KApplication::beep();
		return;
	}

	switch ( *operatorDisplay )
	{
		case ' ':
			isSimpleValue = false;
			euroValue = inputValue;
			currencyValue = euroValue * currencyRate * currencyPrecision;
			break;
		case '+':
			if ( isSimpleValue )
			{
				KApplication::beep();
				return;
			}
			euroValue += inputValue;
			currencyValue = euroValue * currencyRate * currencyPrecision;
			break;
		case '-':
			if ( isSimpleValue )
			{
				KApplication::beep();
				return;
			}
			euroValue -= inputValue;
			currencyValue = euroValue * currencyRate * currencyPrecision;
			break;
		case 'x':
			if ( isSimpleValue )
			{
				isSimpleValue = false;
				euroValue = simpleValue * inputValue;
				currencyValue = euroValue * currencyRate * currencyPrecision;
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
				simpleValue = euroValue / inputValue;
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
	double inputValue = atof( inputDisplay );

	if ( inputDisplay[10] == ' ' )
	{
		if ( isSimpleValue && *operatorDisplay == ' ' )
		{
			isSimpleValue = false;
			currencyValue = simpleValue;
			euroValue = currencyValue / currencyRate / currencyPrecision;
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
			euroValue = currencyValue / currencyRate / currencyPrecision;
			break;
		case '+':
			if ( isSimpleValue )
			{
				KApplication::beep();
				return;
			}
			currencyValue += inputValue;
			euroValue = currencyValue / currencyRate / currencyPrecision;
			break;
		case '-':
			if ( isSimpleValue )
			{
				KApplication::beep();
				return;
			}
			currencyValue -= inputValue;
			euroValue = currencyValue / currencyRate / currencyPrecision;
			break;
		case 'x':
			if ( isSimpleValue )
			{
				isSimpleValue = false;
				currencyValue = simpleValue * inputValue;
				euroValue = currencyValue / currencyRate / currencyPrecision;
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
	double inputValue = atof( inputDisplay );

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
				euroValue = (inputValue * euroValue) / 100.0;
				currencyValue = euroValue * currencyRate * currencyPrecision;
			}
			break;
		case '+':
			if ( isSimpleValue )
				simpleValue = ((100.0 + inputValue) * simpleValue) / 100.0;
			else
			{
				euroValue = ((100.0 + inputValue) * euroValue) / 100.0;
				currencyValue = euroValue * currencyRate * currencyPrecision;
			}
			break;
		case '-':
			if ( isSimpleValue )
				simpleValue = ((100.0 - inputValue) * simpleValue) / 100.0;
			else
			{
				euroValue = ((100.0 - inputValue) * euroValue) / 100.0;
				currencyValue = euroValue * currencyRate * currencyPrecision;
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
	double inputValue = atof( inputDisplay );

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
				euroValue *= inputValue;
				currencyValue = euroValue * currencyRate * currencyPrecision;
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
				euroValue /= inputValue;
				currencyValue = euroValue * currencyRate * currencyPrecision;
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
	if ( isSimpleValue )
		simpleValue = -simpleValue;
	else
	{
		euroValue = -euroValue;
		currencyValue = euroValue * currencyRate * currencyPrecision;
	}
	displayNewResult();
}

// Tranfer from display to memory
void KEuroCalc::memoryInput()
{
	memorySet = true;
	isSimpleMemory = isSimpleValue;
	simpleMemory = simpleValue;
	euroMemory = euroValue;

	displayMemoryButtons();
}

// Transfer from memory to display
void KEuroCalc::memoryRecall()
{
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
				euroValue = euroMemory;
				currencyValue = euroValue * currencyRate * currencyPrecision;
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
				euroValue += euroMemory;
				currencyValue = euroValue * currencyRate * currencyPrecision;
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
				euroValue -= euroMemory;
				currencyValue = euroValue * currencyRate * currencyPrecision;
			}
			break;
		case 'x':
			if ( isSimpleMemory )
			{
				if ( isSimpleValue )
					simpleValue *= simpleMemory;
				else
				{
					euroValue *= simpleMemory;
					currencyValue = euroValue * currencyRate * currencyPrecision;
				}
			}
			else
			{
				if ( isSimpleValue )
				{
					isSimpleValue = false;
					euroValue = simpleValue * euroMemory;
					currencyValue = euroValue * currencyRate * currencyPrecision;
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
					euroValue /= simpleMemory;
					currencyValue = euroValue * currencyRate * currencyPrecision;
				}
			}
			else
			{
				if ( !isSimpleValue && euroMemory != 0.0 )
				{
					isSimpleValue = true;
					simpleValue = euroValue / euroMemory;
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
		euroMemory += euroValue;
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
		euroMemory -= euroValue;
}

// Reset both input, result and memory
void KEuroCalc::reset()
{
	resetInput();

	isSimpleValue = false;
	simpleValue = 0.0;
	euroValue = 0.0;
	currencyValue = 0.0;

	displayNewResult();

	memorySet = false;
	isSimpleMemory = false;
	simpleMemory = 0.0;
	euroMemory = 0.0;

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

// Select currency accordingly to radio button number
void KEuroCalc::selectCurrency(int position)
{
	int currencyNum;

	for (currencyNum = 0; currencyNum < CURRENCIES; currencyNum++)
		if (currency[currencyNum].position == position)
			break;
	if (currencyNum == CURRENCIES) return;

	displayNewCurrency( currencyNum );

	currencyValue = euroValue * currencyRate * currencyPrecision;
	displayNewResult();

	writeOptions( currencyNum );
}

// Read options from preferences file
void KEuroCalc::readOptions(int &currencyNum)
{
	KConfig *config;
	QString option;

	config = KApplication::kApplication()->config();

	config->setGroup("General");
	option = config->readEntry("Currency", "ATS");

	for (currencyNum = 0; currencyNum < CURRENCIES; currencyNum++)
		if ( option == QString::fromUtf8( currency[currencyNum].code ) )
			break;
	if ( currencyNum == CURRENCIES ) currencyNum = 0;
}

// Write options to preferences file
void KEuroCalc::writeOptions( int currencyNum )
{
	KConfig *config;

	config = KApplication::kApplication()->config();

	config->setGroup("General");
	config->writeEntry("Currency", QString::fromUtf8(currency[currencyNum].code) );

	config->sync();
}

// Initialize the buttons without translation and the buttons with UTF-8
// text not correctly handled by uic
void KEuroCalc::initButtons()
{
	for (int currencyNum = 0; currencyNum < FIXED_CURRENCIES; currencyNum++)
		CurrencyList->insertItem
			( QString::fromUtf8( currency[currencyNum].code ) +
			  QString::fromUtf8( " - " ) +
			  i18n( currency[currencyNum].name ) );
	CurrencyList->insertItem( QString::fromUtf8
		( "--------------------------------------------" ) );

	KIO::SimpleJob *job = KIO::get( KURL( ratesUrl ), true, false );
	connect( job, SIGNAL(data(KIO::Job *, const QByteArray &)),
		 this, SLOT(httpData(KIO::Job *, const QByteArray &))
		);
	connect( job, SIGNAL(result(KIO::Job *)),
		 this, SLOT(httpResult(KIO::Job *))
		);

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
	EuroButton->setText( QString::fromUtf8( euroSymbol ) );
	PercentButton->setText( QString::fromUtf8( "%" ) );
	PlusMinusButton->setText( QString::fromUtf8( "+/-" ) );
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
	float roundedEuroValue, roundedCurrencyValue;
	QString simpleDisplay,
	        euroDisplay, currencyDisplay;

	if ( isSimpleValue )
	{
		simpleDisplay.setNum( simpleValue );
		normalize( simpleDisplay );
		ResultDisplay->setText( simpleDisplay );
	}
	else
	{
		roundedEuroValue = floor(euroValue * 100.0 + 0.5)
				 / 100.0;
		euroDisplay.setNum( roundedEuroValue, 'f', 2 );
		normalize( euroDisplay );

		roundedCurrencyValue = floor(currencyValue * 100.0 / currencyPrecision + 0.5)
				     / 100.0 * currencyPrecision;
		currencyDisplay.setNum( roundedCurrencyValue, 'f', currencyPrecision < 100.0? 2: 0 );
		normalize( currencyDisplay );

		ResultDisplay->setText( euroDisplay + QString::fromUtf8( euroSymbol ) + "\n" +
					currencyDisplay + " " + QString::fromUtf8( currencySymbol ) );
	}
}

// Update the currency button accordingly to new currency
void KEuroCalc::displayNewCurrency( int currencyNum )
{
	currencyRate = currency[currencyNum].rate;
	strcpy( currencySymbol, currency[currencyNum].symbol );
	currencyPrecision = currency[currencyNum].euRulesPrecision;

	CurrencyButton->setText( QString::fromUtf8( currencySymbol ) );
	if ( currency[currencyNum].position >= 0 )
	{
		QString rate;
		rate.setNum( currencyRate * currencyPrecision ); 
		RateLabel->setText( "1" + QString::fromUtf8( euroSymbol ) + " = " +
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

