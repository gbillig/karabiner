#!/bin/bash
cd build
qmake ../karabiner.pro -r -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
sleep 2s
make
