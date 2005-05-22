/***************************************************************************
                           currencies.h  -  list of currencies
                             -------------------
    begin                : sam déc  1 23:40:19 CET 2001
    copyright            : (C) 2001-2005 by Éric Bischoff
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

#include <qstring.h>

#define EURO_FIXED 0
#define EURO_ECB 1
#define DOLLAR_NY_FRB 2

#define OFFICIAL_RULES 0
#define SMALLEST_COIN 1
#define NO_ROUNDING 2

struct currencyStruc {
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

#endif

