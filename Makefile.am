####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = keurocalc
keurocalc_SOURCES = calculator.ui keurocalc.cpp main.cpp 
keurocalc_LDADD   =  $(LIB_QT) $(LIB_KDECORE) $(LIB_KDEUI) $(LIB_KIO) $(LIBSOCKET)

EXTRA_DIST = keurocalc.h keurocalc.desktop lo32-app-keurocalc.png lo16-app-keurocalc.png

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Applications/
	$(INSTALL_DATA) $(srcdir)/keurocalc.desktop $(kde_appsdir)/Applications/keurocalc.desktop
	$(mkinstalldirs) $(kde_icondir)/locolor/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/lo32-app-keurocalc.png $(kde_icondir)/locolor/32x32/apps/keurocalc.png
	$(mkinstalldirs) $(kde_icondir)/locolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/lo16-app-keurocalc.png $(kde_icondir)/locolor/16x16/apps/keurocalc.png

uninstall-local:
	-rm -f $(kde_appsdir)/Applications/keurocalc.desktop
	-rm -f $(kde_icondir)/locolor/32x32/apps/keurocalc.png
	-rm -f $(kde_icondir)/locolor/16x16/apps/keurocalc.png

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

# Uncomment the following two lines if you add a ui.rc file for your application to make use of
# KDE
#rcdir = $(kde_datadir)/keurocalc
#rc_DATA = keurocalcui.rc

messages: calculator.cpp
	$(XGETTEXT) --from-code=utf-8 -ki18n -ktr2i18n -kI18N_NOOP calculator.cpp $(keurocalc_SOURCES) -o ../po/keurocalc.pot

