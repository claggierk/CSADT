#!/bin/bash

cd ~/Downloads/
wget http://sourceforge.net/projects/boost/files/boost/1.54.0/boost_1_54_0.tar.gz/download
tar -zxf boost_1_54_0.tar.gz
sudo mv boost_1_54_0 /usr/local/
cd /usr/local/boost_1_54_0
sudo ./bootstrap.sh
sudo apt-get install libboost-all-dev
sudo apt-get install libbz2-dev
sudo ./b2
sudo bjam --build-type=complete --with-thread --layout=versioned
sudo ldconfig /usr/local/lib
