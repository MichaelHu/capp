#!/bin/bash

OUTPUTFILE=myenc_mac

gcc -c *.cpp
gcc -o $OUTPUTFILE \
    -L/usr/lib \
    -lc++ \
    *.o

if [[ $? == 1 ]]; then
    echo "build failed"
    exit 1
fi

rm *.o
echo test...
chmod +x $OUTPUTFILE
./$OUTPUTFILE "test123"

if [[ $? == 1 ]]; then
    echo "test failed"
    exit 1
fi

echo "build successfully"
