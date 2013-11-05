#include <iostream>
#include <boost/thread.hpp>

using namespace std; 
using namespace boost; 

void task1()
{ 
    cout << endl << " --- task1";
}

void task2()
{ 
    cout << endl << " --- task2";
}

int main (int argc, char ** argv)
{
    thread thread_1 = thread(task1);
    thread thread_2 = thread(task2);

    // do other stuff
    thread_2.join();
    thread_1.join();
    return 0;
}
