/***************************************************************************
                          main.cpp  -  main program
                             -------------------
    begin                : lun nov 13 11:22:05 CET 2006
    copyright            : (C) 2006-2008 by Éric Bischoff
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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kapplication.h>

#include <QTextCodec>

#include "converter.h"

static const char
	*name =	I18N_NOOP("CurConvD"),
	*description = I18N_NOOP("A currency converter daemon"),
	*copyright = I18N_NOOP("(c) 2006-2008, the curconvd developpers"),
	*text = I18N_NOOP("A program by Éric Bischoff <ebischoff@nerim.net>\n");

int main(int argc, char *argv[])
{
	KAboutData aboutData("curconvd", "curconvd", ki18n(name), "1.0.0", ki18n(description),
			     KAboutData::License_GPL, ki18n(copyright), ki18n(text));
	KCmdLineOptions options;

        aboutData.addAuthor(ki18n("Éric Bischoff"), ki18n("Design and implementation"), "ebischoff@nerim.net");
	KCmdLineArgs::init(argc, argv, &aboutData);
	KCmdLineArgs::addCmdLineOptions(options);

	QTextCodec::setCodecForLocale( QTextCodec::codecForName("utf-8") );

	KApplication a(false);
	CurrencyConverter *currency;

	printf( "Currency conversion daemon (c) 2006-2008, the curconvd developpers\n" );
	currency = new CurrencyConverter();

	return a.exec();
}
