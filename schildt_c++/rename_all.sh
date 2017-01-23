#!/bin/sh

CURRENT_DIR=`pwd`

for D in `find $CURRENT_DIR -name '[0-9][0-9]_*'`
do
    if test -d $D
        then
           OLD_DIR_NAME=`basename $D`
           NEW_DIR_NAME=0$OLD_DIR_NAME
           echo $NEW_DIR_NAME
           mv $OLD_DIR_NAME $NEW_DIR_NAME
    fi
done
