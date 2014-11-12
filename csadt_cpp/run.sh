#!/bin/bash

run="./run.sh"

task="generate-unique-pairs"
cd $task
$run
command_status=$?
echo "command_status: $command_status"
if [[ $command_status -eq 0 ]]
then
    echo "successful $task"
else
    echo "failed $task"
    exit 1
fi

task="generate-input"
cd ../$task
$run
command_status=$?
if [[ $command_status -eq 0 ]]
then
    echo "successful $task"
else
    echo "failed $task"
    exit 2
fi

task="adt"
cd ../$task
$run
command_status=$?
if [[ $command_status -eq 0 ]]
then
    echo "successful $task"
else
    echo "failed $task"
    exit 2
fi

cd ../
python ../csadt_python/translate-tree/translate-tree.py adt/Tree.txt TreeCPPReady.txt
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: translate-tree/translate-tree.py failed"
    date
    exit 1
fi

python ../csadt_python/classification-accuracy/classification-accuracy.py ../csadt_python/ComparisonRecords.txt TreeCPPReady.txt
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
