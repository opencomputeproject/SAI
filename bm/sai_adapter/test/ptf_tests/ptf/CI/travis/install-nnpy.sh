#!/bin/sh
set -e
git clone https://github.com/nanomsg/nnpy.git
cd nnpy
sudo pip install cffi
sudo pip install --upgrade cffi
sudo pip install .
cd ..
