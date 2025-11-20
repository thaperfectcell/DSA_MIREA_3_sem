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

    return g; // âîçâðàùàåì ñîçäàííûé ãðàô
}

// Ïå÷àòü ìàòðèöû ñìåæíîñòè
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

    vector<int> minEdge(n, INF);   // ìèíèìàëüíûé âåñ ðåáðà, âåäóùåãî â âåðøèíó
    vector<int> selEdge(n, -1);    // îòêóäà ïðèøëè â âåðøèíó
    vector<bool> used(n, false);   // âêëþ÷åíà ëè âåðøèíà â îñòîâ

    minEdge[0] = 0; // íà÷èíàåì ñ âåðøèíû 0 (ìîæíî âûáðàòü ëþáóþ)

    vector<tuple<int,int,int>> mstEdges; // (u, v, w)
    int totalWeight = 0;

    for (int i = 0; i < n; ++i) {
        int v = -1;
        // âûáèðàåì íåèñïîëüçîâàííóþ âåðøèíó ñ ìèíèìàëüíûì minEdge
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

        // äîáàâëÿåì ðåáðî â îñòîâ (êðîìå ñòàðòîâîé âåðøèíû)
        if (selEdge[v] != -1) {
            int u = selEdge[v];
            int w = g[u][v];
            mstEdges.push_back({u, v, w});
            totalWeight += w;
        }

        // îáíîâëÿåì ðàññòîÿíèÿ äî îñòàëüíûõ âåðøèí
        for (int to = 0; to < n; ++to) {
            int w = g[v][to];
            if (w > 0 && !used[to] && w < minEdge[to]) {
                minEdge[to] = w;
                selEdge[to] = v;
            }
        }
    }

    // Âûâîä ðåçóëüòàòà
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

