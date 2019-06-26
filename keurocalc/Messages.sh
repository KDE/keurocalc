#! /bin/bash

xsltproc --stringparam filename ../currencies/currencies.xml currencies2rc.xslt ../currencies/currencies.xml >> rc.cpp

$EXTRACTRC *.ui >> rc.cpp
$XGETTEXT *.cpp -o $podir/keurocalc.pot
