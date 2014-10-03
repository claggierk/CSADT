#!/bin/bash

./clean.sh

echo " ***** adt *********************************************"

numTreeNodes="5"

g++ -rdynamic *.h *.cpp -o adt
compiled_status=$?
if [[ $compiled_status -eq 0 ]]
then
    echo "Compilation success!"
    command="./adt ADT_ready.txt Tree.txt $numTreeNodes"
    echo $command
    $command
    command_status=$?
    if [[ $command_status -eq 0 ]]
    then
        echo "successful $command"
    else
        echo "failed $command"
        return 1
    fi
else
    echo "failed compilation"
    return 2
fi

echo " *******************************************************"
echo ""
