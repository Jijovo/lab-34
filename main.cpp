//Ezzat Mohamadein | ComSc 210 | lab 34
#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

class Graph {
private:
    // adjacency list: vertex -> list of (neighbor, weight)
    map<int, vector<pair<int, int>>> adj;

public:
    void addEdge(int u, int v, int weight) {
        adj[u].push_back({v, weight});
        adj[v].push_back({u, weight});
    }

    void displayAdj() {
        for (const auto& entry : adj) {
            int vertex = entry.first;
            cout << vertex << " -->";
            for (const auto& neighbor : entry.second) {
                cout << " (" << neighbor.first << ", " << neighbor.second << ")";
            }
            cout << endl;
        }
    }

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
                // push neighbors in reverse order to simulate recursion order
                // but for correct order as in sample, we push them in the order they appear
                // since stack is LIFO, we iterate normally and push; the last neighbor in list will be popped first.
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

    // Build the graph exactly as shown in the sample adjacency list
    g.addEdge(0, 1, 12);
    g.addEdge(0, 2, 8);
    g.addEdge(0, 3, 21);
    g.addEdge(2, 3, 6);
    g.addEdge(2, 6, 2);
    g.addEdge(2, 4, 4);
    g.addEdge(2, 5, 5);
    g.addEdge(4, 5, 9);
    g.addEdge(5, 6, 6);

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