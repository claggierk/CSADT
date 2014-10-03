#!/bin/bash

./clean.sh

echo " ***** generate-conditions *****************************"

g++ *.cpp -o generate-conditions
compiled_status=$?
if [[ $compiled_status -eq 0 ]]
then
    echo "Compilation success!"
    command="./generate-conditions ADT_ready.txt Conditions.txt"
    echo $command
    $command
    command_status=$?
    if [[ $command_status -eq 0 ]]
    then
        cp Conditions.txt ../../csadt_python/conditions-real-training.txt
    else
        echo "failed $command"
        exit 1
    fi
else
    echo "failed compilation"
    exit 2
fi

echo " *******************************************************"
echo ""
