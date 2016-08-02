#!/bin/bash
qmake karabiner-gui.pro -r -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
make
sleep 5s
mv *.o ui* moc_* qrc* Makefile karabiner-gui ./build
