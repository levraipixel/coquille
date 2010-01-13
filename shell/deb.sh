#!/bin/bash

dpkg-buildpackage -rfakeroot
rm -f build-stamp configure-stamp
cd debian
  rm -rf coquille
  rm -f coquille.debhelper.log coquille.substvars files
cd ../
