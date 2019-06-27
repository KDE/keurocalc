/***************************************************************************
                          main.cpp  -  main program
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

#include <stdio.h>

#include <QTextCodec>
#include <QCoreApplication>

#include <KAboutData>
#include <KLocalizedString>

#include "converter.h"

static const char
	*name =	I18N_NOOP("CurConvD"),
	*description = I18N_NOOP("A currency converter daemon"),
	*copyright = I18N_NOOP("(c) 2006-2019, the curconvd developers"),
	*text = I18N_NOOP("A program by Éric Bischoff <ebischoff@nerim.net>\n\nTime Genie exchange rates courtesy of <a href='http://www.timegenie.com/' title='foreign exchange rates courtesy of Time Genie'>Time Genie</a>\n");

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	KAboutData aboutData("curconvd", i18n(name), "1.3.0", i18n(description),
			     KAboutLicense::GPL, i18n(copyright), i18n(text));

	aboutData.addAuthor(i18n("Éric Bischoff"), i18n("Design and implementation"), "ebischoff@nerim.net");

	KAboutData::setApplicationData(aboutData);

	QTextCodec::setCodecForLocale( QTextCodec::codecForName("utf-8") );

	printf( "%s - %s\n%s\n\n", (const char *) i18n(name).toUtf8(),
	                           (const char *) i18n(description).toUtf8(),
	                           (const char *) i18n(copyright).toUtf8() );
	printf( "%s\n", (const char *) i18n("Time Genie exchange rates courtesy of Time Genie").toUtf8() );

	new CurrencyConverter();

	return a.exec();
}
