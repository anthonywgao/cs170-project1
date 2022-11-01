#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <chrono>
#include <functional>
using namespace std;
using namespace std::chrono;

class Node
{
public:
    vector<int> curboard;
    int hn;
    int gn;
    int fn;
    void print()
    {
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                cout << curboard.at(3 * i + j) << " ";
            }
            cout << endl;
        }
    }
};
bool operator<(const Node &n1, const Node &n2)
{
    return n1.fn > n2.fn;
}

class Problem
{
public:
    Problem(vector<int> startpos)
    {
        board = startpos;
        goal = {1, 2, 3, 4, 5, 6, 7, 8, 0};
        numNodes = 0;
    }
    // start position
    vector<int> board;
    // end position
    vector<int> goal;
    // tracks how many nodes are expanded
    int numNodes;
    // set of visited states
    set<vector<int>> nodeset;
    int findzero(vector<int> state)
    {
        for (int i = 0; i < state.size(); ++i)
        {
            if (state.at(i) == 0)
            {
                return i;
            }
        }
        return 0;
    }
    bool unvisited(vector<int> state)
    {
        if (nodeset.find(state) == nodeset.end())
        {
            nodeset.insert(state);
            return true;
        }
        return false;
    }
    vector<Node> operators(Node node)
    {
        numNodes++;
        vector<Node> v;
        vector<int> state = node.curboard;
        int z = findzero(state);
        // move left
        if ((z % 3) > 0)
        {
            Node left = node;
            swap(left.curboard.at(z), left.curboard.at(z - 1));
            left.gn++;
            if (unvisited(left.curboard))
                v.push_back(left);
        }
        // move right
        if ((z % 3) < 2)
        {
            Node right = node;
            swap(right.curboard.at(z), right.curboard.at(z + 1));
            right.gn++;
            if (unvisited(right.curboard))
                v.push_back(right);
        }
        // move up
        if (z > 2)
        {
            Node up = node;
            swap(up.curboard.at(z), up.curboard.at(z - 3));
            up.gn++;
            if (unvisited(up.curboard))
                v.push_back(up);
        }
        // move down
        if (z < 6)
        {
            Node down = node;
            swap(down.curboard.at(z), down.curboard.at(z + 3));
            down.gn++;
            if (unvisited(down.curboard))
                v.push_back(down);
        }
        return v;
    }
};

priority_queue<Node> uniform_cost(priority_queue<Node> nodes, vector<Node> expand)
{
    priority_queue<Node> q = nodes;
    for (Node node : expand)
    {
        node.hn = 0;
        node.fn = node.gn;
        q.push(node);
    }
    return q;
}
int misplaced(vector<int> v)
{
    int d = 0;
    vector<int> goal = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    for (int i = 0; i < v.size(); ++i)
    {
        if (v.at(i) != 0 && v.at(i) != goal.at(i))
        {
            d++;
        }
    }
    return d;
}
priority_queue<Node> a_star_misplaced(priority_queue<Node> nodes, vector<Node> expand)
{
    priority_queue<Node> q = nodes;
    for (Node node : expand)
    {
        node.hn = misplaced(node.curboard);
        node.fn = node.gn + node.hn;
        q.push(node);
    }
    return q;
}
int manhattan(vector<int> v)
{
    int d = 0;
    vector<int> goal = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    for (int i = 0; i < v.size(); ++i)
    {
        if (v.at(i) != 0 && v.at(i) != goal.at(i))
        {
            // diff in row + diff in column
            d += abs(v.at(i) / 3 - i / 3) + abs(v.at(i) % 3 - i % 3);
        }
    }
    return d;
}
priority_queue<Node> a_star_manhattan(priority_queue<Node> nodes, vector<Node> expand)
{
    priority_queue<Node> q = nodes;
    for (Node node : expand)
    {
        node.hn = manhattan(node.curboard);
        node.fn = node.gn + node.hn;
        q.push(node);
    }
    return q;
}
// same search as in lecture
void generalSearch(Problem prob, function<priority_queue<Node>(priority_queue<Node>, vector<Node>)> qfunc)
{
    priority_queue<Node> nodes;
    // initialize first node
    Node node0;
    node0.curboard = prob.board;
    node0.hn = 0;
    node0.gn = 0;
    node0.fn = 0;
    nodes.push(node0);
    // max queue size
    int maxSize = 1;
    while (1)
    {
        if (nodes.empty())
        {
            cout << "No solution" << endl;
            return;
        }
        // priority queue will return lowest fn node
        Node node = nodes.top();
        nodes.pop();
        // prints gn and hn and state
        cout << "Expanding to state with g(n) = " << node.gn << " and h(n) = " << node.hn << endl;
        node.print();
        // reach goal
        if (node.curboard == prob.goal)
        {
            cout << "Goal!" << endl;
            cout << "Expanded " << prob.numNodes << " nodes" << endl;
            cout << "Max node queue size was " << maxSize << endl;
            cout << "The depth of the goal node is " << node.gn << endl;
            return;
        }
        nodes = qfunc(nodes, prob.operators(node));
        if (nodes.size() > maxSize)
        {
            maxSize = nodes.size();
        }
    }
}

vector<int> use_default()
{
    cout << "Select difficulty from 1-5" << endl;
    int dif;
    cin >> dif;
    switch (dif)
    {
    // depth 2
    case 2:
        return {1, 2, 3, 4, 5, 6, 0, 7, 8};
    // depth 8
    case 3:
        return {1, 3, 6, 5, 0, 2, 4, 7, 8};
    // depth 16
    case 4:
        return {1, 6, 7, 5, 0, 3, 4, 8, 2};
    // depth 24
    case 5:
        return {0, 7, 2, 4, 6, 1, 3, 5, 8};
    // depth 0
    default:
        return {1, 2, 3, 4, 5, 6, 7, 8, 0};
    }
    return {1, 2, 3, 4, 5, 6, 7, 8, 0};
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
    Problem prob(startpos);
    cout << "Enter 1 for uniform cost, 2 for A* with misplaced tile, or 3 for A* with manhattan" << endl;
    int choice2;
    cin >> choice2;
    // track execution time
    auto start = high_resolution_clock::now();
    switch (choice2)
    {
    case 2:
        generalSearch(prob, &a_star_misplaced);
        break;
    case 3:
        generalSearch(prob, &a_star_manhattan);
        break;
    default:
        generalSearch(prob, &uniform_cost);
        break;
    }
    auto elapsed = duration_cast<seconds>(high_resolution_clock::now() - start);
    cout << "Time elapsed: " << elapsed.count() << " seconds" << endl;
    return 0;
}