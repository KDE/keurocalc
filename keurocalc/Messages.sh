#! /bin/bash

grep '[^-]name =' ../currencies/currencies.xml | sed 's/name = */I18N_NOOP(/; s/ *$$/);/' > names.cpp

$EXTRACTRC *.ui >> rc.cpp
$XGETTEXT *.cpp -o $podir/keurocalc.pot

rm names.cpp
