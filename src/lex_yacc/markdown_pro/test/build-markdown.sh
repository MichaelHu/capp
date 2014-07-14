#!/bin/bash

echo "parsing $1"

../markdown.out "$1" \
    | cat ./tpl/header.tpl.html - ./tpl/footer.tpl.html \
    > "$1".md.html

