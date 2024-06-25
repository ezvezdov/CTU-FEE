#include<bits/stdc++.h>

#define MAX_SIZE 751
#define MAX_F 31

using namespace std;

typedef struct
{
    int i;
    int j;
    int f;
    int count;
} Element;

// 3d cube, with F layers.
int map_count[MAX_SIZE][MAX_SIZE][MAX_F];

int main(){

    int R,C,F, Sr, Sc, Tr, Tc;
    cin >> R >> C >> F >> Sr >> Sc >> Tr >> Tc;
    Sr--;Sc--;Tr--;Tc--;

    bool map[R+1][C+1];
    for(int i = 0; i < R; i++){
        for(int j = 0; j < C; j++){
            cin >> map[i][j];
        }
    }

    queue< Element > q; 
    q.push({Sr,Sc,0});
    
    int min_sum = INT_MAX;
    int i,j,f, count;
    Element cur_element;
    
    // 3D cube BFS
    while (!q.empty())
    {   
        cur_element = q.front();
        i = cur_element.i; 
        j = cur_element.j; 
        f = cur_element.f;
        count = cur_element.count;
        q.pop();

        if(i < 0 || j < 0 || i >= R || j >= C || f > F){
            continue;
        }

        // Continue if dangerous square or increase f
        if(map[i][j] == 1){
            f++;
            if(f > F){continue;}
        }

        // End of cycle
        if(i == Tr && j == Tc){
            min_sum = count;
            break;
        }

        // if not initialized square
        if(map_count[i][j][f] == 0){
            map_count[i][j][f] = i == Sr && j == Sc ? 0: INT_MAX; 
        }

        // set min count or continue
        if(count >= map_count[i][j][f] && count != 0){
            continue;
        }
        else{
            map_count[i][j][f] = count;
        }

        q.push({i-1,j,f,count+1});
        q.push({i+1,j,f,count+1});
        q.push({i,j-1,f,count+1});
        q.push({i,j+1,f,count+1});
        
    }
    cout << min_sum << endl;
}