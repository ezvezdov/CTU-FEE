// https://youtu.be/8LusJS5-AGo

#include<bits/stdc++.h>

#define MAX_L 1005
#define MAX_N MAX_L

using namespace std;

typedef unsigned long long int ulli;

// Installation cost on i-th km
int s[MAX_L] = {0};

int a,b;
ulli VNUU(int z){
    return a * z * z + b * z;
}

ulli get_new_cost(ulli prev_cost, int prev_station_location, int current_station_location, int L){
    ulli new_cost;
    // Case when we install the first station
    if(prev_cost == 0 && prev_station_location == 0){
        // segment before station +  segment after station + cost of installation
        new_cost = VNUU(current_station_location) + VNUU(L - current_station_location) + s[current_station_location];
    }

    // Case when we install i-th station and use value of (i-1)-th station
    else{
        new_cost =  prev_cost - // cost of installation with i-1 stations (include cost of segments)- 
                VNUU(L - prev_station_location) + // (segment [(i-1) station location] - [L]) +
                VNUU(current_station_location - prev_station_location) + // (segment [(i-1) station location] - [current station location]) +
                VNUU(L - current_station_location) + // (segment [current station location] - [L]) +
                s[current_station_location]; // cost of installation current station
    }
    
    return new_cost;
  
}

int main(){

    int L, N;
    cin >> L >> N >> a >> b;

    for(int i = 1; i < L; i++){
        cin >> s[i];
    }

    ulli answer = ULONG_MAX;
    /*
    dp[i][j] shows cost when i-th station on j-th km
    -------------------------
    | |0|1|2|3|4|5|.....|L-1|
    |1|.|.|.|.|.|.|.....| . |
    |2|.|.|.|.|.|.|.....| . |
    |3|.|.|.|.|.|.|.....| . |
    |.|.|.|.|.|.|.|.....| . |
    |.|.|.|.|.|.|.|.....| . |
    |N|.|.|.|.|.|.|.....| . | 
    -------------------------
    */
    ulli dp[MAX_L][MAX_L];

    for(int i = 1; i <= N; i++){
        for(int j = i; j < L; j++){
            dp[i][j] = ULONG_MAX;

            // Case when we install i-th station and use value of (i-1)-th station.
            // We go from (i-1)-th km because of several stations can't be on one km
            // We go to j-th km because check just values before current station location
            for( int k = i-1; k < j && i != 1; k++){
                dp[i][j] = min(dp[i][j],get_new_cost(dp[i-1][k],k,j,L));
            }

            // Case when we install the first station
            if(i == 1){
                dp[i][j] = get_new_cost(0,0,j,L);
            }

            // Check all values when we install N stations and get min
            if(i == N){
                answer = min(answer,dp[i][j]);
            }
        }
    }

    cout << answer << endl;
}
