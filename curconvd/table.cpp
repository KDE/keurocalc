/***************************************************************************
                          table.cpp  -  conversion table
                             -------------------
    begin                : jeu nov 23 21:03:30 CET 2006
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

#include <QDBusConnection>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "table.h"
#include "table.moc"

// Constructor
CurrencyTable::CurrencyTable(const QString &tablePath)
	: Currencies(),
	  path( tablePath ),
	  rounding( NO_ROUNDING )
{
	QDBusConnection::sessionBus().registerObject(path, this, QDBusConnection::ExportScriptableSlots);

	printf( "curconvd: table \"%s\" created\n",
		path.toUtf8().data());
}

// Destructor
CurrencyTable::~CurrencyTable()
{
	QDBusConnection::sessionBus().unregisterObject(path);

	printf( "curconvd: table \"%s\" destroyed\n",
		path.toUtf8().data());
}

// Load currencies from data source
void CurrencyTable::loadSource(const QString &dataSource, const QString &roundingMethod)
{
	if ( !readCurrencies( "currencies.xml" ) )
	{
		fprintf(stderr, "Cannot load currencies.xml" );
		exit(1);
// TODO: Less violent exit
	}
	clearRates();

	if (roundingMethod == "official rules")
		rounding = OFFICIAL_RULES;
	else if (roundingMethod == "smallest coin")
		rounding = SMALLEST_COIN;
	else
		rounding = NO_ROUNDING;

	if (dataSource == "(fixed)")
	{
		addFixedRates( rounding );
		// endDownload is called here
	}
	else if (dataSource == "http://www.ecb.int")
	{
		addFixedRates( rounding, true );
		// endDownload is called here for the first time
		addECBRates( rounding );
		// endDowload is called again when completed
	}
//	else if (dataSource == "http://www.newyorkfed.org")
//	{
//		addNY_FRBRates( rounding );
//		// endDowload is called when completed
//	}
	else if (dataSource == "http://rss.timegenie.com")
	{
		addFixedRates( rounding, true );
		// endDownload is called here for the first time
		addECBRates( rounding );
		// endDowload is called again when completed
	}
}

// Download has ended
void CurrencyTable::endDownload(int defaultCurrency, const QString &date)
{
printf("Here\n");
// TODO: if defaultCurrency is not null, inform that a result is available
}

// List currencies handled
QStringList CurrencyTable::AvailableCurrencies()
{
	QStringList codes;

	printf( "curconvd: %s/AvailableCurrencies()\n",
		path.toUtf8().data());

	for (int num = 0; num < number(); num++)
		if ( position(num) == -2 )
			codes << QString(code(num));

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

	for (num = 0; num < number(); num++)
		if ( code(num) == currencyCode )
			break;
	if (num == number())
		currencySymbol = ""; // TODO: return an error code here
	else
		currencySymbol = symbol(num);

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

	for (num = 0; num < number(); num++)
		if ( code(num) == currencyCode )
			break;
	if (num == number())
		currencyName = ""; // TODO: return an error code here
	else
		currencyName = name(num);

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

	for (num = 0; num < number(); num++)
		if ( code(num) == currencyCode )
			break;
	if (num == number())
		currencyValue = 0.0; // TODO: return an error code here
	else
	{
		double currencyRate, currencyPrecision;

		currencyRate = rate(num);
		switch (rounding)
		{
			case OFFICIAL_RULES:
				currencyPrecision = officialRulesPrecision(num);
				currencyValue = floor(referenceValue * currencyRate * 100.0 + 0.5)
						     / 100.0 * currencyPrecision;
				break;
			case SMALLEST_COIN:
				currencyPrecision = smallestCoinPrecision(num);
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

	for (num = 0; num < number(); num++)
		if ( code(num) == currencyCode )
			break;
	if (num == number())
		referenceValue = 0.0; // TODO: return an error code here
	else
	{
		double currencyRate, currencyPrecision;

		currencyRate = rate(num);
		switch (rounding)
		{
			case OFFICIAL_RULES:
				currencyPrecision = officialRulesPrecision(num);
				referenceValue = floor( currencyValue / currencyRate / currencyPrecision
							 * 100.0 + 0.5) / 100.0;
				break;
			case SMALLEST_COIN:
				currencyPrecision = smallestCoinPrecision(num);
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
	printf( "curconvd: %s/Unload()\n",
		path.toUtf8().data());

	delete this;
}
