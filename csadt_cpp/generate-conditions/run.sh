#!/bin/bash

./clean.sh

echo " ***** generate-conditions *****************************"

g++ *.cpp -o generate-conditions
compiled_status=$?
if [ $compiled_status -eq 0 ]
then
    echo "Compilation success!"
    command="./generate-conditions ADT_ready.txt Conditions.txt"
    echo $command
    $command
    command_status=$?
    echo "Command status: $command_status"
else
    echo "failed compilation"
fi

echo " *******************************************************"
echo ""
