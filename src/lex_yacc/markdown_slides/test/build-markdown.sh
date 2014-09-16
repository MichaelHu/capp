#!/bin/bash

echo "parsing $1"

../markdown.out "$1" \
    | cat ./tpl/header_$2.tpl.html - ./tpl/footer_$2.tpl.html \
    > "$1".html

