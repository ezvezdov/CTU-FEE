#include<bits/stdc++.h>

#define maxHW 100
#define maxN 11
#define pairSize 2

using namespace std;

int minsum = INT_MAX;
int min_cost = INT_MAX;

bool is_suitable(int stations[maxN][pairSize], int stations_count, int D, int cur_i, int cur_j){    
    for(int i = 0; i <= stations_count; i++){
        if(stations[i][0] == cur_i ||
            stations[i][1] == cur_j ||
            abs(cur_i - stations[i][0]) + abs(cur_j - stations[i][1]) < D
        ){
            return false;
        }
    }
    return true;
}

void rec(int streets[maxHW][maxHW],int stations[maxN][pairSize],int i, int j, int sum, int amount, int stations_count, int H, int W, int D){
    if(amount-1 == 0){
        minsum = min(minsum,sum+streets[i][j]);
        return;
    }
    
    sum+=streets[i][j];
    if(sum >= minsum || (amount-1) * min_cost + sum >= minsum){
        return;
    }

    stations[stations_count][0] = i;
    stations[stations_count][1] = j;
    
    for(int cur_i = i+1; H - cur_i >= amount-1; cur_i++){
        for(int cur_j = 0; cur_j < W; cur_j++){
            if(sum + streets[cur_i][cur_j] >= minsum ||
                !is_suitable(stations,stations_count,D,cur_i,cur_j)
            ){
                continue;
            }
            rec(streets,stations,cur_i,cur_j,sum,amount-1,stations_count+1,H,W,D);
        }
    }
    return;
}

int main(){
    int H,W,D,N;
    cin >> H >> W >> D >> N;
    int streets[maxHW][maxHW];
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            scanf("%d",&streets[i][j]);
            min_cost = min(streets[i][j],min_cost);
        }
    }

    int stations[maxN][pairSize];

    for(int i = 0; i <= H - N; i++){
        for(int j = 0; j < W; j++){
            rec(streets,stations, i, j, 0, N,0, H,W,D);        
        }
    }
    cout << minsum << endl;
}
