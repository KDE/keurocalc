/***************************************************************************
                          converter.h  -  d-bus service
                             -------------------
    begin                : lun nov 13 11:28:34 CET 2006
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

#ifndef CURRENCYCONVERTER_H
#define CURRENCYCONVERTER_H

#include <QtCore/QObject>

class CurrencyConverter : public QObject
{
    Q_OBJECT 

public:
    CurrencyConverter();
    virtual ~CurrencyConverter();

public slots:
    Q_SCRIPTABLE QStringList DataSources();
    Q_SCRIPTABLE QStringList RoundingMethods();
    Q_SCRIPTABLE QString ReferenceCurrency(const QString &dataSource);
    Q_SCRIPTABLE QString LoadSource(const QString &dataSource, const QString &roundingMethod);

private:
    unsigned int serialNumber;		// Serial number of next table
};

#endif
