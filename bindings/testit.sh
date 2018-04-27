#! /bin/sh

make clean
make
cp _i3ds_python.so _i3ds.so
cp ../../bindings/test.py .
exec python test.py
