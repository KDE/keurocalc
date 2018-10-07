/***************************************************************************
                          keurocalc.cpp  -  main widget
                             -------------------
    begin                : sam déc  1 23:40:19 CET 2001
    copyright            : (C) 2001-2018 by Éric Bischoff
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

#include <stdlib.h>
#include <math.h>
#include <locale.h>

#include <QDBusConnection>
#include <QPushButton>
#include <QKeyEvent>

#include <KLocalizedString>
#include <KConfigGroup>
#include <KSharedConfig>
#include <KAboutData>
#include <KAboutApplicationDialog>
#include <KMessageBox>
#include <KHelpClient>

#include "keurocalc.h"
#include "ui_calculator.h"
#include "preferences.h"

#include <config-keurocalc.h>

#if HAVE_XLOCALE_H
#include <xlocale.h>
#endif

static const char
	*euroSymbol = " €";
//	*dollarSymbol = " $";

// Constructor
KEuroCalc::KEuroCalc(QWidget *parent)
	: QDialog(parent), Ui::Calculator(),
          currencies()
{
	int position;
	QPalette palette;

	setupUi(this);
	QDBusConnection::sessionBus().registerObject
		("/KEuroCalc", this, QDBusConnection::ExportScriptableSlots);
	connect( &currencies, SIGNAL(endDownload(int,QString)),
	         this, SLOT(endDownload(int,QString))
	       );

	c_locale = newlocale(LC_NUMERIC_MASK, "C", NULL);
	isSimpleValue = false;
	simpleValue = 0.0;
	referenceValue = 0.0;
	currencyValue = 0.0;

	memorySet = false;
	isSimpleMemory = false;
	simpleMemory = 0.0;
	referenceMemory = 0.0;

	if ( !currencies.readCurrencies( "currencies.xml" ) )
	{
		KMessageBox::error( 0, i18n( "Cannot load currencies.xml" ) );
		exit(1);
	}
	readOptions( reference, currencyNum, rounding, displayColor, splashScreen );

	displayNewCurrency();
	displayNewResult();
	resetInput();
	initButtons();
	startDownload();

	position = currencies.position(currencyNum);
	if (position < 0) position = 0;
	CurrencyList->setCurrentIndex( position );

	palette.setColor( backgroundRole(), displayColor );
	ResultDisplay->setPalette( palette );
	InputDisplay->setPalette( palette );
	OperatorDisplay->setPalette( palette );

	setFocusPolicy( Qt::StrongFocus );
}

// Destructor
KEuroCalc::~KEuroCalc()
{
	freelocale(c_locale);
}

// Is splash screen to be displayed ?
bool KEuroCalc::readSplashScreen() const
{
	KConfigGroup config(KSharedConfig::openConfig(), "General");
	QString option;

	option = config.readEntry("SplashScreen", "yes");

	return option == "yes";
}

// Read options from preferences file
void KEuroCalc::readOptions(int &oldReference, int &oldCurrency, int &oldRounding, QColor &oldDisplayColor, bool &oldSplashScreen) const
{
	KConfigGroup config(KSharedConfig::openConfig(), "General");
	QString option;

	option = config.readEntry("Reference", "EURO_ECB");
	if (option == "EURO_FIXED")
		oldReference = EURO_FIXED;
	else if (option == "EURO_ECB")
		oldReference = EURO_ECB;
//	else if (option == "DOLLAR_NY_FRB")
//		oldReference = DOLLAR_NY_FRB;
	else if (option == "EURO_TG")
		oldReference = EURO_TG;
	else oldReference = EURO_ECB;

	option = config.readEntry("Currency", "USD");
	for (oldCurrency = 0; oldCurrency < currencies.number(); ++oldCurrency)
		if ( option == currencies.code(oldCurrency) )
			break;
	if ( oldCurrency == currencies.number() )
		oldCurrency = currencies.dollar();

	option = config.readEntry("Rounding", "OFFICIAL_RULES");
	if (option == "OFFICIAL_RULES")
		oldRounding = OFFICIAL_RULES;
	else if (option == "SMALLEST_COIN")
		oldRounding = SMALLEST_COIN;
	else if (option == "NO_ROUNDING")
		oldRounding = NO_ROUNDING;
	else oldRounding = OFFICIAL_RULES;

	option = config.readEntry("DiplayColor", "#C0FFFF");
	oldDisplayColor.setNamedColor(option);

	option = config.readEntry("SplashScreen", "yes");
	oldSplashScreen = option == "yes";
}

// Write options to preferences file
void KEuroCalc::writeOptions(int newReference, int newCurrency, int newRounding, const QColor &newDisplayColor, bool newSplashScreen)
{
	KConfigGroup config(KSharedConfig::openConfig(), "General");

	switch (newReference)
	{	case EURO_FIXED:
			config.writeEntry("Reference", "EURO_FIXED");
			break;
		case EURO_ECB:
			config.writeEntry("Reference", "EURO_ECB");
			break;
//		case DOLLAR_NY_FRB:
//			config.writeEntry("Reference", "DOLLAR_NY_FRB");
//			break;
		case EURO_TG:
			config.writeEntry("Reference", "EURO_TG");
	}

	config.writeEntry("Currency", currencies.code(newCurrency) );

	switch (newRounding)
	{	case OFFICIAL_RULES:
			config.writeEntry("Rounding", "OFFICIAL_RULES");
			break;
		case SMALLEST_COIN:
			config.writeEntry("Rounding", "SMALLEST_COIN");
			break;
		case NO_ROUNDING:
			config.writeEntry("Rounding", "NO_ROUNDING");
	}

	config.writeEntry("DiplayColor", newDisplayColor.name());

	config.writeEntry("SplashScreen", newSplashScreen? "yes": "no");

	config.sync();
}

// Set new preferences dialog
void KEuroCalc::setPreferences(int newReference, int newCurrency, int newRounding, const QColor &newDisplayColor, bool newSplashScreen)
{
	reference = newReference;
	currencyNum = newCurrency;
	rounding = newRounding;
	displayColor = newDisplayColor;
	splashScreen = newSplashScreen;

	CurrencyList->clear();
	initButtons();
	startDownload();
	Reset();
}

// Handle key press events
void KEuroCalc::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
		case Qt::Key_Period:
		case Qt::Key_Comma:
			inputDigit('.');
			break;
		case Qt::Key_0:
			ZeroButton->animateClick();
			break;
		case Qt::Key_1:
			OneButton->animateClick();
			break;
		case Qt::Key_2:
			TwoButton->animateClick();
			break;
		case Qt::Key_3:
			ThreeButton->animateClick();
			break;
		case Qt::Key_4:
			FourButton->animateClick();
			break;
		case Qt::Key_5:
			FiveButton->animateClick();
			break;
		case Qt::Key_6:
			SixButton->animateClick();
			break;
		case Qt::Key_7:
			SevenButton->animateClick();
			break;
		case Qt::Key_8:
			EightButton->animateClick();
			break;
		case Qt::Key_9:
			NineButton->animateClick();
			break;
		case Qt::Key_Plus:
			PlusButton->animateClick();
			break;
		case Qt::Key_Minus:
			MinusButton->animateClick();
			break;
		case Qt::Key_Asterisk:
			AsteriskButton->animateClick();
			break;
		case Qt::Key_Slash:
			SlashButton->animateClick();
			break;
		case Qt::Key_Backspace:
			BackspaceButton->animateClick();
			break;
		case Qt::Key_E:
		case Qt::Key_Dollar:
			ReferenceButton->animateClick();
			break;
		case Qt::Key_Percent:
			PercentButton->animateClick();
			break;
		case Qt::Key_Enter:
		case Qt::Key_Return:
			SimpleValueButton->animateClick();
			break;
		case Qt::Key_S:
			PlusMinusButton->animateClick();
			break;
		case Qt::Key_Shift:
		case Qt::Key_Control:
		case Qt::Key_Meta:
		case Qt::Key_Alt:
		case Qt::Key_CapsLock:
		case Qt::Key_NumLock:
		case Qt::Key_ScrollLock:
			break;
		default:
			CurrencyButton->animateClick();
			break;
	}
}

// Dot button or key pressed
void KEuroCalc::InputDot()
{
	inputDigit('.');
}

// Zero button or key pressed
void KEuroCalc::InputZero()
{
	inputDigit('0');
}

// One button or key pressed
void KEuroCalc::InputOne()
{
	inputDigit('1');
}

// Two button or key pressed
void KEuroCalc::InputTwo()
{
	inputDigit('2');
}

// Three button or key pressed
void KEuroCalc::InputThree()
{
	inputDigit('3');
}

// Four button or key pressed
void KEuroCalc::InputFour()
{
	inputDigit('4');
}

// Five button or key pressed
void KEuroCalc::InputFive()
{
	inputDigit('5');
}

// Six button or key pressed
void KEuroCalc::InputSix()
{
	inputDigit('6');
}

// Seven button or key pressed
void KEuroCalc::InputSeven()
{
	inputDigit('7');
}

// Eight button or key pressed
void KEuroCalc::InputEight()
{
	inputDigit('8');
}

// Nine button or key pressed
void KEuroCalc::InputNine()
{
	inputDigit('9');
}

// Plus button or key pressed
void KEuroCalc::InputPlus()
{
	inputOperator('+');
}

// Minus button or key pressed
void KEuroCalc::InputMinus()
{
	inputOperator('-');
}

// Multiply button or key pressed
void KEuroCalc::InputAsterisk()
{
	inputOperator('x');
}

// Divide button or key pressed
void KEuroCalc::InputSlash()
{
	inputOperator('/');
}

// Correct last entered digit
void KEuroCalc::InputBackspace()
{
	inputCorrect();
}

// Convert to currency, and add or subtract too if needed
void KEuroCalc::ValidateReference()
{
	double inputValue, currencyRate, currencyPrecision;

	inputValue = strtod_l( inputDisplay, NULL, c_locale );
	currencyRate = currencies.rate(currencyNum);
	switch (rounding)
	{
		case OFFICIAL_RULES:
			currencyPrecision = currencies.officialRulesPrecision(currencyNum);
			break;
		case SMALLEST_COIN:
			currencyPrecision = currencies.smallestCoinPrecision(currencyNum);
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
		else QApplication::beep();
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
				QApplication::beep();
				return;
			}
			referenceValue += inputValue;
			currencyValue = referenceValue * currencyRate * currencyPrecision;
			break;
		case '-':
			if ( isSimpleValue )
			{
				QApplication::beep();
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
				QApplication::beep();
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
				QApplication::beep();
				return;
			}
	}
	resetInput();
	displayNewResult();
	displayMemoryButtons();
}

// Convert to reference, and add or subtract too if needed
void KEuroCalc::ValidateCurrency()
{
	double inputValue, currencyRate, currencyPrecision;

	inputValue = strtod_l( inputDisplay, NULL, c_locale );
	currencyRate = currencies.rate(currencyNum);
	switch (rounding)
	{
		case OFFICIAL_RULES:
			currencyPrecision = currencies.officialRulesPrecision(currencyNum);
			break;
		case SMALLEST_COIN:
			currencyPrecision = currencies.smallestCoinPrecision(currencyNum);
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
		else QApplication::beep();
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
				QApplication::beep();
				return;
			}
			currencyValue += inputValue;
			referenceValue = currencyValue / currencyRate / currencyPrecision;
			break;
		case '-':
			if ( isSimpleValue )
			{
				QApplication::beep();
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
				QApplication::beep();
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
				QApplication::beep();
				return;
			}
	}
	resetInput();
	displayNewResult();
	displayMemoryButtons();
}

// Apply percentage, and add or subtract too if needed
void KEuroCalc::ValidatePercent()
{
	double inputValue, currencyRate, currencyPrecision;

	inputValue = strtod_l( inputDisplay, NULL, c_locale );
	currencyRate = currencies.rate(currencyNum);
	switch (rounding)
	{
		case OFFICIAL_RULES:
			currencyPrecision = currencies.officialRulesPrecision(currencyNum);
			break;
		case SMALLEST_COIN:
			currencyPrecision = currencies.smallestCoinPrecision(currencyNum);
			break;
		default:
			currencyPrecision = 1.0;
	}

	if ( inputDisplay[10] == ' ' )
	{
		QApplication::beep();
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
			QApplication::beep();
			return;
	}
	resetInput();
	displayNewResult();
}

// Enter a simple value, and multiply or divide too if needed.
void KEuroCalc::ValidateSimpleValue()
{
	double inputValue, currencyRate, currencyPrecision;

	inputValue = strtod_l( inputDisplay, NULL, c_locale );
	currencyRate = currencies.rate(currencyNum);
	switch (rounding)
	{
		case OFFICIAL_RULES:
			currencyPrecision = currencies.officialRulesPrecision(currencyNum);
			break;
		case SMALLEST_COIN:
			currencyPrecision = currencies.smallestCoinPrecision(currencyNum);
			break;
		default:
			currencyPrecision = 1.0;
	}

	if ( inputDisplay[10] == ' ' )
	{
		QApplication::beep();
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
				QApplication::beep();
				return;
			}
			break;
		case '-':
			if ( isSimpleValue )
				simpleValue -= inputValue;
			else
			{
				QApplication::beep();
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
				QApplication::beep();
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
			QApplication::beep();
			return;
	}
	resetInput();
	displayNewResult();
	displayMemoryButtons();
}

// Change the sign of the result
void KEuroCalc::ChangeSign()
{
	double currencyRate, currencyPrecision;

	currencyRate = currencies.rate(currencyNum);
	switch (rounding)
	{
		case OFFICIAL_RULES:
			currencyPrecision = currencies.officialRulesPrecision(currencyNum);
			break;
		case SMALLEST_COIN:
			currencyPrecision = currencies.smallestCoinPrecision(currencyNum);
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

// Transfer from display to memory
void KEuroCalc::MemoryInput()
{
	memorySet = true;
	isSimpleMemory = isSimpleValue;
	simpleMemory = simpleValue;
	referenceMemory = referenceValue;

	displayMemoryButtons();
}

// Transfer from memory to display
void KEuroCalc::MemoryRecall()
{
	double currencyRate, currencyPrecision;

	currencyRate = currencies.rate(currencyNum);
	switch (rounding)
	{
		case OFFICIAL_RULES:
			currencyPrecision = currencies.officialRulesPrecision(currencyNum);
			break;
		case SMALLEST_COIN:
			currencyPrecision = currencies.smallestCoinPrecision(currencyNum);
			break;
		default:
			currencyPrecision = 1.0;
	}

	if ( !memorySet )
	{
		QApplication::beep();
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
				QApplication::beep();
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
				QApplication::beep();
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
					QApplication::beep();
					return;
				}
			}
			break;
		case '/':
			if ( isSimpleMemory )
			{
				if ( simpleMemory == 0.0 )
				{
					QApplication::beep();
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
					QApplication::beep();
					return;
				}
			}
	}
	resetInput();
	displayNewResult();
	displayMemoryButtons();
}

// Add memory to display
void KEuroCalc::MemoryPlus()
{
	if ( !memorySet || (isSimpleValue != isSimpleMemory) )
	{
		QApplication::beep();
		return;
	}
	if ( isSimpleMemory )
		simpleMemory += simpleValue;
	else
		referenceMemory += referenceValue;
}

// Subtract memory from display
void KEuroCalc::MemoryMinus()
{
	if ( !memorySet || (isSimpleValue != isSimpleMemory) )
	{
		QApplication::beep();
		return;
	}
	if ( isSimpleMemory )
		simpleMemory -= simpleValue;
	else
		referenceMemory -= referenceValue;
}

// Reset both input, result and memory
void KEuroCalc::Reset()
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
void KEuroCalc::DisplayAbout()
{
	KAboutApplicationDialog *d = new KAboutApplicationDialog(KAboutData::applicationData(), this);
	d->exec();
	delete d;
	AboutButton->setDown( false );
}

// Display help pages
void KEuroCalc::DisplayHelp()
{
	KHelpClient::invokeHelp();
}

// Display settings pages
void KEuroCalc::DisplaySettings()
{
	Preferences *d = new Preferences(this, &currencies);
	d->exec();
	delete d;
	SettingsButton->setDown( false );
}

// Select currency accordingly to drop down list position
void KEuroCalc::SelectCurrency(int position)
{
	int num;
	double currencyRate, currencyPrecision;

	for (num = 0; num < currencies.number(); num++)
		if (currencies.position(num) == position)
			break;
	if ( num == currencies.number() ) return;
	currencyNum = num;

	displayNewCurrency();

	currencyRate = currencies.rate(currencyNum);
	switch (rounding)
	{
		case OFFICIAL_RULES:
			currencyPrecision = currencies.officialRulesPrecision(currencyNum);
			break;
		case SMALLEST_COIN:
			currencyPrecision = currencies.smallestCoinPrecision(currencyNum);
			break;
		default:
			currencyPrecision = 1.0;
	}
	currencyValue = referenceValue * currencyRate * currencyPrecision;
	displayNewResult();
	setFocus();
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
//		case DOLLAR_NY_FRB:
//			SourceLabel->setText( i18n( "NY FRB" ) );
//			DateLabel->setText( i18n( "Loading..." ) );
//			break;
		case EURO_TG:
			SourceLabel->setText( i18n( "TG" ) );
			DateLabel->setText( i18n( "Loading..." ) );
	}
	switch ( rounding )
	{
		case OFFICIAL_RULES:
			RoundingLabel->setText( i18n( "Official rules" ) );
			break;
		case SMALLEST_COIN:
			RoundingLabel->setText( i18n( "Smallest coin" ) );
			break;
		case NO_ROUNDING:
			RoundingLabel->setText( i18n( "No rounding" ) );
	}
	DotButton->setText( QLocale().decimalPoint() );
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
//	ReferenceButton->setText( QString::fromUtf8( reference == DOLLAR_NY_FRB? dollarSymbol: euroSymbol ) );
	ReferenceButton->setText( QString::fromUtf8( euroSymbol ) );
	PercentButton->setText( QString::fromUtf8( "%" ) );
	PlusMinusButton->setText( QString::fromUtf8( "+/-" ) );
}

// Start downloading rates
void KEuroCalc::startDownload()
{
	currencies.clearRates();
	switch (reference)
	{
		case EURO_FIXED:
			currencies.addFixedRates( rounding );
			// endDownload is called here
			break;
		case EURO_ECB:
			currencies.addFixedRates( rounding, true );
			// endDownload is called here for the first time
			CurrencyList->addItem( "--------------------------------------------" );
			currencies.addECBRates( rounding );
			// endDowload is called again when completed
			break;
//		case DOLLAR_NY_FRB:
//			currencies.addNY_FRBRates( rounding );
//			// endDowload is called when completed
//			break;
		case EURO_TG:
			currencies.addFixedRates( rounding, true );
			// endDownload is called here for the first time
			CurrencyList->addItem( "--------------------------------------------" );
			currencies.addTGRates( rounding );
			// endDowload is called again when completed
			break;
	}
}

// Download has ended
void KEuroCalc::endDownload(int defaultCurrency, const QString &date)
{
	int position, num;

	position = CurrencyList->count();
	for (num = 0; num < currencies.number(); ++num)
		if (currencies.position(num) == -2)
	{
		currencies.setPosition(num, position);
		CurrencyList->addItem
			( currencies.code(num) + " - " + currencies.name(num) );
		position++;
	}
	DateLabel->setText( date.isNull()? i18n( "Not loaded" ): date );
	if ( defaultCurrency )
		newRatesList( defaultCurrency );
}

// The rates list has changed, refresh the display
void KEuroCalc::newRatesList(int defaultCurrency)
{
	int position;

	position = currencies.position(currencyNum);
	if (position < 0)					// If current currency does not exist in new rates list, change current currency
	{
		for (currencyNum = 0; currencyNum < currencies.number(); ++currencyNum)
		{
			position = currencies.position(currencyNum);
			if (position == 0) break;
		}
		if ( currencyNum == currencies.number() )	// Handle case of empty list (no currencies at all)
		{
			currencyNum = defaultCurrency;
			currencies.setRate(currencyNum, 1.0);
			position = 0;
			currencies.setPosition(currencyNum, 0);
			CurrencyList->addItem
				( currencies.code(currencyNum) + " - " + currencies.name(currencyNum) );
		}
	}
	CurrencyList->setCurrentIndex( position );
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
				QApplication::beep();
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
				QApplication::beep();
				return;
			}
			if ( c == '.' )
			{
				QApplication::beep();
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
		(QString(inputDisplay).indexOf('.') == -1? beforeUnits: afterUnits);

	displayNewInput();
}

// Input an operator ('+', '-', 'x' or '/')
void KEuroCalc::inputOperator(char c)
{
	if ( inputDisplay[10] != ' '  )
		ValidateSimpleValue();

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
		QString referenceSymbol, currencySymbol;

		if (rounding != NO_ROUNDING)
		{
			roundedReferenceValue = floor(referenceValue * 100.0 + 0.5) / 100.0;
			referenceDisplay.setNum( roundedReferenceValue, 'f', 2 );
		}
		else referenceDisplay.setNum( referenceValue );
		normalize( referenceDisplay );
//		referenceSymbol = QString::fromUtf8( reference == DOLLAR_NY_FRB ? dollarSymbol: euroSymbol );
		referenceSymbol = QString::fromUtf8( euroSymbol );

		switch (rounding)
		{
			case OFFICIAL_RULES:
				currencyPrecision = currencies.officialRulesPrecision(currencyNum);
				roundedCurrencyValue = floor(currencyValue * 100.0 / currencyPrecision + 0.5)
						     / 100.0 * currencyPrecision;
				currencyDisplay.setNum( roundedCurrencyValue, 'f', currencyPrecision < 100.0? 2: 0 );
				break;
			case SMALLEST_COIN:
				currencyPrecision = currencies.smallestCoinPrecision(currencyNum);
				roundedCurrencyValue = floor(currencyValue * 100.0 / currencyPrecision + 0.5)
						     / 100.0 * currencyPrecision;
				currencyDisplay.setNum( roundedCurrencyValue, 'f', currencyPrecision < 100.0? 2: 0 );
				break;
			default:
				currencyDisplay.setNum( currencyValue );
		}
		normalize( currencyDisplay );
		currencySymbol = currencies.symbol(currencyNum);

		ResultDisplay->setText( referenceDisplay + referenceSymbol + '\n' +
					currencyDisplay + ' ' + currencySymbol );
	}
}

// Update the currency button accordingly to new currency
void KEuroCalc::displayNewCurrency()
{
	QString referenceSymbol, currencySymbol;

	currencySymbol = currencies.symbol(currencyNum);
	CurrencyButton->setText( currencySymbol );
	if ( currencies.position(currencyNum) >= 0 )
	{
		double currencyRate, currencyPrecision;
		QString rate;

		currencyRate = currencies.rate(currencyNum);
		switch (rounding)
		{
			case OFFICIAL_RULES:
				currencyPrecision = currencies.officialRulesPrecision(currencyNum);
				break;
			case SMALLEST_COIN:
				currencyPrecision = currencies.smallestCoinPrecision(currencyNum);
				break;
			default:
				currencyPrecision = 1.0;
		}
		rate.setNum( currencyRate * currencyPrecision ); 
//		referenceSymbol = QString::fromUtf8( reference == DOLLAR_NY_FRB ? dollarSymbol: euroSymbol );
		referenceSymbol = QString::fromUtf8( euroSymbol );
		RateLabel->setText
			( '1' + referenceSymbol + " = " + rate + ' ' + currencySymbol ); 
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
	int dotPos = numberDisplay.indexOf('.'),
	    unitPos = (dotPos == -1? numberDisplay.length(): dotPos) - 3;

	if ( dotPos != -1 )
		numberDisplay.replace(	dotPos,
					1,
					QLocale().decimalPoint() );

	while (unitPos > 0)
	{
		numberDisplay.insert(	unitPos,
					" ");
		unitPos -= 3;
	}
}
