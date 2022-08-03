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
#include <QCoreApplication>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QTextCodec>
#endif

#include <KAboutData>
#include <KLocalizedString>

#include "converter.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
    KAboutData aboutData("curconvd", i18n("CurConvD"), "1.3.1", i18n("A currency converter daemon"),
                 KAboutLicense::GPL, i18n("(c) 2006-2019, the curconvd developers"), i18n("A program by Éric Bischoff <ebischoff@nerim.net>\n\nTime Genie exchange rates courtesy of <a href='http://www.timegenie.com/' title='foreign exchange rates courtesy of Time Genie'>Time Genie</a>\n"));

	aboutData.addAuthor(i18n("Éric Bischoff"), i18n("Design and implementation"), "ebischoff@nerim.net");

	KAboutData::setApplicationData(aboutData);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QTextCodec::setCodecForLocale( QTextCodec::codecForName("utf-8") );
#endif
	new CurrencyConverter();

	return a.exec();
}
