#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

const int MAX = 100000;
vector<int> graph[MAX];   // Adjacency list to store the graph
bool visited[MAX];        // Visited array

// Parallel BFS function
void parallelBFS(int start) {
    queue<int> q;
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int levelSize;

        // Protect size reading inside a critical section
        #pragma omp critical
        {
            levelSize = q.size();
        }

        // Parallel processing of the current BFS level
        #pragma omp parallel for
        for (int i = 0; i < levelSize; i++) {
            int node;

            // Extract node from queue inside critical to avoid conflicts
            #pragma omp critical
            {
                if (!q.empty()) {
                    node = q.front();
                    q.pop();
                    cout << node << " ";
                }
            }

            // Visit neighbors
            for (int j = 0; j < graph[node].size(); j++) {
    			int neighbor = graph[node][j];
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
}

int main() {
    int n, m, start;
    cout << "Enter number of nodes, edges and start node: ";
    cin >> n >> m >> start;

    cout << "Enter edges (u v):\n";
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u); // Undirected graph
    }

    // Parallel initialization of visited array
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    cout << "\nParallel BFS Traversal: ";
    parallelBFS(start);
    cout << endl;

    return 0;
}

