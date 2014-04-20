#!/bin/bash

./clean.sh

echo " ***** adt *********************************************"

g++ -rdynamic *.h *.cpp -o adt
compiled_status=$?
if [ $compiled_status -eq 0 ]
then
    echo "Compilation success!"
    command="./adt ADT_ready.txt Tree.txt"
    echo $command
    $command
    command_status=$?
    echo "Command status: $command_status"
else
    echo "failed compilation"
fi

echo " *******************************************************"
echo ""
