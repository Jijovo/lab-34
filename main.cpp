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
    // adjacency list: vertex -> list of (neighbor, weight)
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

    // Iterative DFS using a stack.
    // Pushes neighbors onto the stack in the order they appear in the adjacency list.
    // This yields exactly the sample order when the adjacency list is built as shown.
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
                // Push neighbors in the order they appear in the list.
                // The stack is LIFO, so the last neighbor pushed will be visited first.
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

    // BFS using a queue.
    // Enqueues neighbors in the order they appear in the adjacency list.
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

    // Add edges in the exact order that replicates the sample adjacency list.
    // This order is derived from the sample output:
    // 0 --> (1,12) (2,8) (3,21)
    // 2 --> (0,8) (3,6) (6,2) (4,4) (5,5)
    // 3 --> (0,21) (2,6)
    // 4 --> (5,9) (2,4)
    // 5 --> (6,6) (4,9) (2,5)
    // 6 --> (2,2) (5,6)
    g.addEdge(0, 1, 12);   // step 1
    g.addEdge(0, 2, 8);    // step 2
    g.addEdge(0, 3, 21);   // step 3
    g.addEdge(2, 3, 6);    // step 4
    g.addEdge(2, 6, 2);    // step 5
    g.addEdge(5, 6, 6);    // step 6 – gives (5,6) before (4,5) and (2,5)
    g.addEdge(4, 5, 9);    // step 7 – gives (4,5) before (4,2)
    g.addEdge(2, 4, 4);    // step 8 – adds (2,4) after (2,6)
    g.addEdge(2, 5, 5);    // step 9 – adds (2,5) last

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