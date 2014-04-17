#!/bin/bash

date
python 1_CSVToText.py ../VA_UniqueGoodExplod_2009-2010-2012_Training.csv VA_UniqueGoodExplod_2009-2010-2012_Training.txt
#python 1_CSVToText.py ../VA_UniqueGoodExplod_2009-2010-2012_TrainingSMALL.csv VA_UniqueGoodExplod_2009-2010-2012_Training.txt
python 3_TextToComparisonPairs.py VA_UniqueGoodExplod_2009-2010-2012_Training.txt ComparisonRecords.txt
python 4_ComparisonPairsToTree.py ComparisonRecords.txt conditions.txt Tree.txt
python IllustrateTree.py
python 5_TestingTextThroughTree.py ../VA_UniqueGoodExplod_2009-2010-2012_Training.csv Tree.txt
#python 5_TestingTextThroughTree.py ../VA_UniqueGoodExplod_2009-2010-2012_TrainingSMALL.csv Tree.txt
date

#echo "##### Running: /usr/bin/python 1_ExcelToText.py 0_RecordLinkageInput100Lines.xlsx A1 T101 100Lines.txt"
#/usr/bin/python 1_ExcelToText.py 0_RecordLinkageInput100Lines.xlsx A1 T101 100Lines.txt
#echo "##### Running: python 2_TextToDataPreprocessedText.py 100Lines.txt 100LinesPreProcessed.txt"
#python 2_TextToPreprocessedText.py 100Lines.txt 100LinesPreProcessed.txt

#echo "##### Running: python 3_PreprocessedTextToTree.py 100LinesPreProcessed.txt conditions.txt Tree.txt"
#python 3_PreprocessedTextToTree.py 100LinesPreProcessed.txt conditions.txt Tree.txt
###echo "##### Running: python 4_TestingDataToTree.py 200TestingData.txt Tree.txt"
###python 4_TestingDataToTree.py 200TestingData.txt Tree.txt
