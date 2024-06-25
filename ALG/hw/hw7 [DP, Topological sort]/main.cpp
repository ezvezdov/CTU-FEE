// https://www.youtube.com/watch?v=eL-KzMXSXXI
// https://www.geeksforgeeks.org/graph-and-its-representations/

#include<bits/stdc++.h>

#define MAX_R 100002
#define MAX_P 22

using namespace std;

typedef struct edge{
    int dst;
    int E;
    int Z;
} edge;

vector<edge> graph[MAX_R];
int arr[MAX_R][MAX_P];

// Usual Deep-First-Search for topsort() function
void dfs(int cur_v,int *visited_n, vector<bool> &visited,vector<int> &topsort_v){
    for(int i = 0; i < graph[cur_v].size(); i++){
        if(!visited[graph[cur_v][i].dst]){
            dfs(graph[cur_v][i].dst,visited_n,visited,topsort_v);
        }
        
    }
    visited[cur_v] = true;
    topsort_v.push_back(cur_v);
    *visited_n = *visited_n + 1;
    return;
}

// Find topological sort of graph
void topsort(vector<int> &topsort_v, int R){
    vector<bool> visited(R+1,false);
    int visited_n = 0;
    for(int i = 1; i < visited.size(); i++){
        if(visited[i] == false){
            dfs(i,&visited_n,visited,topsort_v);
        }
    }
}

int main(){
    int R, T, P;
    int R1, R2, E, Z;

    cin >> R >> T >> P;
    for (int i = 0; i < T; i++){
        cin >> R1 >> R2 >> E >> Z;
        graph[R1].push_back({R2,E,Z});
    }
    
    vector<int> topsort_v;
    topsort(topsort_v,R);
    
    // Generate min_z vector (minimal payment amount to return back to start point) 
    vector<int> min_z(R+1, INT_MAX);
    min_z[1] = 0;
    for(int i = 0; i < topsort_v.size(); i++){
        for(int j = 0; j < graph[topsort_v[i]].size(); j++){
            min_z[topsort_v[i]] = min(min_z[topsort_v[i]],
                                    graph[topsort_v[i]][j].Z + min_z[graph[topsort_v[i]][j].dst]
            );
        }
    }

    // Calculate maximal turisticky zisk
    for(int i = topsort_v.size()-1; i >= 0; i--){
        int cur_v = topsort_v[i];

        for(int j = 0; j < graph[cur_v].size(); j++){
            int neighbour = graph[cur_v][j].dst;
            int should_pay = graph[cur_v][j].Z;
            
            for(int k = 0; k+should_pay <= P; k++){

                if(arr[cur_v][k] == 0){
                    // Start from this node
                    if(should_pay + min_z[cur_v] <= P){
                        arr[neighbour][k + should_pay + min_z[cur_v]] = max(graph[cur_v][j].E,arr[neighbour][k+ should_pay+ min_z[cur_v]]);
                    }
                    continue;
                }

                // Update exising data
                arr[neighbour][k+should_pay] = max(arr[cur_v][k] + graph[cur_v][j].E,arr[neighbour][k+should_pay]);                
            }
        }
        
    }

    // Get max answer and print it.
    int max_e = 0;
    for(int i = 0; i <= P; i++){
        max_e = max(max_e,arr[1][i]);
    }
    cout << max_e << endl;

}
