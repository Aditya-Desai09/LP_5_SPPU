#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

const int MAX = 100000;  // Maximum number of nodes
vector<int> graph[MAX];  // Adjacency list to store the graph
bool visited[MAX];       // Visited array for BFS and DFS

// Parallel DFS function
void parallelDFS(int start) {
    stack<int> s;
    visited[start] = true;
    s.push(start);

    cout << "\nParallel DFS Traversal: ";

    while (!s.empty()) {
        int node = s.top();
        s.pop();
        cout << node << " ";

        // Parallelize the traversal of neighbors of the current node
        #pragma omp parallel for
        for (int i = 0; i < graph[node].size(); i++) {
            int adj_node = graph[node][i];
            if (!visited[adj_node]) {
                #pragma omp critical
                {
                    if (!visited[adj_node]) {
                        visited[adj_node] = true;
                        s.push(adj_node);
                    }
                }
            }
        }
    }
}

// Parallel BFS function
void parallelBFS(int start) {
    queue<int> q;
    visited[start] = true;
    q.push(start);

    cout << "\nParallel BFS Traversal: ";

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";

        // Parallel processing of the neighbors
        #pragma omp parallel for
        for (int i = 0; i < graph[node].size(); i++) {
            int neighbor = graph[node][i];
            if (!visited[neighbor]) {
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }
}

int main() {
    int n, m, start_node;
    
    // User input for graph size and edges
    cout << "Enter number of nodes and edges: ";
    cin >> n >> m;
    
    cout << "Enter the edges (u v) of the graph:\n";
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);  // Undirected graph
    }

    // Ask for the starting node
    cout << "Enter the starting node: ";
    cin >> start_node;

    // Initialize visited array to false for both BFS and DFS
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    // Perform Parallel DFS
    parallelDFS(start_node);

    // Reinitialize visited array to false for BFS (since BFS and DFS share the same visited array)
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    // Perform Parallel BFS
    parallelBFS(start_node);

    cout << endl;
    return 0;
}

