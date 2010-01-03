#!/bin/bash

make clean

rm -r qtc-gdbmacros
rm coquille
rm coquille.pro.user
rm Makefile*
rm *~

cd doc
    rm -r *
cd ../

cd lib
  rm -r .moc
  rm libqcodeedit.a
  rm libqtermwidget.a
  rm libqticonloader.a
  rm Makefile

  cd qcodeedit
    rm -r debug
    rm -r release
    rm Makefile*
  cd ../
  
  cd qtermwidget
    rm -r debug
    rm -r release
    rm Makefile*
  cd ../

  cd qticonloader
    rm -r debug
    rm -r release
    rm Makefile*
  cd ../
cd ../

cd src
  rm -r debug
  rm -r release
  rm Makefile*
  
  cd resources
    cd languages
      rm -r *.qm
    cd ../
  cd ../
cd ../
