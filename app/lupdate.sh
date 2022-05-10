#!/bin/bash
folder="./translations"
if [ ! -d  $folder ]; then
    mkdir $folder
fi

lupdate ./ -ts -no-obsolete ./$folder/dde-widgets_zh_CN.ts
#tx push -s -b m20
