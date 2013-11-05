g++ test1.cpp -o test1 -I /usr/local/boost_1_54_0
g++ test1.cpp -o test1
echo 1 2 3 | ./test1
g++ test2.cpp -o test2 -I /usr/local/include -L /usr/local/lib -l boost_thread -l boost_system
./test2
