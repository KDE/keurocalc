/***************************************************************************
                          main.cpp  -  main program
                             -------------------
    begin                : sam déc  1 23:40:19 CET 2001
    copyright            : (C) 2001-2012 by Éric Bischoff
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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <ksplashscreen.h>
#include <kstandarddirs.h>

#include <QTextCodec>

#include "keurocalc.h"

static const char
	*name = I18N_NOOP("KEuroCalc"),
	*description = I18N_NOOP("Cash converter and calculator"),
	*copyright = I18N_NOOP("(c) 2001-2012, the KEuroCalc developers"),
	*text = I18N_NOOP("A program by Éric Bischoff <ebischoff@nerim.net>\n\nThis program is dedicated to humorist Michael Moore, for he\nknows that money does not justify anything...\n\nTime Genie exchange rates courtesy of <a href='http://www.timegenie.com/' title='foreign exchange rates courtesy of Time Genie'>Time Genie</a>\n");

int main(int argc, char *argv[])
{
	KAboutData aboutData("keurocalc", "keurocalc", ki18n(name), "1.2.0", ki18n(description),
			     KAboutData::License_GPL, ki18n(copyright), ki18n(text));
	KCmdLineOptions options;

	aboutData.addAuthor(ki18n("Éric Bischoff"), ki18n("Design and implementation"), "ebischoff@nerim.net");
	aboutData.addAuthor(ki18n("Gil Gross"), ki18n("Additional functionality"), "ptit.ours@gmail.com");
	aboutData.addCredit(ki18n("Melchior Franz"), ki18n("Design and testing"), "a8603365@unet.univie.ac.at");
	aboutData.addCredit(ki18n("Bas Willems"), ki18n("Graphical artwork"), "cybersurfer@euronet.nl");
	aboutData.setOrganizationDomain("kde.org");
	KCmdLineArgs::init(argc, argv, &aboutData);
	KCmdLineArgs::addCmdLineOptions(options);

	QTextCodec::setCodecForLocale( QTextCodec::codecForName("utf-8") );

	KApplication a;
	KSplashScreen *splash;
	KEuroCalc *keurocalc;
	bool splashScreen;

	splash = new KSplashScreen( QPixmap( KStandardDirs::locate( "data", "keurocalc/splash.png" ) ) );
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

	a.setTopWidget(keurocalc);
	keurocalc->show();
	keurocalc->repaint();
	return a.exec();
}
