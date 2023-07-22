/***************************************************************************
                         currencies.cpp  -  list of currencies
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

#include "currencies.h"

#include <QFile>
#include <QDate>
#include <QStandardPaths>
#include <QDomDocument>

#include <KLocalizedString>

static const char
	*urlECB = "http://www.ecb.int/stats/eurofxref/eurofxref-daily.xml",
//	*urlNY_FRB = "http://www.newyorkfed.org/markets/fxrates/FXtoXML.cfm?FEXdate=%04d-%02d-%02d&FEXtime=1200",
	*urlTG = "http://rss.timegenie.com/forex2.xml";

// Constructor
Currencies::Currencies()
	: QObject(),
          variableRates()
{
	currency = 0;
}

// Destructor
Currencies::~Currencies()
{
	if (currency) delete [] currency;
}

// Read currencies list
bool Currencies::readCurrencies( const char *filename )
{
	QDomDocument document( "currencies" );
	QFile file( QStandardPaths::locate(QStandardPaths::AppDataLocation, filename) );

	if ( !file.open( QIODevice::ReadOnly ) ) return false;
	if ( !document.setContent( &file ) ) { file.close(); return false; }
	file.close();

	QDomNodeList currenciesList = document.elementsByTagName( "currency" );
	int num;
	QString name;

	numCurrencies = currenciesList.count();
	if (currency) delete [] currency;
	currency = new currencyStruc[numCurrencies];
	for (num = 0; num < numCurrencies; ++num)
	{
		QDomElement elt = currenciesList.item(num).toElement();

		if ( !elt.hasAttribute( "symbol" ) )
			return false;
		currency[num].symbol =  elt.attribute( "symbol" );

		if ( !elt.hasAttribute( "code" ) )
			return false;
		currency[num].code =  elt.attribute( "code" );
		if ( currency[num].code == "USD" )
			dollarCurrency = num;
		else if ( currency[num].code == "EUR" )
			euroCurrency = num;

		if ( !elt.hasAttribute( "official-rules-precision" ) )
			return false;
		currency[num].officialRulesPrecision = elt.attribute( "official-rules-precision" ).toDouble();

		if ( !elt.hasAttribute( "smallest-coin-precision" ) )
			return false;
		currency[num].smallestCoinPrecision = elt.attribute( "smallest-coin-precision" ).toDouble();

		if ( !elt.hasAttribute( "name" ) )
			return false;
		name = elt.attribute( "name" );
		currency[num].name = i18n( name.toUtf8().data() );

		if ( elt.hasAttribute( "new-york-name" ) )
			currency[num].newYorkName = elt.attribute( "new-york-name" );
		else
			currency[num].newYorkName = "N/A";

		if ( elt.hasAttribute( "fixed-rate" ) )
			currency[num].fixedRate = elt.attribute( "fixed-rate" ).toDouble();
		else
			currency[num].fixedRate = 0.0;

		currency[num].rate = 1.0;
		currency[num].position = -1;
	}
	return true;
}

// Clear rates
void Currencies::clearRates()
{
	int num;

	for (num = 0; num < numCurrencies; num++)
	{
		if ( !currency[num].fixedRate )
		{
			currency[num].rate = 1.0;
			currency[num].position = -1;
		}
	}
}

// Add fixed rates
void Currencies::addFixedRates( int rounding, bool someMoreToCome )
{
	int num;
	double currencyPrecision;
	QString blank("");

	for (num = 0; num < numCurrencies; num++)
		if ( currency[num].fixedRate )
	{
		switch (rounding)
		{
			case OFFICIAL_RULES:
				currencyPrecision = currency[num].officialRulesPrecision;
				break;
			case SMALLEST_COIN:
				currencyPrecision = currency[num].smallestCoinPrecision;
				break;
			default:
				currencyPrecision = 1.0;
		}
		currency[num].rate =
			currency[num].fixedRate / currencyPrecision;
		currency[num].position = -2;
	}
	endDownload( someMoreToCome? 0: euroCurrency, blank );
}

// Add variable rates from European Central Bank
void Currencies::addECBRates( int rounding )
{
	KIO::TransferJob *job;

	roundingMethod = rounding;

	job = KIO::get( QUrl( urlECB ), KIO::Reload, KIO::HideProgressInfo );
	connect( job, SIGNAL(data(KIO::Job*,QByteArray)),
		 this, SLOT(httpDataECB(KIO::Job*,QByteArray))
		);
	connect( job, SIGNAL(result(KJob*)),
	         this, SLOT(httpResultECB(KJob*))
	       );
}

// Add variable rates from New York Federal Reserve Bank
//void Currencies::addNY_FRBRates( int rounding )
//{
//	char url[128];
//	QDate yesterday;
//	KIO::TransferJob *job;
//
//	roundingMethod = rounding;
//
//	yesterday = QDate::currentDate().addDays(-1);
//	// This is suboptimal: we should guess the date of latest working day at 12:00 in New York local time
//	// Or much better: use a URL that does not depend on that date...
//	sprintf(url, urlNY_FRB, yesterday.year(), yesterday.month(), yesterday.day());
//	job = KIO::get( QUrl( url ), KIO::Reload, KIO::HideProgressInfo );
//	connect( job, SIGNAL(data(KIO::Job*,QByteArray)),
//		 this, SLOT(httpDataNY_FRB(KIO::Job*,QByteArray))
//		);
//	connect( job, SIGNAL(result(KJob*)),
//	         this, SLOT(httpResultNY_FRB(KJob*))
//	       );
//}

// Add variable rates from Time Genie foreign exchange
void Currencies::addTGRates( int rounding )
{
	KIO::TransferJob *job;

	roundingMethod = rounding;

	job = KIO::get( QUrl( urlTG ), KIO::Reload, KIO::HideProgressInfo );
	connect( job, SIGNAL(data(KIO::Job*,QByteArray)),
		 this, SLOT(httpDataTG(KIO::Job*,QByteArray))
		);
	connect( job, SIGNAL(result(KJob*)),
	         this, SLOT(httpResultTG(KJob*))
	       );
}

// Exchange rates received from European Central Bank
void Currencies::httpDataECB(KIO::Job *job, const QByteArray &array)
{
	(void) job; // Unused parameter

	if ( array.size() )
	{
		variableRates += QString(array);
	}
	else
	{
		QDomDocument document( "rates" );

		document.setContent( variableRates, true );

		QDomNodeList ratesList = document.elementsByTagName( "Cube" );
		int num;
		double currencyPrecision;

		for (int i = 0; i < ratesList.count(); i++)
		{
			QDomElement elt = ratesList.item(i).toElement();
			if ( elt.hasAttribute( "time" ) )
				date = elt.attribute( "time" );
			else if ( elt.hasAttribute( "currency" ) && elt.hasAttribute( "rate" ) )
			{
				QString code(elt.attribute( "currency" ));
				for (num = 0; num < numCurrencies; ++num)
					if ( code == currency[num].code )
						break;
				if ( num < numCurrencies )
				{
					if ( !currency[num].fixedRate )
					{
						switch (roundingMethod)
						{
							case OFFICIAL_RULES:
								currencyPrecision = currency[num].officialRulesPrecision;
								break;
							case SMALLEST_COIN:
								currencyPrecision = currency[num].smallestCoinPrecision;
								break;
							default:
								currencyPrecision = 1.0;
						}
						currency[num].rate =
							elt.attribute( "rate" ).toDouble() / currencyPrecision;
						currency[num].position = -2;
					}
				}
			}
		}
		variableRates = "";
	}
}

// End of exchange rates from European Central Bank
void Currencies::httpResultECB(KJob *job)
{
	if ( job->error() )
	{
		clearRates();
		endDownload( euroCurrency, "ERROR" );
	}
	else
		endDownload( euroCurrency, date );
}

// Exchange rates received from New York Federal Reserve Bank
//void Currencies::httpDataNY_FRB(KIO::Job *job, const QByteArray &array)
//{
//	static const char *frbny = "http://www.newyorkfed.org/xml/schemas/FX/utility";
//
//	(void) job; // Unused parameter
//
//	if ( array.size() )
//	{
//		variableRates += QString(array);
//	}
//	else
//	{
//		QDomDocument document( "rates" );
//
//		document.setContent( variableRates, true );
//
//		QDomNodeList ratesList = document.elementsByTagNameNS( frbny, "Series" );
//		int num;
//		double currencyPrecision;
//
//		for (int i = 0; i < ratesList.count(); i++)
//		{
//			QDomElement elt = ratesList.item(i).toElement();
//			QDomNodeList dateElements = elt.elementsByTagNameNS( frbny, "TIME_PERIOD" ),
//				     currencyElements = elt.elementsByTagNameNS( frbny, "CURR" ),
//				     valueElements = elt.elementsByTagNameNS( frbny, "OBS_VALUE" );
//			if (dateElements.count() == 1 && currencyElements.count() == 1 && valueElements.count() == 1)
//			{
//				QDomElement dateElement = dateElements.item(0).toElement(),
//					    currencyElement = currencyElements.item(0).toElement(),
//					    valueElement = valueElements.item(0).toElement();
//				QString code(
//					elt.attribute( "UNIT" ) == "USD" ?
//					currencyElement.text():
//					elt.attribute( "UNIT" )
//				            );
//				for (num = 0; num < numCurrencies; num++)
//					if ( code == currency[num].code )
//						break;
//				if ( num < numCurrencies )
//				{
//					if ( !currency[num].fixedRate )
//					{
//						switch (roundingMethod)
//						{
//							case OFFICIAL_RULES:
//								currencyPrecision = currency[num].officialRulesPrecision;
//								break;
//							case SMALLEST_COIN:
//								currencyPrecision = currency[num].smallestCoinPrecision;
//								break;
//							default:
//								currencyPrecision = 1.0;
//						}
//						if ( elt.attribute( "UNIT" ) != "USD" )
//							currency[num].rate =
//								valueElement.text().toDouble() / currencyPrecision;
//						else currency[num].rate =
//							( 1.0 / valueElement.text().toDouble() ) / currencyPrecision;
//						currency[num].position = -2;
//					}
//					if ( date.isNull() )
//						date = dateElement.text();
//				}
//			}
//		}
//		variableRates = "";
//	}
//}

// End of exchange rates from New York Federal Reserve Bank
//void Currencies::httpResultNY_FRB(KJob *job)
//{
//	if ( job->error() )
//	{
//		clearRates();
//		endDownload( dollarCurrency, "ERROR" );
//	}
//	else
//		endDownload( dollarCurrency, date );
//}

// Exchange rates received from Time Genie foreign exchange
void Currencies::httpDataTG(KIO::Job *job, const QByteArray &array)
{
	(void) job; // Unused parameter

	if ( array.size() )
	{
		variableRates += QString(array);
	}
	else
	{
		QDomDocument document( "forex" );

		document.setContent( variableRates, true );

		QDomNodeList ratesList = document.elementsByTagName( "currency" );
		int num;
		double currencyPrecision;

		for (int i = 0; i < ratesList.count(); i++)
		{
			QDomElement elt = ratesList.item(i).toElement();
			if ( elt.hasAttribute( "code" ) && elt.hasAttribute( "rate" ) )
			{
				QString code(elt.attribute( "code" ));
				for (num = 0; num < numCurrencies; ++num)
					if ( code == currency[num].code )
						break;
				if ( num < numCurrencies )
				{
					if ( !currency[num].fixedRate )
					{
						switch (roundingMethod)
						{
							case OFFICIAL_RULES:
								currencyPrecision = currency[num].officialRulesPrecision;
								break;
							case SMALLEST_COIN:
								currencyPrecision = currency[num].smallestCoinPrecision;
								break;
							default:
								currencyPrecision = 1.0;
						}
						currency[num].rate =
							elt.attribute( "rate" ).toDouble() / currencyPrecision;
						currency[num].position = -2;
					}
				}
			}
		}
		variableRates = "";
		date = QDate::currentDate().toString("yyyy-MM-dd");   // TG does not provide time information, use current date
	}
}

// End of exchange rates from Time Genie foreign exchange
void Currencies::httpResultTG(KJob *job)
{
	if ( job->error() )
	{
		clearRates();
		endDownload( euroCurrency, "ERROR" );
	}
	else
		endDownload( euroCurrency, date );

}

#include "moc_currencies.cpp"
