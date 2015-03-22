#!/bin/bash

run="./run.sh"

# ---------------------------------------
start1=$(date +%s.%N)
task="generate-unique-pairs"
cd $task
$run
command_status=$?
echo "command_status: $command_status"
if [[ $command_status -eq 0 ]]
then
    echo "successful $task"
else
    echo "failed $task"
    exit 1
fi
end1=$(date +%s.%N)
runtime1=$(python -c "print(${end1} - ${start1})")

# ---------------------------------------
start2=$(date +%s.%N)
task="generate-input"
cd ../$task
$run
command_status=$?
if [[ $command_status -eq 0 ]]
then
    echo "successful $task"
else
    echo "failed $task"
    exit 2
fi
end2=$(date +%s.%N)
runtime2=$(python -c "print(${end2} - ${start2})")

# ---------------------------------------
start3=$(date +%s.%N)
task="adt"
cd ../$task
$run
command_status=$?
if [[ $command_status -eq 0 ]]
then
    echo "successful $task"
else
    echo "failed $task"
    exit 2
fi
end3=$(date +%s.%N)
runtime3=$(python -c "print(${end3} - ${start3})")

# ---------------------------------------
start4=$(date +%s.%N)
cd ../
python ../csadt_python/translate-tree/translate-tree.py adt/Tree.txt TreeCPPReady.txt
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: translate-tree/translate-tree.py failed"
    date
    exit 1
fi
end4=$(date +%s.%N)
runtime4=$(python -c "print(${end4} - ${start4})")

# ---------------------------------------
start5=$(date +%s.%N)
python ../csadt_python/illustrate-tree/illustrate-tree.py TreeCPPReady.txt CS_ADT_Tree.png
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: illustrate-tree/illustrate-tree.py failed"
    date
    exit 1
fi
end5=$(date +%s.%N)
runtime5=$(python -c "print(${end5} - ${start5})")

# ---------------------------------------
start6=$(date +%s.%N)
python ../csadt_python/classification-accuracy/classification-accuracy.py ../csadt_python/ComparisonRecords.txt TreeCPPReady.txt
status=$?
if (( status != 0 ))
then
    echo " ##### ERROR: classification-accuracy/classification-accuracy.py failed"
    date
    exit 1
fi
end6=$(date +%s.%N)
runtime6=$(python -c "print(${end6} - ${start6})")

# ---------------------------------------
#python classify-testing/classify-testing.py $inputFile Tree.txt
#status=$?
#if (( status != 0 ))
#then
#    echo " ##### ERROR: classify-testing/classify-testing.py failed"
#    date
#    exit 1
#fi
#date

printf "\n"
printf "\nSteps 1 - 6"
printf "\n%.2f" $runtime1
printf "\n%.2f" $runtime2
printf "\n%.2f" $runtime3
printf "\n%.2f" $runtime4
printf "\n%.2f" $runtime5
printf "\n%.2f" $runtime6
printf "\n"
