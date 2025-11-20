#include <iostream>
#include <vector>
#include <iomanip>
#include <tuple>

using namespace std;

vector<vector<int>> createGraphAdjMatrix() {
    int n;
    cout << "Enter number of graph's verticles: ";
    cin >> n;

    vector<vector<int>> g(n, vector<int>(n));
    cout << "Enter adjacency matrix" << n << " lines with " << n
         << " integers, 0 means absence of an edge:\n";

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> g[i][j];
        }
    }

    return g; // возвращаем созданный граф
}

// Печать матрицы смежности
void printMatrix(const vector<vector<int>>& g) {
    int n = g.size();
    cout << "Graph's adjacency matrix:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << setw(3) << g[i][j] << " ";
        }
        cout << "\n";
    }
}

void primMST(const vector<vector<int>>& g) {
    const int INF = 1000000000;
    int n = g.size();

    vector<int> minEdge(n, INF);   // минимальный вес ребра, ведущего в вершину
    vector<int> selEdge(n, -1);    // откуда пришли в вершину
    vector<bool> used(n, false);   // включена ли вершина в остов

    minEdge[0] = 0; // начинаем с вершины 0 (можно выбрать любую)

    vector<tuple<int,int,int>> mstEdges; // (u, v, w)
    int totalWeight = 0;

    for (int i = 0; i < n; ++i) {
        int v = -1;
        // выбираем неиспользованную вершину с минимальным minEdge
        for (int j = 0; j < n; ++j) {
            if (!used[j] && (v == -1 || minEdge[j] < minEdge[v])) {
                v = j;
            }
        }

        if (minEdge[v] == INF) {
            cout << "Graph is disconnected, it's not possible to build a spanning tree.\n";
            return;
        }

        used[v] = true;

        // добавляем ребро в остов (кроме стартовой вершины)
        if (selEdge[v] != -1) {
            int u = selEdge[v];
            int w = g[u][v];
            mstEdges.push_back({u, v, w});
            totalWeight += w;
        }

        // обновляем расстояния до остальных вершин
        for (int to = 0; to < n; ++to) {
            int w = g[v][to];
            if (w > 0 && !used[to] && w < minEdge[to]) {
                minEdge[to] = w;
                selEdge[to] = v;
            }
        }
    }

    // Вывод результата
    cout << "\nMinimal spanning tree (Prim's algorithm):\n";
    cout << "Edges (verticles numbered from 1):\n";
    for (auto [u, v, w] : mstEdges) {
        cout << (u + 1) << " - " << (v + 1) << " (weight = " << w << ")\n";
    }
    cout << "Total spanning tree weight: " << totalWeight << "\n";
}

int main() {
    vector<vector<int>> g = createGraphAdjMatrix();
    cout << "\n";
    printMatrix(g);

    primMST(g);

    return 0;
}

