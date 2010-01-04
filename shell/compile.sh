#!/bin/bash

cd coquille
  lrelease src/src.pro
  qmake-qt4
  make
cd ../
