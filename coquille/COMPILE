#!/bin/bash

lrelease src/src.pro
qmake-qt4
make
cd src
  doxygen doxygen.conf
cd ../
cd doc
  cd latex
    make
    rm -f *.ps *.dvi *.aux *.toc *.idx *.ind *.ilg *.log *.out
  cd ../
cd ../
