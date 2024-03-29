/***************************************************************************
                          table.cpp  -  conversion table
                             -------------------
    begin                : jeu nov 23 21:03:30 CET 2006
    copyright            : (C) 2006-2022 by Éric Bischoff
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

#include "table.h"

#include <QDBusConnection>
#include <QDebug>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Constructor
CurrencyTable::CurrencyTable(const QString &tablePath)
	: QObject(),
	  path( tablePath ),
	  currencies(),
	  rounding( NO_ROUNDING )
{
	QDBusConnection::sessionBus().registerObject(path, this, QDBusConnection::ExportScriptableSlots);

    qDebug() << QStringLiteral("curconvd: table \"%1\" created\n").arg(path);
}

// Destructor
CurrencyTable::~CurrencyTable()
{
	QDBusConnection::sessionBus().unregisterObject(path);

    qDebug() << QStringLiteral("curconvd: table \"%1\" destroyed\n").arg(path);
}

// Load currencies from data source
void CurrencyTable::loadSource(const QString &dataSource, const QString &roundingMethod)
{
	if ( !currencies.readCurrencies( "currencies.xml" ) )
	{
		fprintf(stderr, "Cannot load currencies.xml" );
		exit(1);
// TODO: Less violent exit
	}
	currencies.clearRates();

	if (roundingMethod == "official rules")
		rounding = OFFICIAL_RULES;
	else if (roundingMethod == "smallest coin")
		rounding = SMALLEST_COIN;
	else
		rounding = NO_ROUNDING;

        connect( &currencies, SIGNAL(endDownload(int,QString)),
                 this, SLOT(endDownload(int,QString))
               );

	if (dataSource == "(fixed)")
	{
		currencies.addFixedRates( rounding );
		// endDownload is called here
	}
	else if (dataSource == "http://www.ecb.int")
	{
		currencies.addFixedRates( rounding, true );
		// endDownload is called here for the first time
		currencies.addECBRates( rounding );
		// endDowload is called again when completed
	}
//	else if (dataSource == "http://www.newyorkfed.org")
//	{
//		currencies.addNY_FRBRates( rounding );
//		// endDowload is called when completed
//	}
	else if (dataSource == "http://rss.timegenie.com")
	{
		currencies.addFixedRates( rounding, true );
		// endDownload is called here for the first time
		currencies.addTGRates( rounding );
		// endDowload is called again when completed
	}
}

// Download has ended
void CurrencyTable::endDownload(int defaultCurrency, const QString &date)
{
(void) defaultCurrency;
(void) date;
qDebug() << "curconvd: end of download not implemented yet!";
// TODO: if defaultCurrency is not null, inform that a result is available
}

// List currencies handled
QStringList CurrencyTable::AvailableCurrencies()
{
	QStringList codes;

	printf( "curconvd: %s/AvailableCurrencies()\n",
		path.toUtf8().data());

	for (int num = 0; num < currencies.number(); num++)
		if ( currencies.position(num) == -2 )
			codes << QString(currencies.code(num));

	return codes;
}

// Gives the everyday symbol for a given currency code 
QString CurrencyTable::Symbol(const QString &currencyCode)
{
	QString currencySymbol;
	int num;

	printf( "curconvd: %s/Symbol(\"%s\")\n",
		path.toUtf8().data(),
		currencyCode.toUtf8().data());

	for (num = 0; num < currencies.number(); ++num)
		if ( currencies.code(num) == currencyCode )
			break;
	if (num == currencies.number())
		currencySymbol = ""; // TODO: return an error code here
	else
		currencySymbol = currencies.symbol(num);

	return currencySymbol;
}

// Gives the description for a given currency code
QString CurrencyTable::Name(const QString &currencyCode)
{
	QString currencyName;
	int num;

	printf( "curconvd: %s/Name(\"%s\")\n",
		path.toUtf8().data(),
		currencyCode.toUtf8().data());

	for (num = 0; num < currencies.number(); ++num)
		if ( currencies.code(num) == currencyCode )
			break;
	if (num == currencies.number())
		currencyName = ""; // TODO: return an error code here
	else
		currencyName = currencies.name(num);

	return currencyName;
}

// Convert an amount expressed in the reference currency
double CurrencyTable::ConvertFromReference(const QString &currencyCode, double referenceValue)
{
	double currencyValue;
	int num;

	printf( "curconvd: %s/ConvertFromReference(\"%s\", %g)\n",
		path.toUtf8().data(),
		currencyCode.toUtf8().data(),
		referenceValue);

	for (num = 0; num < currencies.number(); ++num)
		if ( currencies.code(num) == currencyCode )
			break;
	if (num == currencies.number())
		currencyValue = 0.0; // TODO: return an error code here
	else
	{
		double currencyRate, currencyPrecision;

		currencyRate = currencies.rate(num);
		switch (rounding)
		{
			case OFFICIAL_RULES:
				currencyPrecision = currencies.officialRulesPrecision(num);
				currencyValue = floor(referenceValue * currencyRate * 100.0 + 0.5)
						     / 100.0 * currencyPrecision;
				break;
			case SMALLEST_COIN:
				currencyPrecision = currencies.smallestCoinPrecision(num);
				currencyValue = floor(referenceValue * currencyRate * 100.0 + 0.5)
						     / 100.0 * currencyPrecision;
				break;
			default:
				currencyValue = referenceValue * currencyRate;
		}
	}

	return currencyValue;
}

// Convert an amount expressed in the given currency
double CurrencyTable::ConvertToReference(const QString &currencyCode, double currencyValue)
{
	double referenceValue;
	int num;

	printf( "curconvd: %s/ConvertToReference(\"%s\", %g)\n",
		path.toUtf8().data(),
		currencyCode.toUtf8().data(),
		currencyValue);

	for (num = 0; num < currencies.number(); ++num)
		if ( currencies.code(num) == currencyCode )
			break;
	if (num == currencies.number())
		referenceValue = 0.0; // TODO: return an error code here
	else
	{
		double currencyRate, currencyPrecision;

		currencyRate = currencies.rate(num);
		switch (rounding)
		{
			case OFFICIAL_RULES:
				currencyPrecision = currencies.officialRulesPrecision(num);
				referenceValue = floor( currencyValue / currencyRate / currencyPrecision
							 * 100.0 + 0.5) / 100.0;
				break;
			case SMALLEST_COIN:
				currencyPrecision = currencies.smallestCoinPrecision(num);
				referenceValue = floor( currencyValue / currencyRate / currencyPrecision
							 * 100.0 + 0.5) / 100.0;
				break;
			default:
				referenceValue = currencyValue / currencyRate;
		}
	}

	return referenceValue;
}

// Unload this currencies table
void CurrencyTable::Unload()
{
    qDebug() << QStringLiteral("curconvd: %1/Unload()").arg(path);

	delete this;
}

#include "moc_table.cpp"
