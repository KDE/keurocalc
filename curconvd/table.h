/***************************************************************************
                          table.h  -  conversion table
                             -------------------
    begin                : jeu nov 23 21:03:27 CET 2006
    copyright            : (C) 2006-2018 by Éric Bischoff
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

#ifndef CURRENCYTABLE_H
#define CURRENCYTABLE_H

#include "../currencies/currencies.h"

class CurrencyTable : public Currencies
{
    Q_OBJECT 

public:
    explicit CurrencyTable(const QString &tablePath);
    virtual ~CurrencyTable();
    void loadSource(const QString &dataSource, const QString &roundingMethod);

public slots:
    Q_SCRIPTABLE QStringList AvailableCurrencies();
    Q_SCRIPTABLE QString Symbol(const QString &currencyCode);
    Q_SCRIPTABLE QString Name(const QString &currencyCode);
    Q_SCRIPTABLE double ConvertFromReference(const QString &currencyCode, double referenceValue);
    Q_SCRIPTABLE double ConvertToReference(const QString &currencyCode, double currencyValue);
    Q_SCRIPTABLE void Unload();

    void endDownload(int defaultCurrency, const QString &date);

private:
    QString path;			// Object path
    int rounding;			// Rounding method
};

#endif
