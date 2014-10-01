#!/bin/bash

TreePYTHONReady="TreePYTHONReady.txt"
TreePYTHONGraph="TreePYTHON.png"
TreeCPPReady="TreeCPPReady.txt"
TreeCPPGraph="TreeCPP.png"

date
python illustrate-tree.py $TreePYTHONReady $TreePYTHONGraph
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: illustrate-tree.py failed"
    date
    exit 1
fi

date
python illustrate-tree.py $TreeCPPReady $TreeCPPGraph
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: illustrate-tree.py failed"
    date
    exit 1
fi
