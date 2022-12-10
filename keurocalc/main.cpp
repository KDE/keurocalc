/***************************************************************************
                          main.cpp  -  main program
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

#include <unistd.h>

#include <QTextCodec>
#include <QStandardPaths>
#include <QSplashScreen>

#include <KAboutData>
#include <KLocalizedString>

#include "keurocalc.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	KLocalizedString::setApplicationDomain("keurocalc");

    const QString copyright = i18n("(c) 2001-2022, the KEuroCalc developers");
    const QString description = i18n("Cash converter and calculator");
    KAboutData aboutData("keurocalc", i18n("KEuroCalc"), "1.3.2", description,
                 KAboutLicense::GPL, copyright, i18n("A program by Éric Bischoff <bischoff@kde.org>\n\nThis program is dedicated to all who know that money does not justify anything.\n\nTime Genie exchange rates courtesy of <a href='http://www.timegenie.com/' title='foreign exchange rates courtesy of Time Genie'>Time Genie</a>\n"));

	aboutData.addAuthor(i18n("Éric Bischoff"), i18n("Design and implementation"), "bischoff@kde.org");
	aboutData.addAuthor(i18n("Gil Gross"), i18n("Additional functionality"), "ptit.ours@gmail.com");
	aboutData.addCredit(i18n("Melchior Franz"), i18n("Design and testing"), "a8603365@unet.univie.ac.at");
	aboutData.addCredit(i18n("Bas Willems"), i18n("Graphical artwork"), "cybersurfer@euronet.nl");
	aboutData.setOrganizationDomain("kde.org");
	aboutData.setTranslator(i18nc("NAME OF TRANSLATORS", "Your names"),
                                i18nc("EMAIL OF TRANSLATORS", "Your emails"));

	KAboutData::setApplicationData(aboutData);

	a.setWindowIcon(QIcon::fromTheme("keurocalc", a.windowIcon()));

	QTextCodec::setCodecForLocale( QTextCodec::codecForName("utf-8") );

	QSplashScreen *splash;
	KEuroCalc *keurocalc;
	bool splashScreen;

	splash = new QSplashScreen( QPixmap( QStandardPaths::locate(QStandardPaths::AppDataLocation, "splash.png" ) ) );
    splash->showMessage(description + '\n' + copyright, Qt::AlignBottom);

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
