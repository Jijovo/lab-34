//Ezzat Mohamadein | ComSc 210 | lab 34
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <iomanip>
#include <algorithm>
#include <climits>

using namespace std;

class CityRoutePlanner {
private:
    map<int, vector<pair<int, int>>> adj;
    map<int, string> locationName;

public:
    void addEdge(int u, int v, int fuel) {
        adj[u].push_back({v, fuel});
        adj[v].push_back({u, fuel});
    }

    void setLocationName(int id, const string& name) {
        locationName[id] = name;
    }

    void displayNetwork() {
        cout << "\n=== CITY ROUTE NETWORK (Fuel consumption in liters) ===\n";
        for (const auto& entry : adj) {
            int v = entry.first;
            cout << locationName[v] << " (ID " << v << ") connects to:\n";
            for (const auto& neighbor : entry.second) {
                int n = neighbor.first;
                int fuel = neighbor.second;
                cout << "  → " << locationName[n] << " (ID " << n << ") - Fuel: " << fuel << " L\n";
            }
            cout << endl;
        }
    }

    void dfsWithFuel(int start) {
        set<int> visited;
        stack<pair<int, int>> st;
        vector<int> order;
        map<int, int> fuelFromStart;

        st.push({start, 0});
        cout << "\n=== ROUTE EXPLORATION (DFS) from " << locationName[start] << " ===\n";
        cout << "Purpose: Find all possible driving routes and their fuel costs.\n\n";

        while (!st.empty()) {
            auto [v, accFuel] = st.top();
            st.pop();
            if (visited.find(v) == visited.end()) {
                visited.insert(v);
                order.push_back(v);
                fuelFromStart[v] = accFuel;
                cout << "Arrive at " << locationName[v] << " (ID " << v << ")";
                if (accFuel > 0)
                    cout << " - Total fuel used so far: " << accFuel << " L";
                cout << endl;

                vector<pair<int, int>> neighbors = adj[v];
                for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
                    int n = it->first;
                    int fuel = it->second;
                    if (visited.find(n) == visited.end()) {
                        st.push({n, accFuel + fuel});
                        cout << "  → Possible drive to " << locationName[n] << " (fuel " << fuel << " L)\n";
                    }
                }
                if (neighbors.empty() || (v != start && neighbors.size() == 1 && visited.count(neighbors[0].first)))
                    cout << "  (Dead end reached)\n";
            }
        }
        cout << "\nDFS traversal order: ";
        for (int v : order) cout << locationName[v] << " ";
        cout << endl;
    }

    void bfsMinHops(int start) {
        set<int> visited;
        queue<int> q;
        map<int, int> parent;
        map<int, int> hops;

        visited.insert(start);
        q.push(start);
        hops[start] = 0;
        parent[start] = -1;

        cout << "\n=== SHORTEST ROUTE BY ROAD SEGMENTS (BFS) from " << locationName[start] << " ===\n";
        cout << "Purpose: Find the route with the fewest road segments, then compute its fuel cost.\n\n";

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            cout << "Checking " << locationName[v] << " (ID " << v << ") - Hop distance: " << hops[v] << "\n";
            for (const auto& neighbor : adj[v]) {
                int n = neighbor.first;
                if (visited.find(n) == visited.end()) {
                    visited.insert(n);
                    parent[n] = v;
                    hops[n] = hops[v] + 1;
                    q.push(n);
                    cout << "  → " << locationName[n] << " reachable in " << hops[n] << " segment(s)\n";
                }
            }
        }

        cout << "\n=== ROUTE SUMMARY (Minimal hops from " << locationName[start] << ") ===\n";
        for (const auto& entry : adj) {
            int dest = entry.first;
            if (dest == start) continue;
            vector<int> path;
            int cur = dest;
            while (cur != -1) {
                path.push_back(cur);
                cur = parent[cur];
            }
            reverse(path.begin(), path.end());
            int totalFuel = 0;
            for (size_t i = 0; i < path.size() - 1; ++i) {
                int u = path[i], v = path[i+1];
                for (const auto& edge : adj[u]) {
                    if (edge.first == v) {
                        totalFuel += edge.second;
                        break;
                    }
                }
            }
            cout << "To " << locationName[dest] << " (ID " << dest << "): "
                 << hops[dest] << " segment(s), total fuel = " << totalFuel << " L, path: ";
            for (int node : path) cout << locationName[node] << " ";
            cout << endl;
        }
    }

    void shortestPaths(int start) {
        map<int, int> dist;
        map<int, int> prev;
        set<int> unvisited;

        for (const auto& entry : adj) {
            int v = entry.first;
            dist[v] = INT_MAX;
            unvisited.insert(v);
        }
        dist[start] = 0;
        prev[start] = -1;

        while (!unvisited.empty()) {
            int u = -1;
            int minDist = INT_MAX;
            for (int v : unvisited) {
                if (dist[v] < minDist) {
                    minDist = dist[v];
                    u = v;
                }
            }
            if (u == -1) break;
            unvisited.erase(u);

            for (const auto& neighbor : adj[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;
                if (unvisited.find(v) != unvisited.end()) {
                    int newDist = dist[u] + weight;
                    if (newDist < dist[v]) {
                        dist[v] = newDist;
                        prev[v] = u;
                    }
                }
            }
        }

        cout << "\n=== SHORTEST PATHS (Minimum Fuel Consumption) from " << locationName[start] << " ===\n";
        cout << "Using Dijkstra's algorithm.\n\n";
        for (const auto& entry : adj) {
            int v = entry.first;
            if (dist[v] == INT_MAX) {
                cout << locationName[start] << " -> " << locationName[v] << " : NOT REACHABLE\n";
            } else {
                vector<int> path;
                int cur = v;
                while (cur != -1) {
                    path.push_back(cur);
                    cur = prev[cur];
                }
                reverse(path.begin(), path.end());
                cout << locationName[start] << " -> " << locationName[v] << " : " << dist[v] << " L, path: ";
                for (int node : path) cout << locationName[node] << " ";
                cout << endl;
            }
        }
    }

    void minimumSpanningTree(int start) {
        set<int> inMST;
        map<int, int> minEdgeWeight;
        map<int, int> parent;
        set<int> notInMST;

        for (const auto& entry : adj) {
            int v = entry.first;
            notInMST.insert(v);
            minEdgeWeight[v] = INT_MAX;
            parent[v] = -1;
        }
        minEdgeWeight[start] = 0;

        cout << "\n=== MINIMUM SPANNING TREE (Prim's algorithm) ===\n";
        cout << "Edges in MST (fuel consumption as weight):\n";

        while (!notInMST.empty()) {
            int u = -1;
            int minW = INT_MAX;
            for (int v : notInMST) {
                if (minEdgeWeight[v] < minW) {
                    minW = minEdgeWeight[v];
                    u = v;
                }
            }
            if (u == -1) break;

            notInMST.erase(u);
            inMST.insert(u);

            if (parent[u] != -1) {
                cout << "Edge from " << parent[u] << " to " << u << " with weight: " << minEdgeWeight[u] << " units\n";
            }

            for (const auto& neighbor : adj[u]) {
                int v = neighbor.first;
                int w = neighbor.second;
                if (inMST.find(v) == inMST.end() && w < minEdgeWeight[v]) {
                    minEdgeWeight[v] = w;
                    parent[v] = u;
                }
            }
        }
    }
};

int main() {
    CityRoutePlanner planner;

    // Build the graph (same as Steps 2-5)
    planner.addEdge(0, 1, 5);
    planner.addEdge(0, 2, 10);
    planner.addEdge(0, 4, 15);
    planner.addEdge(1, 2, 3);
    planner.addEdge(1, 5, 8);
    planner.addEdge(2, 7, 12);
    planner.addEdge(2, 8, 6);
    planner.addEdge(4, 5, 4);
    planner.addEdge(4, 9, 7);
    planner.addEdge(5, 7, 9);
    planner.addEdge(5, 8, 11);
    planner.addEdge(7, 8, 2);
    planner.addEdge(7, 10, 13);
    planner.addEdge(8, 11, 14);
    planner.addEdge(9, 10, 1);
    planner.addEdge(10, 11, 16);
    planner.addEdge(11, 12, 17);
    planner.addEdge(9, 12, 18);

    planner.setLocationName(0, "City Center");
    planner.setLocationName(1, "North District");
    planner.setLocationName(2, "East District");
    planner.setLocationName(4, "South District");
    planner.setLocationName(5, "West District");
    planner.setLocationName(7, "International Airport");
    planner.setLocationName(8, "Central Train Station");
    planner.setLocationName(9, "City Stadium");
    planner.setLocationName(10, "Mega Mall");
    planner.setLocationName(11, "General Hospital");
    planner.setLocationName(12, "University Campus");

    //step 6, add menu
    int choice;
    do {
        cout << endl << "========================================" << endl;
        cout << "   CITY ROUTE PLANNER MENU" << endl;
        cout << "========================================" << endl;
        cout << "[1] Display network (adjacency list)" << endl;
        cout << "[2] Plan inspection route (DFS from City Center)" << endl;
        cout << "[3] Find minimal hop routes (BFS from City Center)" << endl;
        cout << "[4] Calculate shortest paths by fuel (Dijkstra)" << endl;
        cout << "[5] Find Minimum Spanning Tree (Prim)" << endl;
        cout << "[0] Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1:
                planner.displayNetwork();
                break;
            case 2:
                planner.dfsWithFuel(0);
                break;
            case 3:
                planner.bfsMinHops(0);
                break;
            case 4:
                planner.shortestPaths(0);
                break;
            case 5:
                planner.minimumSpanningTree(0);
                break;
            case 0:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}