#! /bin/bash

grep '[^-]name =' ../currencies/currencies.xml | sed 's/name = */I18N_NOOP(/; s/ *$$/);/' > names.cpp

uic-qt4 calculator.ui > calculator.cpp
uic-qt4 settingsdialog.ui > settingsdialog.cpp

xgettext --from-code=utf-8 -ki18n -kki18n -kI18N_NOOP -ktranslate:2 *.cpp -o ../po/keurocalc.pot

rm names.cpp calculator.cpp settingsdialog.cpp
