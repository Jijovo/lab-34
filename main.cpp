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
    map<int, vector<pair<int, int>>> adj;          // fuel consumption in liters
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

    // DFS: explore all possible routes, accumulating fuel cost
    void dfsWithFuel(int start) {
        set<int> visited;
        stack<pair<int, int>> st; // (vertex, accumulated fuel from start)
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

                // Push neighbors in reverse order to maintain natural order in output
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

    // BFS: find the route with the fewest road segments (ignoring fuel), then compute its fuel cost
    void bfsMinHops(int start) {
        set<int> visited;
        queue<int> q;
        map<int, int> parent;   // to reconstruct path
        map<int, int> hops;     // number of hops from start

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

        // Show the hop‑distance and actual fuel consumption for each destination
        cout << "\n=== ROUTE SUMMARY (Minimal hops from " << locationName[start] << ") ===\n";
        for (const auto& entry : adj) {
            int dest = entry.first;
            if (dest == start) continue;
            // reconstruct path
            vector<int> path;
            int cur = dest;
            while (cur != -1) {
                path.push_back(cur);
                cur = parent[cur];
            }
            reverse(path.begin(), path.end());
            // compute total fuel along the path
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

    // Step 4: Dijkstra's algorithm for shortest path (minimum fuel consumption)
    void shortestPaths(int start) {
        map<int, int> dist;          // distance (fuel) from start to each vertex
        map<int, int> prev;          // previous vertex in optimal path
        set<int> unvisited;

        // Initialize distances: INF for all, 0 for start
        for (const auto& entry : adj) {
            int v = entry.first;
            dist[v] = INT_MAX;
            unvisited.insert(v);
        }
        dist[start] = 0;
        prev[start] = -1;

        while (!unvisited.empty()) {
            // Find vertex with smallest distance
            int u = -1;
            int minDist = INT_MAX;
            for (int v : unvisited) {
                if (dist[v] < minDist) {
                    minDist = dist[v];
                    u = v;
                }
            }
            if (u == -1) break; // no more reachable nodes
            unvisited.erase(u);

            // Relax edges from u
            for (const auto& neighbor : adj[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;
                if (unvisited.find(v) != unvisited.end()) { // still unprocessed
                    int newDist = dist[u] + weight;
                    if (newDist < dist[v]) {
                        dist[v] = newDist;
                        prev[v] = u;
                    }
                }
            }
        }

        // Print results
        cout << "\n=== SHORTEST PATHS (Minimum Fuel Consumption) from " << locationName[start] << " ===\n";
        cout << "Using Dijkstra's algorithm (all weights positive).\n\n";
        for (const auto& entry : adj) {
            int v = entry.first;
            if (dist[v] == INT_MAX) {
                cout << locationName[start] << " -> " << locationName[v] << " : NOT REACHABLE\n";
            } else {
                // Reconstruct path
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
};

int main() {
    CityRoutePlanner planner;

    // ---------- Build the exact graph from Step 2 ----------
    // vertices: 0,1,2,4,5,7,8,9,10,11,12   (3 and 6 deleted, 6 new added)
    // edges with fuel weights (liters)
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

    // Assign descriptive names (city landmarks)
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

    // Display the network
    planner.displayNetwork();

    // Run DFS from City Center (vertex 0)
    planner.dfsWithFuel(0);

    // Run BFS from City Center – fewest road segments
    planner.bfsMinHops(0);

    // Step 4: Shortest paths (minimum fuel) using Dijkstra
    planner.shortestPaths(0);

    return 0;
}