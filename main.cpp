/***************************************************************************
                          main.cpp  -  main program
                             -------------------
    begin                : sam déc  1 23:40:19 CET 2001
    copyright            : (C) 2001 by Éric Bischoff
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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>

#include "keurocalc.h"

static const char *description =
	I18N_NOOP("Cash converter and calculator for the Euro"),
	*text = I18N_NOOP("A program by Éric Bischoff <e.bischoff@noos.fr>\n\nThis program is dedicated to Charles the Great,\nVictor Hugo, Homer, Giuseppe Verdi and all other\ngreat Europeans who didn't make it to have their\nfaces on the Euro banknotes.\n");

// Command line arguments
static KCmdLineOptions options[] =
{
	{ 0, 0, 0 }
};

int main(int argc, char *argv[])
{
	KAboutData aboutData( "keurocalc", I18N_NOOP("KEuroCalc"),
	"0.04", description, KAboutData::License_GPL,
	"(c) 2001, the KEuroCalc developers", text);
	aboutData.addAuthor("Éric Bischoff", I18N_NOOP("Design and implementation"), "e.bischoff@noos.fr");
	aboutData.addCredit("Melchior Franz", I18N_NOOP("Design and testing"), "a8603365@unet.univie.ac.at");
	KCmdLineArgs::init( argc, argv, &aboutData );
	KCmdLineArgs::addCmdLineOptions( options );

	KApplication a;
	KEuroCalc *keurocalc = new KEuroCalc();
	a.setMainWidget(keurocalc);
	keurocalc->show();  

	return a.exec();
}
