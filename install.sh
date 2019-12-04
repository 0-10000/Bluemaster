#!/bin/bash

build='build'
source_dir=$PWD
install_dir=$source_dir'/out'
bluez_dir=$1

sudo apt install cmake

mkdir -p $build
pushd $build
cmake -D INSTALL_DIR=$install_dir -D BLUEZ_DIR=$bluez_dir $source_dir
make
make install
popd
