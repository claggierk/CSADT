#!/bin/bash

./clean.sh

echo " ***** generate-unique-combinations ********************"

g++ GenerateUniqueCombinations.cpp -o generate-unique-combinations
compiled_status=$?
if [ $compiled_status -eq 0 ]
then
    echo "Compilation success!"
    command="./generate-unique-combinations $(cat ../../csadt_python/Training.txt | wc -l)"
    echo $command
    $command
    command_status=$?
    echo "Command status: $command_status"
    if [ $command_status -eq 0 ]
    then
    	cp UniqueCombinations.txt ../GenerateInput/
    	echo "Copied UniqueCombinations.txt into ../GenerateInput/"
	fi
else
    echo "failed compilation"
fi

echo " *******************************************************"
echo ""