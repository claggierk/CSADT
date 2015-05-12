For any shell script:
* /usr/bin/dos2unix [shell_script]

h2. Clone the PeopleRL git repository:
git clone git@github.com:claggierk/PeopleRL.git

h2. Set up the input data:
mv person_data.tar.gz PeopleRL
cd PeopleRL
tar -zxvf person_data.tar.gz

h2. Convert input csv to txt and compute Levenstein distances of pairs:
This is used later, when the accuracy is computed.
cd csadt_python
vim run.sh to edit the "inputFile" variable (default is ../10.csv)
./run.sh
   - creates 'Training.txt' and 'ComparisonRecords.txt'

h2. Determine unique pairs of persons:
cd ../csadt_cpp/generate-unique-pairs
./run
   - creates 'UniqueCombinations.txt'

h2. Compute the Levenstein distances of person-pairs:
cd ../generate-input/
./run
 - creates 'ADT_ready.txt'

h2. Generate conditions:
 * needed only if running the python implementation)
# cd ../generate-conditions
# ./run
#   - creates 'Conditions.txt'

h2. Run the ADT/CSADT algorithm:
cd ../adt
vim adt.cpp
 - edit NUM_THREADS_TO_RUN_SIMULTANEOUSLY
 - edit costPlus

 - edit costMinus
vim run.sh to edit the "numTreeNodes" variable
./run.sh
 - creates '[1 through numTreeNodes]-Tree.txt'

h2. Translate the generated tree:
cd ../
python ../csadt_python/translate-tree/translate-tree.py adt/[numTreeNodes]-Tree.txt TreeCPPReady.txt
   - creates TreeCPPReady.txt

h2. Illustrate the generated tree:
python ../csadt_python/illustrate-tree/illustrate-tree.py TreeCPPReady.txt CS_ADT_Tree.png
   - creates CS_ADT_Tree.png

h2. Compute accuracy of generated tree
python ../csadt_python/classification-accuracy/classification-accuracy.py ../csadt_python/ComparisonRecords.txt TreeCPPReady.txt
   - creates 2-Accuracy.txt,  2-Precision.txt, and  2-Recall.txt
