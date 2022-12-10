/***************************************************************************
                          keurocalc.h  -  main widget
                             -------------------
    begin                : sam déc  1 23:40:19 CET 2001
    copyright            : (C) 2001-2022 by Éric Bischoff
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

#ifndef KEUROCALC_H
#define KEUROCALC_H

#include <QDialog>

#include "ui_calculator.h"
#include "../currencies/currencies.h"

class KEuroCalc : public QDialog, public Ui::Calculator
{
    Q_OBJECT 

public:
    explicit KEuroCalc(QWidget* parent=0);
    virtual ~KEuroCalc();
    virtual void keyPressEvent( QKeyEvent *e ) override;
    bool readSplashScreen() const;
    void readOptions(int &oldReference, int &oldCurrency, int &oldRounding, QColor &oldDisplayColor, bool &oldSplashScreen) const;
    void writeOptions(int newReference, int newCurrency, int newRounding, const QColor &newDisplayColor, bool newSplashScreen);

    void setPreferences(int newReference, int newCurrency, int newRounding, const QColor &newDisplayColor, bool newSplashScreen);

public slots:
    Q_SCRIPTABLE void InputDot();
    Q_SCRIPTABLE void InputZero();
    Q_SCRIPTABLE void InputOne();
    Q_SCRIPTABLE void InputTwo();
    Q_SCRIPTABLE void InputThree();
    Q_SCRIPTABLE void InputFour();
    Q_SCRIPTABLE void InputFive();
    Q_SCRIPTABLE void InputSix();
    Q_SCRIPTABLE void InputSeven();
    Q_SCRIPTABLE void InputEight();
    Q_SCRIPTABLE void InputNine();
    Q_SCRIPTABLE void InputPlus();
    Q_SCRIPTABLE void InputMinus();
    Q_SCRIPTABLE void InputAsterisk();
    Q_SCRIPTABLE void InputSlash();
    Q_SCRIPTABLE void InputBackspace();
    Q_SCRIPTABLE void ValidateReference();
    Q_SCRIPTABLE void ValidateCurrency();
    Q_SCRIPTABLE void ValidatePercent();
    Q_SCRIPTABLE void ValidateSimpleValue();
    Q_SCRIPTABLE void ChangeSign();
    Q_SCRIPTABLE void MemoryInput();
    Q_SCRIPTABLE void MemoryRecall();
    Q_SCRIPTABLE void MemoryPlus();
    Q_SCRIPTABLE void MemoryMinus();
    Q_SCRIPTABLE void Reset();
    Q_SCRIPTABLE void DisplayAbout();
    Q_SCRIPTABLE void DisplayHelp();
    Q_SCRIPTABLE void DisplaySettings();
    Q_SCRIPTABLE void SelectCurrency(int position);

    void endDownload(int defaultCurrency, const QString &date);

private:  
    char operatorDisplay[2],
         inputDisplay[12];
    enum { beforeUnits,
	   atUnits,
	   afterUnits
    } inputPos;

    locale_t c_locale;
    bool isSimpleValue;
    double simpleValue,
           referenceValue,
           currencyValue;
    bool memorySet,
         isSimpleMemory;
    double simpleMemory,
           referenceMemory;

    int reference;
    int currencyNum;
    int rounding;
    QColor displayColor;
    bool splashScreen;

    Currencies currencies;

    void initButtons();
    void startDownload();
    void newRatesList(int defaultCurrency); 

    void inputDigit( char c );
    void inputCorrect();
    void inputOperator( char c );
    void resetInput();
    void displayNewInput();
    void displayNewResult();
    void displayNewCurrency();
    void displayMemoryButtons();
    void normalize( QString &numberDisplay );
};

#endif
