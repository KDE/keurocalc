/***************************************************************************
                          keurocalc.h  -  main widget
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

#ifndef KEURO_H
#define KEURO_H

class KLocale;

#include <kapp.h>
#include <kio/job.h>

#include <qwidget.h>

#include "calculator.h"

class KEuroCalc : public Calculator
{
  Q_OBJECT 

public:
    KEuroCalc(QWidget* parent=0, const char *name=0);
    ~KEuroCalc();
    virtual void keyPressEvent( QKeyEvent *e );    
    inline int getReference() const {return reference;}
    void setPreferences(int newReference);

public slots:
    virtual void httpData(KIO::Job *, const QByteArray &); 
    virtual void httpResultECB(KIO::Job *); 
    virtual void httpResultNYFRB(KIO::Job *); 
    virtual void inputDot();
    virtual void inputZero();
    virtual void inputOne();
    virtual void inputTwo();
    virtual void inputThree();
    virtual void inputFour();
    virtual void inputFive();
    virtual void inputSix();
    virtual void inputSeven();
    virtual void inputEight();
    virtual void inputNine();
    virtual void inputPlus();
    virtual void inputMinus();
    virtual void inputAsterisk();
    virtual void inputSlash();
    virtual void inputBackspace();
    virtual void validateReference();
    virtual void validateCurrency();
    virtual void validatePercent();
    virtual void validateSimpleValue();
    virtual void changeSign();
    virtual void memoryInput();
    virtual void memoryRecall();
    virtual void memoryPlus();
    virtual void memoryMinus();
    virtual void reset();
    virtual void displayAbout();
    virtual void displayHelp();
    virtual void displaySettings();
    virtual void selectCurrency(int position);

private:  
    QString variableRates;
    char operatorDisplay[2],
         inputDisplay[12];
    enum { beforeUnits,
	   atUnits,
	   afterUnits
    } inputPos;

    bool isSimpleValue;
    double simpleValue,
           referenceValue,
           currencyValue;
    bool memorySet,
         isSimpleMemory;
    double simpleMemory,
           referenceMemory;

    int currencyNum;
    enum referenceMode { euroFixed,
	   euroECB,
	   dollarNYFRB
    } reference;

    void readOptions();
    void writeOptions();
    void initButtons();
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
