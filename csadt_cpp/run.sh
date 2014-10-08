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
python ../csadt_python/classify-testing/classify-testing.py ../VA_UniqueGoodExplod_2009-2010-2012_Training.csv TreeCPPReady.txt
#python ../csadt_python/classify-testing/classify-testing.py ../20.csv TreeCPPReady.txt
