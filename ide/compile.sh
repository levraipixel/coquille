#!/bin/bash

lrelease src/src.pro
qmake-qt4
make
cd src
doxygen doxygen.conf
