#include<bits/stdc++.h>

using namespace std;

void genrate_start_arrays(vector<int> &path_robots,vector<int> &light_start,int d1, int d2, int n){
    for(int j = 0; j <= d1 && j < n; j++){
        path_robots[j] = 1;
        light_start[j] = 0;
    }
    for(int j = 0; j <= d2 && (n-1)-j >= 0; j++){
        path_robots[(n-1)-j] = 2;
        light_start[(n-1)-j] = 0;
    }
}

void update_path_robots(vector<int> &path_robots, vector<int> &light_start, vector<int> &path_light, int time_now, int r1_pos, int r2_pos, int robot_num, int d1, int d2, int n){
    if(robot_num == 1){
        if(r1_pos-1 - d1 >= 0){
            if(path_robots[r1_pos-1 - d1] == 1){
                path_robots[r1_pos-1 - d1] = 0;
                path_light[r1_pos-1 - d1] += 2 * (time_now -  light_start[r1_pos-1 - d1]);
                light_start[r1_pos-1 - d1] = -1;
            }
            else if(path_robots[r1_pos-1 - d1] == 3){
                path_robots[r1_pos-1 - d1] = 2;
                path_light[r1_pos-1 - d1] += 3 * (time_now -  light_start[r1_pos-1 - d1]);
                light_start[r1_pos-1 - d1] = time_now;
            }
        }
        if(r1_pos + d1 < n){
            if(path_robots[r1_pos + d1] == 0){
                path_robots[r1_pos + d1] = 1;
                light_start[r1_pos + d1] = time_now;
                
            }
            else if(path_robots[r1_pos + d1] == 2){
                path_robots[r1_pos + d1] = 3;

                path_light[r1_pos + d1] += 2 * (time_now -  light_start[r1_pos + d1]);
                light_start[r1_pos + d1] = time_now;
            }
        }
    }
    if(robot_num == 2){
        if(r2_pos+1 + d2 < n){
            if(path_robots[r2_pos+1 + d2] == 2){
                path_robots[r2_pos+1 + d2] = 0;
                path_light[r2_pos+1 + d2] += 2 * (time_now -  light_start[r2_pos+1 + d2]);
                light_start[r2_pos+1 + d2] = -1;
            }
            else if(path_robots[r2_pos+1 + d2] == 3){
                path_robots[r2_pos+1 + d2] = 1;
                path_light[r2_pos+1 + d2] += 3 * (time_now -  light_start[r2_pos+1 + d2]);
                light_start[r2_pos+1 + d2] = time_now;
            }
        }
        if(r2_pos - d2 >= 0){
            if(path_robots[r2_pos - d2] == 0){
                path_robots[r2_pos - d2] = 2;
                light_start[r2_pos - d2] = time_now;
            }
            else if(path_robots[r2_pos - d2] == 1){
                path_robots[r2_pos - d2] = 3;
                path_light[r2_pos - d2] += 2 * (time_now - light_start[r2_pos - d2]);
                light_start[r2_pos - d2] = time_now;
            }
        }
    }
}

void final_time_update(vector<int> &path_robots, vector<int> &path_light, vector<int> &light_start, int time_now, int r1_pos, int r2_pos, int d1, int d2, int n){
    if(path_robots[r1_pos] == 1){
        path_light[r1_pos] += 2 * (time_now - light_start[r1_pos]);
        light_start[r1_pos] = -1;
    }
    if(path_robots[r2_pos] == 2){
        path_light[r2_pos] += 2 * (time_now - light_start[r2_pos]);
        light_start[r2_pos] = -1;

    }
    
    if(path_robots[r2_pos] == 3){
        path_light[r2_pos] += 3 * (time_now - light_start[r2_pos]);
        light_start[r2_pos] = -1;
    }
    for(int i = 1; i <= max(d1,d2); i++){
        if(i <= d1){
            if(r1_pos + i < n){
                if(path_robots[r1_pos + i] != 3){
                    path_light[r1_pos + i] += 2 * (time_now - light_start[r1_pos + i]);
                    light_start[r1_pos + i] = -1;
                }
            }
            if(r1_pos - i >= 0){
                if(path_robots[r1_pos - i] != 3){
                    path_light[r1_pos - i] += 2 * (time_now - light_start[r1_pos - i]);
                    light_start[r1_pos - i] = -1;
                }
            }
        }
        if(i <= d2){
            if(r2_pos + i < n){
                if(path_robots[r2_pos + i] != 3){
                    path_light[r2_pos + i] += 2 * (time_now - light_start[r2_pos + i]);
                }
                else{
                    path_light[r2_pos + i] += 3 * (time_now - light_start[r2_pos + i]);
                    
                }
                light_start[r2_pos + i] = -1;
            }
            if(r2_pos - i >= 0){
                if(path_robots[r2_pos - i] != 3){
                    path_light[r2_pos - i] += 2 * (time_now - light_start[r2_pos - i]);
                }
                else{
                    path_light[r2_pos - i] += 3 * (time_now - light_start[r2_pos - i]);
                }
                light_start[r2_pos - i] = -1;
            }
        }
    }
}


int main(){
    int n = 1, d1 = 0, d2 = 0;
    cin >> n >> d1 >> d2;
    vector<int> times1,times2, t_all;
    t_all.push_back(0);
    
    int t_cur = 0;
    for(int i = 0; i < n; i++){
        int t_in;
        cin >> t_in;
        times1.push_back(t_in);
        t_cur+=t_in;
        t_all.push_back(t_cur);
    }

    t_cur = t_all[t_all.size()-1];;
    for(int i = 0; i < n; i++){
        int t_in;
        cin >> t_in;
        times2.push_back(t_in);
        t_cur -= t_in;
        t_all.push_back(t_cur);
    }

    sort(t_all.begin(),t_all.end());
    t_all.erase( unique( t_all.begin(), t_all.end() ), t_all.end() );
                
    vector<int> path_robots(n,0); // 0 - without light, 1 - 1st robot, 2 - 2nd robot, 3 - 1st and 2nd robot
    vector<int> path_light(n,0); // path_light[i] = quality on i square
    vector<int> light_start(n,-1); // light_start[i] = time when light appears in i square
    genrate_start_arrays(path_robots,light_start,d1,d2,n); 

    int r1_pos = 0, r2_pos = n-1, r1_ind = 0, r2_ind = n-1;
    int cur_t1 = 0, cur_t2 = 0;
    int t_prev = 0;

    for(int i = 0; i < t_all.size(); i++){
        if(cur_t1 + times1[r1_ind] == t_all[i]){
            r1_pos++;
            r1_ind++;
            cur_t1 = t_all[i];
            update_path_robots(path_robots, light_start, path_light, t_all[i],r1_pos,r2_pos,1,d1,d2,n);
        }
        if(cur_t2 + times2[r2_ind] == t_all[i]){
            r2_pos--;
            r2_ind--;
            cur_t2 = t_all[i];
            update_path_robots(path_robots, light_start, path_light, t_all[i],r1_pos,r2_pos,2,d1,d2,n);
        }

        t_prev = t_all[i]; // update previous time value
    }
    final_time_update(path_robots,path_light,light_start,t_prev,r1_pos,r2_pos,d1,d2,n);

    cout << *min_element(path_light.begin(), path_light.end()) << " " << *max_element(path_light.begin(), path_light.end()) << endl;
}
