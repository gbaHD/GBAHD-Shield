#!/bin/zsh

BLUEPAD32=$(pwd)/external/Bluepad32

# Install Bluepad32 to components
[ ! -d ${BLUEPAD32}/src/components/bluepad32 ] && cp -r ${BLUEPAD32}/src/components/bluepad32 ./components
[ ! -d ${BLUEPAD32}/src/components/bluepad32_arduino ] && cp -r ${BLUEPAD32}/src/components/bluepad32_arduino ./components

# Setup BTStack
IDF_PATH=$(pwd)
python3 ${BLUEPAD32}/external/btstack/port/esp32/integrate_btstack.py
