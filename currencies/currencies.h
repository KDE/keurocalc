/***************************************************************************
                           currencies.h  -  list of currencies
                             -------------------
    begin                : sam déc  1 23:40:19 CET 2001
    copyright            : (C) 2001-2014 by Éric Bischoff
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

#ifndef CURRENCY_H
#define CURRENCY_H

#include <QString>
#include <QObject>

#include <kio/job.h>

#define EURO_FIXED 0
#define EURO_ECB 1
//#define DOLLAR_NY_FRB 2
#define EURO_TG 3

#define OFFICIAL_RULES 0
#define SMALLEST_COIN 1
#define NO_ROUNDING 2

struct currencyStruc
{
	QString symbol;
	QString code;
	double officialRulesPrecision;
	double smallestCoinPrecision;
	QString name;
	QString newYorkName;
	double fixedRate;
	double rate;
	int position;
};

class Currencies : public QObject
{
    Q_OBJECT 

private:  
    QString variableRates;
    int roundingMethod;
    int numCurrencies;
    int dollarCurrency, euroCurrency;
    currencyStruc *currency;

public:
    Currencies();
    virtual ~Currencies();

    inline int number() const {return numCurrencies;}
    inline int dollar() const {return dollarCurrency;}
    inline int euro() const {return euroCurrency;}
    inline const QString &symbol(int num) const {return currency[num].symbol;}
    inline const QString &code(int num) const {return currency[num].code;}
    inline double officialRulesPrecision(int num) const {return currency[num].officialRulesPrecision;}
    inline double smallestCoinPrecision(int num) const {return currency[num].smallestCoinPrecision;}
    inline const QString &name(int num) const {return currency[num].name;}
    inline const QString &newYorkName(int num) const {return currency[num].newYorkName;}
    inline double fixedRate(int num) const {return currency[num].fixedRate;}
    inline double rate(int num) const {return currency[num].rate;}
    inline void setRate(int num, double rate) {currency[num].rate = rate;}
    inline int position(int num) const {return currency[num].position;}
    inline void setPosition(int num, int position) {currency[num].position = position;}

    bool readCurrencies( const char *filename );
    void clearRates();
    void addFixedRates( int rounding, bool someMoreToCome = false );
    void addECBRates( int rounding );
//  void addNY_FRBRates( int rounding );
    void addTGRates( int rounding );


private slots:
    virtual void httpDataECB(KIO::Job *, const QByteArray &); 
//  virtual void httpDataNY_FRB(KIO::Job *, const QByteArray &); 
    virtual void httpDataTG(KIO::Job *, const QByteArray &); 

signals:
    void endDownload(int defaultCurrency, const QString &date);
};

#endif
