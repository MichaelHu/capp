#!/bin/bash

find . -type f \
    -regex "\..*\.txt" \
    -exec sh build-markdown.sh {} \;

find . -type f -regex "\..*\.md\.html" \
    | sed -E 's/^.+$/<a href="&" target="_blank">&<\/a><br>/g' \
    | cat ./tpl/header.tpl.html - ./tpl/footer.tpl.html  \
    > test_index.txt.md.html
