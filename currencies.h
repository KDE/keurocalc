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

#define CURRENCIES 49
#define FIXED_CURRENCIES 12
#define EURO_CURRENCY FIXED_CURRENCIES
#define DOLLAR_CURRENCY (FIXED_CURRENCIES + 1)
#define VARIABLE_CURRENCIES 37

struct currencyStruc {
	double rate;
	const char *symbol;
	const char *code;
	double rulesPrecision;
	double smallestCoinPrecision;
	const char *name;
	const char *newYorkName;
	int position;
};

#endif

