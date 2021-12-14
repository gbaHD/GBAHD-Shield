#!/bin/zsh

BLUEPAD32=$(pwd)/external/bluepad32

# Install Bluepad32 to components
[ ! -d components/bluepad32 ] && cp -r ${BLUEPAD32}/src/components/bluepad32 ./components
[ ! -d components/bluepad32_arduino ] && cp -r ${BLUEPAD32}/src/components/bluepad32_arduino ./components

# Setup BTStack
IDF_PATH=$(pwd)
python3 ${BLUEPAD32}/external/btstack/port/esp32/integrate_btstack.py
