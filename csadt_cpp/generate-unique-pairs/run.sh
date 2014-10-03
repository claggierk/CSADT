#!/bin/bash

./clean.sh

echo " ***** generate-unique-combinations ********************"

g++ GenerateUniquePairs.cpp -unique-pairs.cpp -o generate-unique-pairs
compiled_status=$?
if [ $compiled_status -eq 0 ]
then
    echo "Compilation success!"
    command="./generate-unique-pairs $(cat ../../csadt_python/Training.txt | wc -l)"
    echo $command
    $command
    command_status=$?
    echo "Command status: $command_status"
    if [ $command_status -eq 0 ]
    then
        cp UniqueCombinations.txt ../generate-input/
        echo "Copied UniqueCombinations.txt into ../generate-input/"
	fi
else
    echo "failed compilation"
fi

echo " *******************************************************"
echo ""
