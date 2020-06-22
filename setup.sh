#!/bin/bash

sudo apt update

sudo apt install -y gcc g++
sudo apt install -y cmake
sudo apt install -y libeigen3-dev
sudo apt install -y libomp-dev
sudo apt install -y libncursesada5-dev
sudo apt install -y libboost-all-dev
sudo apt install -y libomp-dev
sudo apt install -y liblapack-dev libblas-dev libarmadillo-dev
sudo apt install -y libtbb-dev

sudo apt install -y libgtest-dev
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp *.a /usr/lib
echo "INSTALLATION COMPLETE"

