#!/bin/bash

rm -f prepare-input

g++ *.h *.cpp -o prepare-input -I /usr/local/include -L /usr/local/lib -l boost_thread -l boost_system

compiled_status=$?

if [ $compiled_status -eq 0 ]
then
    echo "Compilation success!"
    command="./prepare-input UniqueCombinations.txt 100LinesPreProcessed.txt ADT_ready.txt"
    echo $command
    $command
    command_status=$?
    echo "Command status: $command_status"
else
    echo "failed compilation"
fi
