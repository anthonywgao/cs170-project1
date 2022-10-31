#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
using namespace std;

vector<int> use_default()
{
    return {1, 2, 3, 4, 5, 6, 7, 0, 8};
}
vector<int> use_original()
{
    vector<int> v;
    for (int i = 0; i < 3; ++i)
    {
        cout << "Please enter line " << i + 1 << endl;
        for (int j = 0; j < 3; ++j)
        {
            int temp;
            cin >> temp;
            v.push_back(temp);
        }
    }
    return v;
}

int main()
{
    // menu stuff
    cout << "Eight puzzle solver" << endl;
    cout << "Enter 1 for default puzzle or 2 for original puzzle" << endl;
    int choice1;
    cin >> choice1;
    vector<int> startpos;
    switch (choice1)
    {
    case 2:
        startpos = use_original();
        break;
    default:
        startpos = use_default();
        break;
    }

    return 0;
}