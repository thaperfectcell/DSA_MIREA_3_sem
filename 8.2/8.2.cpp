#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <chrono>

using namespace std;

unsigned long long bruteOperationsCount = 0;
unsigned long long dpOperationsCount = 0;

const long long INF = numeric_limits<long long>::max() / 4;

long long bruteRecursive(int x, int y,
                         const vector<vector<int>>& a,
                         int n, int m)
{
    if (x >= n || y >= m)
        return INF;

    if (x == n - 1 && y == m - 1)
        return a[x][y];

    bruteOperationsCount += 3;

    long long right  = bruteRecursive(x,     y + 1, a, n, m);
    long long down   = bruteRecursive(x + 1, y,     a, n, m);
    long long diag   = bruteRecursive(x + 1, y + 1, a, n, m);

    long long best = min({right, down, diag});
    if (best == INF) return INF;

    return a[x][y] + best;
}

long long solveDP(const vector<vector<int>>& a, int n, int m)
{
    vector<vector<long long>> dp(n, vector<long long>(m, 0));

    dp[0][0] = a[0][0];
    dpOperationsCount++;

    for (int j = 1; j < m; ++j) {
        dp[0][j] = dp[0][j - 1] + a[0][j];
        dpOperationsCount += 2;
    }

    for (int i = 1; i < n; ++i) {
        dp[i][0] = dp[i - 1][0] + a[i][0];
        dpOperationsCount += 2;
    }

    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            long long bestPrev = min({
                dp[i - 1][j],
                dp[i][j - 1],
                dp[i - 1][j - 1]
            });

            dp[i][j] = a[i][j] + bestPrev;
            dpOperationsCount += 5;
        }
    }

    return dp[n - 1][m - 1];
}

int main()
{
    vector<vector<int>> a = {
        {5,  9,  1,  3, 4, 6, 8},
        {4,  7,  2,  8, 8, 9, 0},
        {6,  3,  5,  1, 1, 3, 4},
        {9,  4,  2,  7, 5, 5, 3},
        {5,  9,  1,  3, 2, 2, 4},
        {5,  1,  3,  3, 7, 9, 8},
        {5,  9,  6,  3, 8, 7, 3},
        {5,  9,  1,  3, 4, 2, 0},
    };

    int n = a.size();
    int m = a[0].size();

    cout << "Matrix:\n";
    for (auto& row : a) {
        for (int x : row) cout << x << " ";
        cout << "\n";
    }
    cout << "\n";

    auto start = chrono::high_resolution_clock::now();
    long long dpAnswer = solveDP(a, n, m);
    auto end = chrono::high_resolution_clock::now();

    long long dpTimeMicro =
        chrono::duration_cast<chrono::microseconds>(end - start).count();

    cout << "DP result: " << dpAnswer << endl;
    cout << "DP operations: " << dpOperationsCount << endl;
    cout << "DP time: " << dpTimeMicro << " microseconds\n\n";

    start = chrono::high_resolution_clock::now();
    long long bruteAnswer = bruteRecursive(0, 0, a, n, m);
    end = chrono::high_resolution_clock::now();

    long long bruteTimeMicro =
        chrono::duration_cast<chrono::microseconds>(end - start).count();

    cout << "Brute result: " << bruteAnswer << endl;
    cout << "Brute operations: " << bruteOperationsCount << endl;
    cout << "Brute time: " << bruteTimeMicro << " microseconds\n";

    return 0;
}
