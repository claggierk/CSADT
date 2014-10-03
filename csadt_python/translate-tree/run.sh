#!/bin/bash

rm Tree*.txt

TreePYTHON="TreePYTHON.txt"
TreePYTHONReady="TreePYTHONReady.txt"
TreeCPP="TreeCPP.txt"
TreeCPPReady="TreeCPPReady.txt"

cp ../Tree.txt $TreePYTHON
cp ../../csadt_cpp/adt/Tree.txt $TreeCPP

date
python translate-tree.py $TreePYTHON $TreePYTHONReady
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: translate-tree.py failed"
    date
    exit 1
fi

date
python translate-tree.py $TreeCPP $TreeCPPReady
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: translate-tree.py failed"
    date
    exit 1
fi

diff TreePYTHONReady.txt TreeCPPReady.txt
