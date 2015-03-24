#!/bin/bash

records=$1

if [ -f  "Training-$records.txt" ]
then
  unlink Training.txt
  ln -s Training-$records.txt Training.txt
else
  echo "Training-$records.txt does not exist"
  exit 1
fi

if [ -f "ComparisonRecords-$records.txt" ]
then
  unlink ComparisonRecords.txt
  ln -s ComparisonRecords-$records.txt ComparisonRecords.txt
else
  echo "ComparisonRecords-$records.txt does not exist"
  exit 2
fi
