/***************************************************************************
                          main.cpp  -  main program
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

#include <unistd.h>

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <ksplashscreen.h>
#include <kstandarddirs.h>

#include <qtextcodec.h>

#include "keurocalc.h"

static const char
	*name =	I18N_NOOP("KEuroCalc"),
	*description = I18N_NOOP("Cash converter and calculator"),
	*copyright = I18N_NOOP("(c) 2001-2004, the KEuroCalc developers"),
	*text = I18N_NOOP("A program by Ã‰ric Bischoff <e.bischoff@noos.fr>\n\nThis program is dedicated to humorist Michael Moore, for he\nknows that money does not justify anything...\n");

// Command line arguments
static KCmdLineOptions options[] =
{
	{ 0, 0, 0 }
};

int main(int argc, char *argv[])
{
	KAboutData aboutData( "keurocalc", name, "0.9.0", description,
	 	              KAboutData::License_GPL, copyright, text);
	aboutData.addAuthor("Ã‰ric Bischoff", I18N_NOOP("Design and implementation"), "e.bischoff@noos.fr");
	aboutData.addCredit("Melchior Franz", I18N_NOOP("Design and testing"), "a8603365@unet.univie.ac.at");
	aboutData.addCredit("Bas Willems", I18N_NOOP("Graphical artwork"), "cybersurfer@euronet.nl");
	KCmdLineArgs::init( argc, argv, &aboutData );
	KCmdLineArgs::addCmdLineOptions( options );

	QTextCodec::setCodecForLocale( QTextCodec::codecForName("utf-8") );

	KApplication a;
	KSplashScreen *splash;
	KEuroCalc *keurocalc;

	splash = new KSplashScreen( QPixmap( locate( "data", "keurocalc/splash.png" ) ) );

	splash->message(i18n(description) + "\n" + i18n(copyright), Qt::AlignBottom);
	splash->show();

	keurocalc = new KEuroCalc();
	a.setMainWidget(keurocalc);
	keurocalc->show();

	usleep(600000); // Something less idiotic to propose ?
	splash->finish(keurocalc);
	delete splash;

	keurocalc->repaint();
	return a.exec();
}
