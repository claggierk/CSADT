#!/bin/bash

./clean.sh

echo " ***** prepare-input ***********************************"

g++ *.h *.cpp -o prepare-input -I /usr/local/include -L /usr/local/lib -l boost_thread -l boost_system
compiled_status=$?
if [[ $compiled_status -eq 0 ]]
then
    echo "Compilation success!"
    command="./prepare-input UniqueCombinations.txt ../../csadt_python/Training.txt ADT_ready.txt"
    echo $command
    $command
    command_status=$?
    echo "Command status: $command_status"
    if [[ $command_status -eq 0 ]]
    then
        cp ADT_ready.txt ../adt/
        cp ADT_ready.txt ../generate-conditions/
        echo "Copied ADT_ready.txt into ../adt/"
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
