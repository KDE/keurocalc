####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = keurocalc
keurocalc_SOURCES = calculator.ui settingsdialog.ui currencies.cpp keurocalc.cpp preferences.cpp main.cpp 
keurocalc_LDADD   =  $(LIB_QT) $(LIB_KDECORE) $(LIB_KDEUI) $(LIB_KIO) $(LIBSOCKET)

EXTRA_DIST = keurocalc.desktop cr48-app-keurocalc.png cr32-app-keurocalc.png cr16-app-keurocalc.png splash.png

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Applications/
	$(INSTALL_DATA) $(srcdir)/keurocalc.desktop $(kde_appsdir)/Applications/keurocalc.desktop
	$(mkinstalldirs) $(kde_icondir)/crystalsvg/48x48/apps/
	$(INSTALL_DATA) $(srcdir)/cr48-app-keurocalc.png $(kde_icondir)/crystalsvg/48x48/apps/keurocalc.png
	$(mkinstalldirs) $(kde_icondir)/crystalsvg/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/cr32-app-keurocalc.png $(kde_icondir)/crystalsvg/32x32/apps/keurocalc.png
	$(mkinstalldirs) $(kde_icondir)/crystalsvg/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/cr16-app-keurocalc.png $(kde_icondir)/crystalsvg/16x16/apps/keurocalc.png
	$(mkinstalldirs) $(kde_datadir)/keurocalc/
	$(INSTALL_DATA) $(srcdir)/splash.png $(kde_datadir)/keurocalc/splash.png
	$(INSTALL_DATA) $(srcdir)/currencies.xml $(kde_datadir)/keurocalc/currencies.xml

uninstall-local:
	-rm -f $(kde_appsdir)/Applications/keurocalc.desktop
	-rm -f $(kde_icondir)/crystalsvg/48x48/apps/keurocalc.png
	-rm -f $(kde_icondir)/crystalsvg/32x32/apps/keurocalc.png
	-rm -f $(kde_icondir)/crystalsvg/16x16/apps/keurocalc.png
	-rm -rf $(kde_datadir)/keurocalc/

####### kdevelop will overwrite this part!!! (end)############
# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)

METASOURCES = AUTO

# the library search path. 
keurocalc_LDFLAGS = $(all_libraries) $(KDE_RPATH)

names.cpp: currencies.xml
	grep '[^-]name =' currencies.xml | sed 's/name = */I18N_NOOP(/; s/ *$$/);/' > names.cpp

messages: calculator.cpp settingsdialog.cpp names.cpp
	$(XGETTEXT) -ki18n -ktr2i18n -kI18N_NOOP *.cpp -o ../po/keurocalc.pot
