#!/bin/bash

rm -f Training.txt
rm -f ComparisonRecords.txt
rm -f Tree.txt
rm -f CS_ADT_Tree.png

numNodes="2"

inputFile="../10.csv"
#inputFile="../20.csv"
#inputFile="../30.csv"
#inputFile="../40.csv"
#inputFile="../50.csv"
#inputFile="../60.csv"
#inputFile="../70.csv"
#inputFile="../80.csv"
#inputFile="../90.csv"
#inputFile="../100.csv"
#inputFile="../200.csv"
#inputFile="../300.csv"
#inputFile="../400.csv"
#inputFile="../500.csv"
#inputFile="../600.csv"
#inputFile="../644.csv"

start1=$(date +%s.%N)
python raw-data-to-text/1_CSVToText.py $inputFile Training.txt
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: raw-data-to-text/1_CSVToText.py failed"
    exit 1
fi
end1=$(date +%s.%N)
runtime1=$(python -c "print(${end1} - ${start1})")

start2=$(date +%s.%N)
python compare-records/compare-pairs.py Training.txt ComparisonRecords.txt
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: compare-records/compare-pairs.py failed"
    exit 1
fi
end2=$(date +%s.%N)
runtime2=$(python -c "print(${end2} - ${start2})")

start3=$(date +%s.%N)
python create-tree/create-tree.py ComparisonRecords.txt conditions.txt $numNodes Tree.txt
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: create-tree/create-tree.py failed"
    exit 1
fi
end3=$(date +%s.%N)
runtime3=$(python -c "print(${end3} - ${start3})")

start4=$(date +%s.%N)
python illustrate-tree/illustrate-tree.py Tree.txt CS_ADT_Tree.png
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: illustrate-tree/illustrate-tree.py failed"
    exit 1
fi
end4=$(date +%s.%N)
runtime4=$(python -c "print(${end4} - ${start4})")

start5=$(date +%s.%N)
python classification-accuracy/classification-accuracy.py ComparisonRecords.txt Tree.txt
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: classification-accuracy/classification-accuracy.py failed"
    exit 1
fi
end5=$(date +%s.%N)
runtime5=$(python -c "print(${end5} - ${start5})")

#start6=$(date +%s.%N)
#python classify-testing/classify-testing.py $inputFile Tree.txt
#status=$?
#if (( status != 0 ))
#then
#    echo " ##### ERROR: classify-testing/classify-testing.py failed"
#    exit 1
#fi
#end6=$(date +%s.%N)
#runtime6=$(python -c "print(${end6} - ${start6})")

printf "\n"
printf "\nSteps 1 - 6"
printf "\n%.2f" $runtime1
printf "\n%.2f" $runtime2
printf "\n%.2f" $runtime3
printf "\n%.2f" $runtime4
printf "\n%.2f" $runtime5
#printf "\n%.2f" $runtime6
printf "\n"
