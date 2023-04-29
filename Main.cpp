#include <iostream>
#include <vector>
#include <cstring>
#include <climits>

using namespace std;

const int n = 4;
const int MAX = 1;
int dist[n + 1][n + 1] = {
{0 ,0 , 0 , 0 , 0 },
{0 ,0 , 2 , 2 , 3 },
{0 ,2 , 0 , 4 , 5 },
{0 ,2 , 4 , 0 , 8 },
{0 ,3 , 5 , 8 , 0 }};
int memo[n + 1][1 << (n + 1)];
int fun(int i, int mask);

void readfile(){
    
}


// Helper function to print path
void printPath(int i, int mask, vector<int> &path)
{
    // Base case
    if (mask == ((1 << i) | 3))
    {
        path.push_back(i);
        return;
    }

    // Recursion
    for (int j = 1; j <= n; j++)
    {
        if ((mask & (1 << j)) && j != i && j != 1)
        {
            int subproblem = fun(j, mask & (~(1 << i))) + dist[j][i];
            if (memo[i][mask] == subproblem)
            {
                printPath(j, mask & (~(1 << i)), path);
                path.push_back(i);
                return;
            }
        }
    }
}

int fun(int i, int mask)
{
    if (mask == ((1 << i) | 3))
        return dist[1][i];
    if (memo[i][mask] != 0)
        return memo[i][mask];

    int res = MAX;
    for (int j = 1; j <= n; j++)
    {
        if ((mask & (1 << j)) && j != i && j != 1)
        {
            int subproblem = fun(j, mask & (~(1 << i))) + dist[j][i];
            res = min(res, subproblem);
        }
    }

    memo[i][mask] = res;
    return res;
}

int main()
{
    int ans = MAX;
    for (int i = 1; i <= n; i++)
    {
        int subproblem = fun(i, (1 << (n + 1)) - 1) + dist[i][1];
        if (subproblem < ans)
            ans = subproblem;
    }

    vector<int> path;
    for (int i = 1; i <= n; i++)
    {
        int subproblem = fun(i, (1 << (n + 1)) - 1) + dist[i][1];
        if (subproblem == ans)
        {
            printPath(i, (1 << (n + 1)) - 1, path);
            break;
        }
    }

    cout << "The cost of most efficient tour = " << ans << endl;
    cout << "Path: ";
    cout << "1" << " -> ";
    for (int i = path.size() - 1; i >= 0; i--)
    {
        cout << path[i];
        if (i != 0)
            cout << " -> ";
    }
    cout  << " -> "<< "1";

    return 0;
}
