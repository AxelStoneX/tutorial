#!/bin/bash

CURRENT_DIR=`pwd`

for D in `find $CURRENT_DIR -name '[0-9][0-9]_*'`
do
    if test -d $D
    then
        echo $D
        TMP1=`basename $D`
        NEWFILENAME=`echo 0${TMP1}`
        mv $D $NEWFILENAME
    fi
done
