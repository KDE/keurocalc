/***************************************************************************
                         currencies.cpp  -  list of currencies
                             -------------------
    begin                : sam d�c  1 23:40:19 CET 2001
    copyright            : (C) 2001-2004 by �ric Bischoff
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

#include <klocale.h>

#include "currencies.h"

currencyStruc currency[CURRENCIES] =
{
	{ 13.7603,  "öS", "ATS",   1.0,  10.0, I18N_NOOP("Austrian schilling"), "N/A", 1.0, -1 }, // 0
	{     1.0,   "A$", "AUD",   1.0,   1.0, I18N_NOOP("Australian dollar"), "Dollar/Australia", 1.0, -1 }, // 1
	{ 40.3399,   "FB", "BEF",   1.0,  10.0, I18N_NOOP("Belgian franc"), "N/A", 1.0, -1 }, // 2
	{     1.0,   "Lv", "BGN",   1.0,   1.0, I18N_NOOP("Bulgarian lev"), "N/A", 1.0, -1 }, // 3
	{     1.0,   "R$", "BRL",   1.0,   1.0, I18N_NOOP("Brazilian real"), "Real/Brazil", 1.0, -1 }, // 4
	{     1.0, "Can$", "CAD",   1.0,   1.0, I18N_NOOP("Canadian dollar"), "Dollar/Canada", 1.0, -1 }, // 5
	{     1.0,   "FS", "CHF",   1.0,   1.0, I18N_NOOP("Swiss franc"), "Franc/Switzerland", 1.0, -1 }, // 6
	{     1.0,    "Y", "CNY",   1.0,   1.0, I18N_NOOP("Chinese yuan"), "Yuan/China, P.R.", 1.0, -1 }, // 7
	{     1.0,  "£C", "CYP",   1.0,   1.0, I18N_NOOP("Cyprus pound"), "N/A", 1.0, -1 }, // 8
	{     1.0,    "K", "CZK",   1.0,   1.0, I18N_NOOP("Czech koruna"), "N/A", 1.0, -1 }, // 9
	{ 1.95583,   "DM", "DEM",   1.0,   1.0, I18N_NOOP("German deutschmark"), "N/A", 1.0, -1 }, // 10
	{     1.0,  "DKr", "DKK",   1.0,   1.0, I18N_NOOP("Danish krone"), "Krone/Denmark", 1.0, -1 }, // 11
	{     1.0,  "EKr", "EEK",   1.0,   1.0, I18N_NOOP("Estonian kroon"), "N/A", 1.0, -1 }, // 12
	{ 1.66386,  "Pts", "ESP", 100.0, 100.0, I18N_NOOP("Spanish peseta"), "N/A", 1.0, -1 }, // 13
	{     1.0," €", "EUR",   1.0,   1.0, I18N_NOOP("EU euro"), "Euro/European Monetary Union", 1.0, -1 }, // 14
	{ 5.94573,   "mk", "FIM",   1.0,   1.0, I18N_NOOP("Finnish markka"), "N/A", 1.0, -1 }, // 15
	{ 6.55957,   "FF", "FRF",   1.0,   1.0, I18N_NOOP("French franc"), "N/A", 1.0, -1 }, // 16
	{     1.0,   "£", "GBP",   1.0,   1.0, I18N_NOOP("Great Britain pound"), "Pound/United Kingdom", 1.0, -1 }, // 17
	{ 3.40750,   "Dr", "GRD", 100.0, 100.0, I18N_NOOP("Greek drachma"), "N/A", 1.0, -1 }, // 18
	{     1.0,  "HK$", "HKD",   1.0,   1.0, I18N_NOOP("Hong kong dollar"), "Dollar/Hong Kong", 1.0, -1 }, // 19
	{     1.0,   "Ht", "HUF", 100.0, 100.0, I18N_NOOP("Hungarian forint"), "N/A", 1.0, -1 }, // 20
	{ .787564, "£IR", "IEP",   1.0,   1.0, I18N_NOOP("Irish punt"), "N/A", 1.0, -1 }, // 21
	{     1.0,   "Rs", "INR",   1.0,   1.0, I18N_NOOP("Indian rupee"), "Rupee/India", 1.0, -1 }, // 22
	{     1.0,  "Ikr", "ISK",   1.0,   1.0, I18N_NOOP("Icelandic krona"), "N/A", 1.0, -1 }, // 23
	{ 1936.27,    "L", "ITL", 100.0,1000.0, I18N_NOOP("Italian lira"), "N/A", 1.0, -1 }, // 24
	{     1.0,   "¥", "JPY", 100.0, 100.0, I18N_NOOP("Japanese yen"), "Yen/Japan", 1.0, -1 }, // 25
	{     1.0,    "W", "KRW", 100.0,1000.0, I18N_NOOP("South Korean won"), "Won/South Korea", 1.0, -1 }, // symbol should be "₩" - 26
	{     1.0, "SLRs", "LKR",   1.0,   1.0, I18N_NOOP("Sri Lankan rupee"), "Rupee/Sri Lanka", 1.0, -1 }, // 27
	{     1.0,   "lt", "LTL",   1.0,   1.0, I18N_NOOP("Latvian lats"), "N/A", 1.0, -1 }, // 28
	{ 40.3399, "FLux", "LUF",   1.0,  10.0, I18N_NOOP("Luxembourg franc"), "N/A", 1.0, -1 }, // 29
	{     1.0,   "Ls", "LVL",   1.0,   1.0, I18N_NOOP("Lithuanian litas"), "N/A", 1.0, -1 }, // 30
	{     1.0,   "Lm", "MTL",   1.0,   1.0, I18N_NOOP("Maltese lira"), "N/A", 1.0, -1 }, // 31
	{     1.0, "Mex$", "MXN",   1.0,   1.0, I18N_NOOP("Mexican peso"), "Peso/Mexico", 1.0, -1 }, // 32
	{     1.0,   "MR", "MYR",   1.0,   1.0, I18N_NOOP("Malaysian ringgit"), "Ringgit/Malaysia", 1.0, -1 }, // 33
	{ 2.20371,   "Fl", "NLG",   1.0,   1.0, I18N_NOOP("Dutch gulden"), "N/A", 1.0, -1 }, // 34
	{     1.0,  "NKr", "NOK",   1.0,   1.0, I18N_NOOP("Norvegian krone"), "Krone/Norway", 1.0, -1 }, // 35
	{     1.0,  "NZ$", "NZD",   1.0,   1.0, I18N_NOOP("New Zealand dollar"), "Dollar/New Zealand", 1.0, -1 }, // 36
	{     1.0, "zł", "PLN",   1.0,   1.0, I18N_NOOP("Polish sloty"), "N/A", 1.0, -1 }, // 37
	{ 2.00482,  "Esc", "PTE", 100.0, 100.0, I18N_NOOP("Portuguese escudo"), "N/A", 1.0, -1 }, // 38
	{     1.0,    "L", "ROL", 100.0,10000., I18N_NOOP("Romanian leu"), "N/A", 1.0, -1 }, // 39
	{     1.0,  "SKr", "SEK",   1.0,   1.0, I18N_NOOP("Swedish krona"), "Krona/Sweden", 1.0, -1 }, // 40
	{     1.0,   "S$", "SGB",   1.0,   1.0, I18N_NOOP("Singapore dollar"), "Dollar/Singapore", 1.0, -1 }, // 41
	{     1.0,  "SIT", "SIT",   1.0,   1.0, I18N_NOOP("Slovenian tolar"), "N/A", 1.0, -1 }, // 42
	{     1.0,   "Sk", "SKK",   1.0,   1.0, I18N_NOOP("Slovakian koruna"), "N/A", 1.0, -1 }, // 43
	{     1.0,   "Bt", "THB",   1.0,   1.0, I18N_NOOP("Thailandese baht"), "Baht/Thailand", 1.0, -1 }, // 44
	{     1.0,   "Bt", "TWD",   1.0,   1.0, I18N_NOOP("New Taiwanese dollar"), "N.T. Dollar/Taiwan", 1.0, -1 }, // 45
	{     1.0,    "$", "USD",   1.0,   1.0, I18N_NOOP("US dollar"), "N/A", 1.0, -1 }, // 46
	{     1.0,    "$", "VEB", 100.0,1000.0, I18N_NOOP("Venezuelian bolivar"), "Bolivar/Venezuela", 1.0, -1 }, // 47
	{     1.0,    "R", "ZAR",   1.0,   1.0, I18N_NOOP("South African rand"), "Rand/South Africa", 1.0, -1 } // 48
};

