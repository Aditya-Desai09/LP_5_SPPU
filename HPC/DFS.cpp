#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

const int MAX = 100000;
vector<int> graph[MAX];    // Adjacency list to store the graph
bool visited[MAX];         // Array to mark visited nodes

// Parallel DFS function
void parallelDFS(int start) {
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int node = s.top();
        s.pop();

        // Visit the node if it has not been visited
        if (!visited[node]) {
            visited[node] = true;
            cout << node << " ";

            // Start a parallel region for neighbors
            #pragma omp parallel
            {
                int num_threads = omp_get_num_threads();
                #pragma omp single
                {
                    cout << "\nNumber of threads: " << num_threads << endl;
                }

                #pragma omp for
                for (int i = 0; i < graph[node].size(); i++) {
                    int neighbor = graph[node][i];
                    if (!visited[neighbor]) {
                        #pragma omp critical
                        {
                            s.push(neighbor);
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
        graph[v].push_back(u);  // Since it's an undirected graph
    }

    // Initialize visited array in parallel
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    cout << "\nParallel DFS Traversal: ";
    parallelDFS(start);
    cout << endl;

    return 0;
}

