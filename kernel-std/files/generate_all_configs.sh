#!/bin/sh

# Adjusts the configuration options to build the variants correctly
#
# arg1: are we only generating debug configs


DEBUGBUILDSENABLED=$1

if [ -z $1 ]; then
    exit 1
fi

if [ $DEBUGBUILDSENABLED -eq 0 ]; then
    for i in kernel-*debug*.config; do
        base=`echo $i | sed -r s/-?debug//g`
        NEW=kernel-`echo $base | cut -d - -f2-`
        mv $i $NEW
    done
fi
