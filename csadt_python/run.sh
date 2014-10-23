#!/bin/bash

rm -f Training.txt
rm -f ComparisonRecords.txt
rm -f Tree.txt
rm -f CS_ADT_Tree.png

#inputFile="../10.csv"
inputFile="../30.csv"
#inputFile="../VA_UniqueGoodExplod_2009-2010-2012_Training.csv"
numNodes="2"

date
python raw-data-to-text/1_CSVToText.py $inputFile Training.txt
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: raw-data-to-text/1_CSVToText.py failed"
    date
    exit 1
fi

python compare-records/compare-pairs.py Training.txt ComparisonRecords.txt
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: compare-records/compare-pairs.py failed"
    date
    exit 1
fi

python create-tree/create-tree.py ComparisonRecords.txt conditions.txt $numNodes Tree.txt
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: create-tree/create-tree.py failed"
    date
    exit 1
fi

python illustrate-tree/illustrate-tree.py Tree.txt CS_ADT_Tree.png
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: illustrate-tree/illustrate-tree.py failed"
    date
    exit 1
fi

python classification-accuracy/classification-accuracy.py ComparisonRecords.txt Tree.txt
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: classification-accuracy/classification-accuracy.py failed"
    date
    exit 1
fi

#python classify-testing/classify-testing.py $inputFile Tree.txt
#status=$?
#if (( status != 0 ))
#then
#    echo " ##### ERROR: classify-testing/classify-testing.py failed"
#    date
#    exit 1
#fi
#date
