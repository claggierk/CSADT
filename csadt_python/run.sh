#!/bin/bash

rm -f Training.txt
rm -f ComparisonRecords.txt
rm -f Tree.txt
rm -f CS_ADT_Tree.png

inputFile="../30.csv"

date
python 1_CSVToText.py $inputFile Training.txt
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: 1_CSVToText.py failed"
    date
    exit 1
fi

python 3_TextToComparisonPairs.py Training.txt ComparisonRecords.txt
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: 3_TextToComparisonPairs.py failed"
    date
    exit 1
fi

python 4_ComparisonPairsToTree.py ComparisonRecords.txt conditions.txt Tree.txt
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: 4_ComparisonPairsToTree.py failed"
    date
    exit 1
fi

python IllustrateTree.py
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: IllustrateTree.py failed"
    date
    exit 1
fi

python 5_TestingTextThroughTree.py $inputFile Tree.txt
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: 5_TestingTextThroughTree.py failed"
    date
    exit 1
fi
date
