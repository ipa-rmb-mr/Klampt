#!/bin/bash
git pull origin master
cd Cpp/Dependencies/KrisLibrary
git pull origin master
make
cd ../../..
rm -rf Python/build
make
echo "If you wish to install the updated Klamp't, type sudo make install"
echo "If you wish to install the updated Klamp't Python API, type"
echo "  sudo make python-install"
