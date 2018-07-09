#!/bin/sh
swig -c\+\+ -python Seeed_vl53l0x.i
python setup.py build_ext --inplace



