#!/bin/bash

rm -f adt

g++ *.h *.cpp -o adt

compiled_status=$?

if [ $compiled_status -eq 0 ]
then
    echo "Compilation success!"
    command="./adt input.txt Tree.txt"
    echo $command
    $command
    command_status=$?
    echo "Command status: $command_status"
else
    echo "failed compilation"
fi
