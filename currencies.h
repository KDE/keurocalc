/***************************************************************************
                           currencies.h  -  list of currencies
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

#ifndef CURRENCY_H
#define CURRENCY_H

#define EURO_FIXED 0
#define EURO_ECB 1
#define DOLLAR_NY_FRB 2

#define CURRENCIES 49
#define FIXED_CURRENCIES 12
#define EURO_CURRENCY 14
#define DOLLAR_CURRENCY 46

struct currencyStruc {
	double fixedRate;
	const char *symbol;
	const char *code;
	double officialRulesPrecision;
	double smallestCoinPrecision;
	const char *name;
	const char *newYorkName;
	double rate;
	int position;
};

#define OFFICIAL_RULES 0
#define SMALLEST_COIN 1
#define NO_ROUNDING 2

#endif

