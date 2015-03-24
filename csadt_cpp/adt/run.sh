#!/bin/bash

./clean.sh

echo " ***** adt *********************************************"

#numTreeNodes="5"
#numTreeNodes="7"
#numTreeNodes="10"
#numTreeNodes="12"
#numTreeNodes="15"
numTreeNodes="2"

g++ -rdynamic *.h *.cpp -o adt -I /usr/local/include -L /usr/local/lib -l boost_thread -l boost_system
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
        exit 1
    fi
else
    echo "failed compilation"
    exit 2
fi

echo " *******************************************************"
echo ""
