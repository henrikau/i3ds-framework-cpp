#! /bin/sh

if [ -z "$1" ]
then
    echo "Usage: $0 [directory to recusively format C++ files]"
    exit 1
fi

exec astyle --style=gnu -s2 -O --recursive  $1/*.cpp $1/*.hpp
