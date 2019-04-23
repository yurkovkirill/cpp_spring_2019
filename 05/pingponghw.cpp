#include <iostream>
#include <thread>
#include <atomic>
const int count = 500000;
using namespace std;

std::atomic<bool> check;

void ping_pong(const char * string, bool value)
{
    for(int i = 0; i < count; i++)
    {
        while(check == value);//ждем другой поток
        cout << string << i << endl;
        check = value;//переход в другой поток
    }
}

int main()
{
    check = true;
   	
    std::thread t1(ping_pong, "ping", false);
    std::thread t2(ping_pong, "pong", true);
    
    t1.join();
    t2.join();
    
    return 0;
}