#include<bits/stdc++.h>

using namespace std;

int main(){
    map<int,int> mp;
    vector<int> v;

    int n;
    while(scanf("%d",&n) == 1){
        if(mp[n] == 0){
            mp[n] = 1;
        }
        else if(mp[n] == 1){
            mp[n] = 2;
            v.push_back(n);
        }
    }
    sort(v.begin(), v.end());

    for(int i = 0; i < v.size(); i++){
        cout << " " << v[i];
    }
    cout << endl;
}