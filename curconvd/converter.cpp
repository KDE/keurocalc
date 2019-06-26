/***************************************************************************
                          converter.cpp  -  d-bus service
                             -------------------
    begin                : lun nov 13 11:22:05 CET 2006
    copyright            : (C) 2006-2019 by Éric Bischoff
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

#include "converter.h"

#include <stdio.h>

#include <QDBusConnection>
#include <QStringList>

#include <KLocalizedString>

#include "table.h"

// Constructor
CurrencyConverter::CurrencyConverter()
	: QObject(),
	  serialNumber(0)
{
	QDBusConnection::sessionBus().registerService("org.kde.curconvd");
	QDBusConnection::sessionBus().registerObject("/CurrencyConverter", this, QDBusConnection::ExportScriptableSlots);

	printf( "%s\n", (const char *) i18n("curconvd: waiting for D-Bus requests").toUtf8() );
}

// Destructor
CurrencyConverter::~CurrencyConverter()
{
	QDBusConnection::sessionBus().unregisterObject("/CurrencyConverter");
	QDBusConnection::sessionBus().unregisterService("org.kde.curconvd");

	printf( "%s\n", (const char *) i18n("curconvd: stopped waiting for D-Bus requests").toUtf8() );
}

// List available data sources
QStringList CurrencyConverter::DataSources()
{
	QStringList dataSources;

	printf( "curconvd: /CurrencyConverter/DataSources()\n" );

	dataSources << QString("(fixed)");
        dataSources << QString("http://www.ecb.int");
//	dataSources << QString("http://www.newyorkfed.org");
	dataSources << QString("http://rss.timegenie.com");

	return dataSources;
}

// List available rounding methods
QStringList CurrencyConverter::RoundingMethods()
{
	QStringList roundingMethods;

	printf( "curconvd: /CurrencyConverter/RoundingMethods()\n" );

	roundingMethods << QString("none");
        roundingMethods << QString("official rules");
	roundingMethods << QString("smallest coin");

	return roundingMethods;
}

// Gives the reference currency for a data source
QString CurrencyConverter::ReferenceCurrency(const QString &dataSource)
{
	QString reference;

	printf( "curconvd: /CurrencyConverter/ReferenceCurrency(\"%s\")\n",
		dataSource.toUtf8().data() );

	if (dataSource == "(fixed)")
		reference = "EUR";
	else if (dataSource == "http://www.ecb.int")
		reference = "EUR";
//	else if (dataSource == "http://www.newyorkfed.org")
//		reference = "USD";
	else if (dataSource == "http://rss.timegenie.com")
		reference = "USD";
	else reference = "";

	return reference;
}

// Load currencies from data source
QString CurrencyConverter::LoadSource(const QString &dataSource, const QString &roundingMethod)
{
	QString tablePath;
	CurrencyTable *table;

	printf( "curconvd: /CurrencyConverter/LoadSource(\"%s\", \"%s\")\n",
		dataSource.toUtf8().data(),
		roundingMethod.toUtf8().data() );

	tablePath = QString( "/CurrencyConverter/tables/%1" ).arg( ++serialNumber );

	table = new CurrencyTable(tablePath);
	table->loadSource(dataSource, roundingMethod);

	return tablePath;
}
