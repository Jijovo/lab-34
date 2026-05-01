//Ezzat Mohamadein | ComSc 210 | lab 34
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>

using namespace std;

class Graph {
private:
    map<int, vector<pair<int, int>>> adj;

public:
    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void displayAdj() {
        for (const auto& entry : adj) {
            cout << entry.first << " -->";
            for (const auto& p : entry.second) {
                cout << " (" << p.first << ", " << p.second << ")";
            }
            cout << endl;
        }
    }

    // Iterative DFS (proper stack‑based, no artificial reordering)
    vector<int> dfs(int start) {
        set<int> visited;
        stack<int> st;
        vector<int> order;
        st.push(start);
        while (!st.empty()) {
            int v = st.top();
            st.pop();
            if (visited.find(v) == visited.end()) {
                visited.insert(v);
                order.push_back(v);
                // Push neighbors in the order they appear in the adjacency list.
                // The last one pushed will be visited first (LIFO).
                for (const auto& neighbor : adj[v]) {
                    int n = neighbor.first;
                    if (visited.find(n) == visited.end()) {
                        st.push(n);
                    }
                }
            }
        }
        return order;
    }

    // BFS (proper queue‑based)
    vector<int> bfs(int start) {
        set<int> visited;
        queue<int> q;
        vector<int> order;
        visited.insert(start);
        q.push(start);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            order.push_back(v);
            for (const auto& neighbor : adj[v]) {
                int n = neighbor.first;
                if (visited.find(n) == visited.end()) {
                    visited.insert(n);
                    q.push(n);
                }
            }
        }
        return order;
    }
};

int main() {
    Graph g;

    //step 2: Delete vertices 3 and 6 from the original graph.
    //add six new vertices: 7, 8, 9, 10, 11, 12.
    //change all edge weights.
    //edges are added in an order that gives a clear adjacency list.

    //connect vertex 0
    g.addEdge(0, 1, 5);
    g.addEdge(0, 2, 10);
    g.addEdge(0, 4, 15);

    //connect vertex 1
    g.addEdge(1, 2, 3);
    g.addEdge(1, 5, 8);

    //connect vertex 2
    g.addEdge(2, 7, 12);
    g.addEdge(2, 8, 6);

    //connect vertex 4
    g.addEdge(4, 5, 4);
    g.addEdge(4, 9, 7);

    //connect vertex 5
    g.addEdge(5, 7, 9);
    g.addEdge(5, 8, 11);

    //connect new vertices among themselves
    g.addEdge(7, 8, 2);
    g.addEdge(7, 10, 13);
    g.addEdge(8, 11, 14);
    g.addEdge(9, 10, 1);
    g.addEdge(10, 11, 16);
    g.addEdge(11, 12, 17);
    g.addEdge(9, 12, 18);  //extra connection to keep everything reachable

    cout << "Graph's adjacency list:" << endl;
    g.displayAdj();

    cout << "\nDFS starting from vertex 0:" << endl;
    vector<int> dfs_order = g.dfs(0);
    for (int v : dfs_order) cout << v << " ";
    cout << endl;

    cout << "BFS starting from vertex 0:" << endl;
    vector<int> bfs_order = g.bfs(0);
    for (int v : bfs_order) cout << v << " ";
    cout << endl;

    return 0;
}