#!/bin/bash

find . -mindepth 1 -maxdepth 1 -type d -exec echo "echo 'buiding {} ...'; pushd {} && make && popd; echo" \; \
    > ___tmp.lst

bash < ___tmp.lst

rm ___tmp.lst
