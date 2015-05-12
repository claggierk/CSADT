### For any shell script:

/usr/bin/dos2unix [shell_script]

### Clone the PeopleRL git repository:

git clone git@github.com:claggierk/PeopleRL.git

### Set up the input data:

1. mv person_data.tar.gz PeopleRL
2. cd PeopleRL
3. tar -zxvf person_data.tar.gz

### Convert input csv to txt and compute Levenstein distances of pairs:

1. This is used later, when the accuracy is computed.
2. cd csadt_python
3. vim run.sh to edit the "inputFile" variable (default is ../10.csv)
4. ./run.sh
   - creates 'Training.txt' and 'ComparisonRecords.txt'

### Determine unique pairs of persons:

1. cd ../csadt_cpp/generate-unique-pairs
2. ./run
   - creates 'UniqueCombinations.txt'

### Compute the Levenstein distances of person-pairs:

1. cd ../generate-input/
2. ./run
 - creates 'ADT_ready.txt'

### Generate conditions:

1. needed only if running the python implementation)
2. cd ../generate-conditions
3. ./run
   - creates 'Conditions.txt'

### Run the ADT/CSADT algorithm:

1. cd ../adt
2. vim adt.cpp
 - edit NUM_THREADS_TO_RUN_SIMULTANEOUSLY
 - edit costPlus
 - edit costMinus
3. vim run.sh to edit the "numTreeNodes" variable
4. ./run.sh
 - creates '[1 through numTreeNodes]-Tree.txt'

### Translate the generated tree:

1. cd ../
2. python ../csadt_python/translate-tree/translate-tree.py adt/[numTreeNodes]-Tree.txt TreeCPPReady.txt
   - creates TreeCPPReady.txt

###  Illustrate the generated tree:

1. python ../csadt_python/illustrate-tree/illustrate-tree.py TreeCPPReady.txt CS_ADT_Tree.png
   - creates CS_ADT_Tree.png

### Compute accuracy of generated tree

1. python ../csadt_python/classification-accuracy/classification-accuracy.py ../csadt_python/ComparisonRecords.txt TreeCPPReady.txt
   - creates 2-Accuracy.txt,  2-Precision.txt, and  2-Recall.txt
