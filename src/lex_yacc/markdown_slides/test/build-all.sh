#!/bin/bash

THEME=default

if [ "" != $1 ] ; then
    THEME=$1
fi

find . -type f \
    -regex "\..*\.md" \
    -exec sh build-markdown.sh {} $THEME \;

find . -type f -regex "\..*\.md\.html" \
    | sed -E 's/^.+$/<a href="&" target="_blank">&<\/a><br>/g' \
    | cat ./tpl/header.tpl.html - ./tpl/footer.tpl.html  \
    > test_index.txt.md.html
