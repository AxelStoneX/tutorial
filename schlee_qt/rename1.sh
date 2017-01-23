#!/bin/bash

CURRENT_DIR=`pwd`

echo "Current directory is $CURRENT_DIR"
for D in `find $CURRENT_DIR -name '0[*'`
do
   #echo $D
    cd $D
    STR_D=$(basename $D)
    TMP1=`find . -name '[0-9][0-9]*.pro'`
    TMP2=`echo $TMP1 | grep -oP "[0-9][0-9]."`
    TMP3=`echo ${TMP2//_}`
    TMP4=`sed "s/^............/$TMP3/" <<< $STR_D`
    echo $TMP4
    cd ..
    mv $D $TMP4
done


