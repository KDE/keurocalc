####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = keurocalc
keurocalc_SOURCES = calculator.ui settingsdialog.ui currencies.cpp keurocalc.cpp preferences.cpp main.cpp 
keurocalc_LDADD   =  $(LIB_KIO)

KDE_ICON = keurocalc

mydatadir = $(kde_datadir)/keurocalc
mydata_DATA = splash.png currencies.xml

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Applications/
	$(INSTALL_DATA) $(srcdir)/keurocalc.desktop $(kde_appsdir)/Applications/keurocalc.desktop

uninstall-local:
	-rm -f $(kde_appsdir)/Applications/keurocalc.desktop

####### kdevelop will overwrite this part!!! (end)############

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)

METASOURCES = AUTO

# the library search path. 
keurocalc_LDFLAGS = $(all_libraries) $(KDE_RPATH)

names.cpp: currencies.xml
	grep '[^-]name =' currencies.xml | sed 's/name = */I18N_NOOP(/; s/ *$$/);/' > names.cpp

messages: rc.cpp names.cpp 
	$(XGETTEXT) *.cpp -o $(podir)/keurocalc.pot
