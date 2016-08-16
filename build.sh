#!/bin/bash
cd build
qmake ../karabiner-gui.pro -r -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
sqt "make in"leep 2s
make
