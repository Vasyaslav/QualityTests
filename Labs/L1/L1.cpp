#include <iostream>
#include <utility>
#include <vector>

using namespace std;

vector<int> Routing1(vector<int> input)
{

}

int GetToHouse(int cur, int dest)
{
    if (cur = dest) return 2;
    if (cur % 2 == dest % 2)
    {
        return abs((cur - dest) / 2);
    }
    else
    {
        if (abs(cur - dest) < 4) return 1;


    }
    int time = 0;
    return time;
}

int Housing1(int houses, int dest1, int dest2)
{
    int time = 0;
    return time;
}

int main()
{
    Housing1(10, 4, 7);
    return 0;
}