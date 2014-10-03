#!/bin/bash

run="./run.sh"

task="generate-unique-pairs"
cd $task
$run
command_status=$?
echo "command_status: $command_status"
if [[ $compiled_status -eq 0 ]]
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
if [[ $compiled_status -eq 0 ]]
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
if [[ $compiled_status -eq 0 ]]
then
    echo "successful $task"
else
    echo "failed $task"
    exit 2
fi
