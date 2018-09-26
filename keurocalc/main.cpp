/***************************************************************************
                          main.cpp  -  main program
                             -------------------
    begin                : sam déc  1 23:40:19 CET 2001
    copyright            : (C) 2001-2018 by Éric Bischoff
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

#include <unistd.h>

#include <QtCore/QTextCodec>
#include <QtCore/QStandardPaths>
#include <QtWidgets/QSplashScreen>

#include <KCoreAddons/KAboutData>
#include <KI18n/KLocalizedString>

#include "keurocalc.h"

static const char
	*name = I18N_NOOP("KEuroCalc"),
	*description = I18N_NOOP("Cash converter and calculator"),
	*copyright = I18N_NOOP("(c) 2001-2018, the KEuroCalc developers"),
	*text = I18N_NOOP("A program by Éric Bischoff <ebischoff@nerim.net>\n\nThis program is dedicated to all who know that money does not justify anything.\n\nTime Genie exchange rates courtesy of <a href='http://www.timegenie.com/' title='foreign exchange rates courtesy of Time Genie'>Time Genie</a>\n");

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	KLocalizedString::setApplicationDomain("keurocalc");

	KAboutData aboutData("keurocalc", i18n(name), "1.3.0", i18n(description),
			     KAboutLicense::GPL, i18n(copyright), i18n(text));

	aboutData.addAuthor(i18n("Éric Bischoff"), i18n("Design and implementation"), "ebischoff@nerim.net");
	aboutData.addAuthor(i18n("Gil Gross"), i18n("Additional functionality"), "ptit.ours@gmail.com");
	aboutData.addCredit(i18n("Melchior Franz"), i18n("Design and testing"), "a8603365@unet.univie.ac.at");
	aboutData.addCredit(i18n("Bas Willems"), i18n("Graphical artwork"), "cybersurfer@euronet.nl");
	aboutData.setOrganizationDomain("kde.org");

	KAboutData::setApplicationData(aboutData);

	QTextCodec::setCodecForLocale( QTextCodec::codecForName("utf-8") );

	QSplashScreen *splash;
	KEuroCalc *keurocalc;
	bool splashScreen;

	splash = new QSplashScreen( QPixmap( QStandardPaths::locate(QStandardPaths::AppDataLocation, "splash.png" ) ) );
	splash->showMessage(i18n(description) + "\n" + i18n(copyright), Qt::AlignBottom);

	keurocalc = new KEuroCalc();

	splashScreen = keurocalc->readSplashScreen();
	if ( splashScreen )
	{
		splash->show();
		splash->repaint();
		for (int i = 0; i < 8; i++)
		{	// Something less idiotic to propose?
			a.processEvents();
			usleep(100000);
		}
		splash->finish(keurocalc);
	}
	delete splash;

	keurocalc->show();
	keurocalc->repaint();
	return a.exec();
}
