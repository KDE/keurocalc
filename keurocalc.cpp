/***************************************************************************
                          keurocalc.cpp  -  main widget
                             -------------------
    begin                : sam d�  1 23:40:19 CET 2001
    copyright            : (C) 2001 by �ic Bischoff
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
#include <qradiobutton.h>
#include <qbuttongroup.h>

#include <kapp.h>
#include <kaboutapplication.h>
#include <klocale.h>
#include <kglobal.h>
#include <kconfig.h>

#include "keurocalc.h"
#include "keurocalc.moc"


static struct currencyStruc {
	double rate;
	const char *symbol;
	double precision;
	QButton *button;
} currency[12] = {
	{ 1.95583,  "DM",   1.0, 0 },
	{ 3.40750,  "DR", 100.0, 0 },
	{ 2.00482, "ESC", 100.0, 0 },
	{ 4.03399,  "FB",  10.0, 0 },
	{ 6.55957,  "FF",   1.0, 0 },
	{ 2.20371,  "FL",   1.0, 0 },
	{ 4.03399,"FLUX",  10.0, 0 },
	{ 1.93627,   "L",1000.0, 0 },
	{ 5.94573,  "MF",   1.0, 0 },
	{ 1.37603,  "öS",  10.0, 0 },
	{ 1.66386, "PTA", 100.0, 0 },
	{ 0.78756, "£IR",   1.0, 0 }
};

static const char *euroSymbol = " €";

KEuroCalc::KEuroCalc(QWidget *parent, const char *name)
  : Calculator( parent, name )
{
	int currencyNum;
	
	readOptions( currencyNum );

	resetInput();

	initButtons();

	CurrencyButtonGroup->setButton( currencyNum );
	currencyRate = currency[currencyNum].rate;
	strcpy( currencySymbol, currency[currencyNum].symbol );
	currencyPrecision = currency[currencyNum].precision;
	displayNewCurrency();

	isSimpleValue = false;
	simpleValue = 0.0;
	euroValue = 0.0;
	currencyValue = 0.0;
	displayNewResult();

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
		case Key_Equal:
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
	inputOperator('*');
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

	switch ( *operatorDisplay )
	{
		case ' ':
			isSimpleValue = false;
			euroValue = inputValue;
			currencyValue = euroValue * currencyRate * currencyPrecision;
			break;
		case '+':
			if (isSimpleValue)
			{
				KApplication().beep();
				return;
			}
			euroValue += inputValue;
			currencyValue = euroValue * currencyRate * currencyPrecision;
			break;
		case '-':
			if (isSimpleValue)
			{
				KApplication().beep();
				return;
			}
			euroValue -= inputValue;
			currencyValue = euroValue * currencyRate * currencyPrecision;
			break;
		case '*':
			if (isSimpleValue)
			{
				isSimpleValue = false;
				euroValue = simpleValue * inputValue;
				currencyValue = euroValue * currencyRate * currencyPrecision;
			}
			else
			{
				KApplication().beep();
				return;
			}
			break;
		case '/':
			if (!isSimpleValue && inputValue != 0.0)
			{
				isSimpleValue = true;
				simpleValue = euroValue / inputValue;
			}
			else
			{
				KApplication().beep();
				return;
			}
	}
	resetInput();
	displayNewResult();
}

// Convert to currency, and add or substract too if needed
void KEuroCalc::validateCurrency()
{
	double inputValue = atof( inputDisplay );

	switch ( *operatorDisplay )
	{
		case ' ':
			isSimpleValue = false;
			currencyValue = inputValue;
			euroValue = currencyValue / currencyRate / currencyPrecision;
			break;
		case '+':
			if (isSimpleValue)
			{
				KApplication().beep();
				return;
			}
			currencyValue += inputValue;
			euroValue = currencyValue / currencyRate / currencyPrecision;
			break;
		case '-':
			if (isSimpleValue)
			{
				KApplication().beep();
				return;
			}
			currencyValue -= inputValue;
			euroValue = currencyValue / currencyRate / currencyPrecision;
			break;
		case '*':
			if (isSimpleValue)
			{
				isSimpleValue = false;
				currencyValue = simpleValue * inputValue;
				euroValue = currencyValue / currencyRate / currencyPrecision;
			}
			else
			{
				KApplication().beep();
				return;
			}
			break;
		case '/':
			if (!isSimpleValue && inputValue != 0.0)
			{
				isSimpleValue = true;
				simpleValue = currencyValue / inputValue;
			}
			else
			{
				KApplication().beep();
				return;
			}
	}
	resetInput();
	displayNewResult();
}

// Apply percentage, and add or substract too if needed
void KEuroCalc::validatePercent()
{
	double inputValue = atof( inputDisplay );

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
		case '*':
		case '/':
			KApplication().beep();
			return;
	}
	resetInput();
	displayNewResult();
}

// Enter a simple value, and multiply or divide too if needed.
void KEuroCalc::validateSimpleValue()
{
	double inputValue = atof( inputDisplay );

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
				KApplication().beep();
				return;
			}
			break;
		case '-':
			if ( isSimpleValue )
				simpleValue -= inputValue;
			else
			{
				KApplication().beep();
				return;
			}
			break;
		case '*':
			if ( isSimpleValue )
				simpleValue *= inputValue;
			else
			{
				euroValue *= inputValue;
				currencyValue = euroValue * currencyRate * currencyPrecision;
			}
			break;
		case '/':
			if (inputValue == 0.0)
			{
				KApplication().beep();
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
			KApplication().beep();
			return;
	}
	resetInput();
	displayNewResult();
}

// Change the sign of the result
void KEuroCalc::changeSign()
{
	double inputValue = atof( inputDisplay );

	euroValue = -euroValue;
	currencyValue = euroValue * currencyRate * currencyPrecision;
	
	displayNewResult();
}

// Reset both input and result
void KEuroCalc::reset()
{
	resetInput();

	isSimpleValue = false;
	simpleValue = 0.0;
	euroValue = 0.0;
	currencyValue = 0.0;

	displayNewResult();
}

// Display "about" page
void KEuroCalc::displayAbout()
{
	KAboutApplication *d = new KAboutApplication;
	d->show();
	delete d;
	AboutButton->setDown(false);
}

// Display help pages
void KEuroCalc::displayHelp()
{
	kapp->invokeHelp();
}

// Select currency accordingly to radio button number
void KEuroCalc::selectCurrency(int currencyNum)
{
	currencyRate = currency[currencyNum].rate;
	strcpy( currencySymbol, currency[currencyNum].symbol );
	currencyPrecision = currency[currencyNum].precision;
	displayNewCurrency();

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
	option = config->readEntry("Currency", "DM");

	for (currencyNum = 0; currencyNum < 12; currencyNum++)
		if ( option == QString::fromUtf8(currency[currencyNum].symbol) )
			break;
	if (currencyNum == 12) currencyNum = 0;
}

// Write options to preferences file
void KEuroCalc::writeOptions( int currencyNum )
{
	KConfig *config;

	config = KApplication::kApplication()->config();

	config->setGroup("General");
	config->writeEntry("Currency", QString::fromUtf8(currency[currencyNum].symbol) );

	config->sync();
}

// Initialize the buttons without translation and the buttons with UTF-8
// text not correctly handled by uic
void KEuroCalc::initButtons()
{
	currency[0].button = DeutschmarkButton;
	currency[1].button = DrachmaButton;
	currency[2].button = EscudoButton;
	currency[3].button = FrancBelgeButton;
	currency[4].button = FrancFrancaisButton;
	currency[5].button = GuldenButton;
	currency[6].button = FrancLuxembourgeoisButton;
	currency[7].button = LiraButton;
	currency[8].button = MarkkaButton;
	currency[9].button = SchillingButton;
	currency[10].button = PesetaButton;
	currency[11].button = PuntButton;

	for (int currencyNum = 0; currencyNum < 12; currencyNum++)
		currency[currencyNum].button->setText( QString::fromUtf8(currency[currencyNum].symbol) );

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

	SlashButton->setText( QString::fromUtf8( "/" ) );
	AsteriskButton->setText( QString::fromUtf8( "X" ) );
	MinusButton->setText( QString::fromUtf8( "-" ) );
	PlusButton->setText( QString::fromUtf8( "+" ) );
	EuroButton->setText( QString::fromUtf8( euroSymbol ) );
	PercentButton->setText( QString::fromUtf8( "%" ) );
	SimpleValueButton->setText( QString::fromUtf8( "=" ) );
	PlusMinusButton->setText( QString::fromUtf8( "+/-" ) );
	ResetButton->setText( QString::fromUtf8( "AC" ) );
}

// Input a digit ('0' to '9' or '.')
void KEuroCalc::inputDigit(char c)
{
	const char *s;
	char *d;
	switch ( inputPos )
	{
		case atUnits:
			switch ( c )
			{
				case '.':
					inputDisplay[9] = '0';
					inputDisplay[10] = c;
					inputPos = afterUnits;
					break;
				case '0':
					break;
				default:
					inputDisplay[10] = c;
					inputPos = beforeUnits;
			}
			break;
		case beforeUnits:
			if ( inputDisplay[0] != ' ' )
			{
				KApplication().beep();
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
				KApplication().beep();
				return;
			}
			if ( c == '.' )
			{
				KApplication().beep();
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
			if (inputDisplay[9] == ' ' && inputDisplay[10] == '0')
				*operatorDisplay = ' ';
			break;
		case beforeUnits:
		case afterUnits:
			s = inputDisplay + 9;
			d = inputDisplay + 10;
			while ( d > inputDisplay) *d-- = *s--;
			*d = ' ';
			if (inputDisplay[10] == ' ') inputDisplay[10] = '0';
			break;
	}
	inputPos =
		inputDisplay[9] == ' ' && inputDisplay[10] == '0'?
		atUnits:
		(QString(inputDisplay).find('.') == -1? beforeUnits: afterUnits);

	displayNewInput();
}

// Input an operator ('+', '-', '*' or '/')
void KEuroCalc::inputOperator(char c)
{
	if (inputDisplay[9] != ' ' || inputDisplay[10] != '0')
		validateSimpleValue();

	*operatorDisplay = c;

	displayNewInput();
}

// Reset input area
void KEuroCalc::resetInput()
{
	strcpy(operatorDisplay, " ");
	strcpy( inputDisplay, "          0" );
	inputPos = atUnits;
	OperatorDisplay->setText( operatorDisplay );
	InputDisplay->setText( inputDisplay );
}

// Display current input in input area
void KEuroCalc::displayNewInput()
{
	QString display(inputDisplay);
	int pos = display.find('.');

	if ( pos != -1 )
		display.replace(pos,
				KGlobal::locale()->decimalSymbol().length(),
				KGlobal::locale()->decimalSymbol() );

	OperatorDisplay->setText( operatorDisplay );
	InputDisplay->setText( display );
}

// Display latest computed value in results area
void KEuroCalc::displayNewResult()
{
	float rounding,
	      roundedEuroValue, roundedCurrencyValue;
	QString simpleDisplay,
	        euroDisplay, currencyDisplay;
	int pos;

	if ( isSimpleValue )
	{
		simpleDisplay.setNum( simpleValue );
		ResultDisplay->setText( simpleDisplay );
	}
	else
	{
		rounding = euroValue >= 0.0? 0.5: -0.5;
		roundedEuroValue = int(euroValue * 100.0 + rounding)
				 / 100.0;
		euroDisplay.setNum( roundedEuroValue, 'f', 2 );
		pos = euroDisplay.find('.');
		if ( pos != -1 )
			euroDisplay.replace(pos,
					    KGlobal::locale()->decimalSymbol().length(),
					    KGlobal::locale()->decimalSymbol() );

		rounding = currencyValue >= 0.0? 0.5: -0.5;
		roundedCurrencyValue = int(currencyValue * 100.0 / currencyPrecision + rounding)
				     / 100.0 * currencyPrecision;
		currencyDisplay.setNum( roundedCurrencyValue, 'f', currencyPrecision < 100.0? 2: 0 );
		pos = currencyDisplay.find('.');
		if ( pos != -1 )
			currencyDisplay.replace(pos,
						KGlobal::locale()->decimalSymbol().length(),
						KGlobal::locale()->decimalSymbol() );

		ResultDisplay->setText( euroDisplay + QString::fromUtf8( euroSymbol ) + "\n" +
					currencyDisplay + " " + QString::fromUtf8( currencySymbol ) );
	}
}

// Update the currency button accordingly to new currency
void KEuroCalc::displayNewCurrency()
{
	CurrencyButton->setText( QString::fromUtf8( currencySymbol ) );
}
