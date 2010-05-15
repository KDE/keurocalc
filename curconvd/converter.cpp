/***************************************************************************
                          converter.cpp  -  d-bus service
                             -------------------
    begin                : lun nov 13 11:22:05 CET 2006
    copyright            : (C) 2006-2010 by Éric Bischoff
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

#include <stdio.h>

#include <QDBusConnection>
#include <QStringList>

#include "converter.h"
#include "converter.moc"
#include "table.h"

// Constructor
CurrencyConverter::CurrencyConverter()
	: QObject(),
	  serialNumber(0)
{
	QDBusConnection::sessionBus().registerObject("/CurrencyConverter", this, QDBusConnection::ExportScriptableSlots);

	printf( "curconvd: waiting for D-Bus requests\n" );
}

// Destructor
CurrencyConverter::~CurrencyConverter()
{
	QDBusConnection::sessionBus().unregisterObject("/CurrencyConverter");

	printf( "curconvd: stopped waiting for D-Bus requests\n" );
}

// List available data sources
QStringList CurrencyConverter::DataSources()
{
	QStringList dataSources;

	printf( "curconvd: /CurrencyConverter/DataSources()\n" );

	dataSources << QString("(fixed)");
        dataSources << QString("http://www.ecb.int");
	dataSources << QString("http://www.ny.frb.org");

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
	else if (dataSource == "http://www.ny.frb.org")
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
